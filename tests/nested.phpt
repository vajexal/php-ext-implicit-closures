--TEST--
Nested closures
--EXTENSIONS--
implicit_closures
--FILE--
<?php
$x = 100;

$foo = function() {
    return function () {
        return $x;
    };
};

echo $foo()();
?>
--EXPECT--
100
