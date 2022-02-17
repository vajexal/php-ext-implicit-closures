--TEST--
Closure with implicit use
--EXTENSIONS--
implicit_closures
--FILE--
<?php
$x = 100;

$foo = function() {
    return $x;
};

echo $foo();
?>
--EXPECT--
100
