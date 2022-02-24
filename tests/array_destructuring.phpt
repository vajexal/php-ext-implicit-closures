--TEST--
Array destructuring
--EXTENSIONS--
implicit_closures
--FILE--
<?php
$foo = function() {
    [$a, $b] = [1, 2];
    list($c, $d) = [3, 4];
    ['e' => $e] = ['e' => 5];
    return $a + $b + $c + $d + $e;
};

echo $foo();
?>
--EXPECT--
15
