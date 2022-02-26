--TEST--
Foreach
--EXTENSIONS--
implicit_closures
--FILE--
<?php
$x = [1];

$foo = function() {
    $sum = 0;
    foreach ($x as $val) {
        $sum += $val;
    }
    foreach ([2 => 3] as $key => $value) {
        $sum += $key + $value;
    }
    return $sum;
};

echo $foo();
?>
--EXPECT--
6
