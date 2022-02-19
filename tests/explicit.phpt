--TEST--
Explicit use
--EXTENSIONS--
implicit_closures
--FILE--
<?php
$x = 100;

$foo = function() use ($x) {
    return $x;
};

echo $foo();
?>
--EXPECT--
100
