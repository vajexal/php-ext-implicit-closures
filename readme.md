With this extension you can use closures without `use` statement

[![Build Status](https://github.com/vajexal/php-ext-implicit-closures/workflows/Build/badge.svg)](https://github.com/vajexal/php-ext-implicit-closures/actions)

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
