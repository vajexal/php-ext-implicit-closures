--TEST--
Closure with local var
--EXTENSIONS--
implicit_closures
--FILE--
<?php
$foo = function() {
    $x = 1;
    return $x;
};

echo $foo();
?>
--EXPECT--
1
