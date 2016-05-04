Parser combinator library for C++.  
`coco` is just getting started.

## Install

`coco` depends on boost 1.59.0.

```
git clone https://github.com/agatan/coco
cd coco
mkdir build && cd build
cmake ..
make
make install
```

## Components

### expected

`coco::expected` is a type that represents an errorable value.  
It has a value or an error.

```c++
#include <coco/expected.hpp>

coco::expected<int, std::string> fact(int n) {
  if (n < 0) {
    return coco::make_error("negative number");
  }
  if (n == 0 {
    return 1;
  }
  return fact(n - 1).flat_map([=](auto i) { return n * i; });
}

auto result = fact(3);
assert(result.is_ok());
assert(result.unwrap() == 6);
assert(*result == 6);
assert(result.map([](auto n) { return n + 1; }).unwrap() == 7);

result = fact(-100);
assert(result.is_error());
assert(result.unwrap_error() == "negative number");
```

### combix

`coco::combix` is a parser combinator component inspired by [Parsec](https://hackage.haskell.org/package/parsec) and [combine](https://github.com/Marwes/combine).  
`coco::combix` can be applied to any stream (not only character stream but also byte stream, token stream, etc...)  
This component is under development.

#### usage

include `<coco/combix.hpp>`.  
`coco::combix` is undocumented yet... See the `test/` that contains all of `combix` parsers and combinators tests.


#### example

[agatan/coco-combix-demo](https://github.com/agatan/coco-combix-demo) is `coco::combix`'s demo repository, parsing arithmetic expressions and calculating thier values by `coco::combix`.  
[agatan/scomp](https://github.com/agatan/scomp) is a compiler using `coco::combix`.

## LICENSE

This library is released under [the Boost Software License](http://boost.org/LICENSE_1_0.txt)
