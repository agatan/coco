This is my utility library on C++.  
`coco` is just getting started.

## Components

### expected

`coco::expected` is a type that represents an errorable value.  
It has a valur or an error.

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

auto const src = std::string{"ab1"};
auto s = coco::combix::iter_stream(std::begin(src), std::end(src));
auto const any_p = coco::combix::any();
auto const lower_p = coco::combix::satisfy([](auto&& c) { return 'a' <= c && c <= 'z'; });
auto const is_a_p = coco::combix::token('a');

{
  auto saved = coco::combix::save(s);
  assert(any_p.parse(saved).is_ok());
}
{
  auto saved = coco::combix::save(s);
  assert(lower_p.parse(saved).unwrap() == 'a');
  assert(lower_p.parse(saved).unwrap() == 'b');
  assert(lower_p.parse(saved).is_error());
}
{
  auto saved = coco::combix::save(s);
  assert(is_a_p.parse(saved).unwrap() == 'a');
  assert(is_a_p.parse(saved).is_error());
}
```


## LICENSE

This library is released under [the Boost Software License](http://boost.org/LICENSE_1_0.txt)
