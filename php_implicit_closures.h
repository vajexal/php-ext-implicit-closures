/* implicit_closures extension for PHP */

#ifndef PHP_IMPLICIT_CLOSURES_H
# define PHP_IMPLICIT_CLOSURES_H

extern zend_module_entry implicit_closures_module_entry;
# define phpext_implicit_closures_ptr &implicit_closures_module_entry

# define PHP_IMPLICIT_CLOSURES_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_IMPLICIT_CLOSURES)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_IMPLICIT_CLOSURES_H */
