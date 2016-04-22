This is my utility library on C++.  
`coco` is just getting started.

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
This component is under development.

```c++
#include <coco/combix/primitives.hpp>
#include <coco/combix/combinators.hpp>
#include <coco/combix/iterator_stream.hpp>

#include <iostream>
#include <iterator>
#include <string>

auto expression() {
  auto number = coco::combix::map(
      coco::combix::satisfy([](auto&& c) { return '0' <= c && c <= '9'; }),
      [](auto&& c) { return static_cast<int>(c) - '0'; });

  return number;
}

int main() {
  char const src[] = "2";
  auto s = coco::combix::iter_stream(std::begin(src), std::end(src));

  auto n = expression();

  std::cout << n.parse(s).unwrap() << std::endl;
}
```

#### primitives

- `any`
- `satisfy`
- `token`

#### combinators

- `map`
- `choice`
- `seq`

#### TODO

- [ ] `lookahead`
- [ ] `not_followed_by`
- [ ] `many`
- [ ] `many1`
- [ ] `chainl`

## LICENSE

This library is released under [the Boost Software License](http://boost.org/LICENSE_1_0.txt)
