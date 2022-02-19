--TEST--
Undefined variable
--EXTENSIONS--
implicit_closures
--FILE--
<?php
$foo = function() {
    return $x;
};

echo $foo();
?>
--EXPECTF--
Warning: Undefined variable $x in %s on line 3
