--TEST--
Try catch
--EXTENSIONS--
implicit_closures
--FILE--
<?php
$foo = function() {
    try {
        return 100;
    } catch (Exception $e) {
        return $e->getCode();
    }
};

echo $foo();
?>
--EXPECT--
100
