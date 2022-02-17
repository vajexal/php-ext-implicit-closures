With this extension you can use closures without `use` statement

```php
$x = 100;

$foo = function() {
    return $x;
};

echo $foo(), PHP_EOL;
```

### Installation

```bash
phpize
./configure
make
make install
```

### Testing

```bash
make test
```
