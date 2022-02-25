--TEST--
Object assign
--EXTENSIONS--
implicit_closures
--FILE--
<?php
$a = new class {
    public int $x;
};

$foo = function() {
    $a->x = 100;
};

$foo();
echo $a->x;
?>
--EXPECT--
100
