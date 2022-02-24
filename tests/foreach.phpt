--TEST--
Foreach
--EXTENSIONS--
implicit_closures
--FILE--
<?php
$foo = function() {
    foreach ([1 => 2] as $key => $value) {
        return $key + $value;
    }
};

echo $foo();
?>
--EXPECT--
3
