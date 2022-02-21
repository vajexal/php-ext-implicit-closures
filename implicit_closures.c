/* implicit_closures extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_implicit_closures.h"

void (*original_ast_process_function)(zend_ast *ast);

static zend_string *get_var_name_from_ast(zend_ast *ast) {
    if (!ast) {
        return NULL;
    }

    zend_ast *name_ast = ast->child[0];

    if (name_ast->kind != ZEND_AST_ZVAL || Z_TYPE_P(zend_ast_get_zval(name_ast)) != IS_STRING) {
        return NULL;
    }

    zend_string *name = zend_ast_get_str(name_ast);

    if (zend_is_auto_global(name) || zend_string_equals_literal(name, "this")) {
        return NULL;
    }

    return name;
}

//region copy-paste from zend_compile.c with some modifications
typedef struct {
    HashTable uses;
    HashTable locals;
    bool varvars_used;
} closure_info;

static void find_implicit_binds_recursively(closure_info *info, zend_ast *ast) {
    if (!ast) {
        return;
    }

    if ((ast->kind == ZEND_AST_ASSIGN || ast->kind == ZEND_AST_ASSIGN_REF) && ast->child[0]->kind == ZEND_AST_VAR) {
        zend_string *name = get_var_name_from_ast(ast->child[0]);
        if (name) {
            zend_hash_add_empty_element(&info->locals, name);
        }
        find_implicit_binds_recursively(info, ast->child[1]);
    } else if (ast->kind == ZEND_AST_VAR) {
        zend_string *name = get_var_name_from_ast(ast);
        if (name) {
            zval lineno;
            ZVAL_LONG(&lineno, ast->lineno);
            zend_hash_add(&info->uses, name, &lineno);
        } else {
            zend_ast *name_ast = ast->child[0];
            if (name_ast->kind != ZEND_AST_ZVAL || Z_TYPE_P(zend_ast_get_zval(name_ast)) != IS_STRING) {
                info->varvars_used = 1;
                find_implicit_binds_recursively(info, name_ast);
            }
        }
    } else if (zend_ast_is_list(ast)) {
        zend_ast_list *list = zend_ast_get_list(ast);
        uint32_t i;
        for (i = 0; i < list->children; i++) {
            find_implicit_binds_recursively(info, list->child[i]);
        }
    } else if (ast->kind == ZEND_AST_CLOSURE) {
        /* For normal closures add the use() list. */
        zend_ast_decl *closure_ast = (zend_ast_decl *) ast;
        zend_ast *uses_ast = closure_ast->child[1];
        if (uses_ast) {
            zend_ast_list *uses_list = zend_ast_get_list(uses_ast);
            uint32_t i;
            for (i = 0; i < uses_list->children; i++) {
                zval lineno;
                ZVAL_LONG(&lineno, ast->lineno);
                zend_hash_add(&info->uses, zend_ast_get_str(uses_list->child[i]), &lineno);
            }
        }
    } else if (ast->kind == ZEND_AST_ARROW_FUNC) {
        /* For arrow functions recursively check the expression. */
        zend_ast_decl *closure_ast = (zend_ast_decl *) ast;
        find_implicit_binds_recursively(info, closure_ast->child[2]);
    } else if (!zend_ast_is_special(ast)) {
        uint32_t i, children = zend_ast_get_num_children(ast);
        for (i = 0; i < children; i++) {
            find_implicit_binds_recursively(info, ast->child[i]);
        }
    }
}
//endregion

static void find_implicit_binds(closure_info *info, zend_ast *params_ast, zend_ast *stmt_ast, zend_ast *uses_ast) {
    zend_ast_list *param_list = zend_ast_get_list(params_ast);
    zend_ast_list *use_list = zend_ast_get_list(uses_ast);

    zend_hash_init(&info->uses, param_list->children + use_list->children, NULL, NULL, 0);
    zend_hash_init(&info->locals, HT_MIN_SIZE, NULL, NULL, 0);

    find_implicit_binds_recursively(info, stmt_ast);

    // remove parameter variables
    for (uint32_t i = 0; i < param_list->children; i++) {
        zend_ast *param_ast = param_list->child[i];
        zend_hash_del(&info->uses, zend_ast_get_str(param_ast->child[1]));
    }

    // remove use variables
    for (uint32_t i = 0; i < use_list->children; i++) {
        zend_ast *use_ast = use_list->child[i];
        zend_hash_del(&info->uses, zend_ast_get_str(use_ast));
    }

    // remove closure's local variables
    zend_string *var_name;
    ZEND_HASH_FOREACH_STR_KEY(&info->locals, var_name) {
        zend_hash_del(&info->uses, var_name);
    } ZEND_HASH_FOREACH_END();
}

static void make_implicit_bindings(zend_ast **ast_ptr, void *context) {
    zend_ast *ast = *ast_ptr;

    if (ast == NULL) {
        return;
    }

    if (ast->kind == ZEND_AST_CLOSURE) {
        zend_ast_decl *decl = (zend_ast_decl *) ast;
        zend_ast *params_ast = decl->child[0];
        zend_ast *uses_ast = decl->child[1];
        zend_ast *stmt_ast = decl->child[2];

        make_implicit_bindings(&stmt_ast, NULL); // go deeper to handle nested closures first

        closure_info info;
        memset(&info, 0, sizeof(closure_info));

        if (!uses_ast) {
            uses_ast = zend_ast_create_list(0, ZEND_AST_CLOSURE_USES);
        }

        find_implicit_binds(&info, params_ast, stmt_ast, uses_ast);

        zend_string *var_name;
        zval *lineno;
        ZEND_HASH_FOREACH_STR_KEY_VAL(&info.uses, var_name, lineno) {
            zend_ast *use_var = zend_ast_create_zval_from_str(var_name);
            Z_LINENO(((zend_ast_zval *)use_var)->val) = Z_LVAL_P(lineno);
            uses_ast = zend_ast_list_add(uses_ast, use_var);
        } ZEND_HASH_FOREACH_END();

        zend_hash_destroy(&info.uses);
        zend_hash_destroy(&info.locals);

        decl->child[1] = uses_ast;

        return;
    }

    zend_ast_apply(ast, make_implicit_bindings, NULL);
}

void implicit_closures_ast_process(zend_ast *ast) {
    make_implicit_bindings(&ast, NULL);

    if (original_ast_process_function) {
        original_ast_process_function(ast);
    }
}

PHP_MINIT_FUNCTION(implicit_closures) {
    original_ast_process_function = zend_ast_process;
    zend_ast_process = implicit_closures_ast_process;

    return SUCCESS;
}

PHP_RINIT_FUNCTION(implicit_closures) {
#if defined(ZTS) && defined(COMPILE_DL_IMPLICIT_CLOSURES)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    return SUCCESS;
}

PHP_MINFO_FUNCTION(implicit_closures) {
    php_info_print_table_start();
    php_info_print_table_header(2, "implicit_closures support", "enabled");
    php_info_print_table_end();
}

static const zend_function_entry ext_functions[] = {
    ZEND_FE_END
};

zend_module_entry implicit_closures_module_entry = {
    STANDARD_MODULE_HEADER,
    "implicit_closures",
    ext_functions,
    PHP_MINIT(implicit_closures),
    NULL,
    PHP_RINIT(implicit_closures),
    NULL,
    PHP_MINFO(implicit_closures),
    PHP_IMPLICIT_CLOSURES_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_IMPLICIT_CLOSURES
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(implicit_closures)
#endif
