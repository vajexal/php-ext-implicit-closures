--TEST--
Closure in method
--EXTENSIONS--
implicit_closures
--FILE--
<?php
class Foo
{
    public function bar($key)
    {
        return function () {
            return $key;
        };
    }
}

$foo = new Foo();

echo $foo->bar(100)();
?>
--EXPECT--
100
