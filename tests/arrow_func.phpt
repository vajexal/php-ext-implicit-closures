--TEST--
Arrow function
--EXTENSIONS--
implicit_closures
--FILE--
<?php
$a = 10;

$foo = function() {
    return fn($b) => $a + $b;
};

echo $foo()(5);
?>
--EXPECT--
15
