#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE expected

#include <coco/combix/combinators.hpp>
#include <coco/combix/iterator_stream.hpp>
#include <coco/combix/primitives.hpp>
#include <coco/combix/chars.hpp>
#include <coco/combix/parser.hpp>
#include <coco/combix/lazy_fun.hpp>

#include <boost/test/unit_test.hpp>

#include <string>

namespace cbx = coco::combix;

using stream_type = cbx::iterator_stream<std::string::const_iterator>;

cbx::parser<int, stream_type> expression();

cbx::parser<int, stream_type> number() {
  return cbx::map(cbx::many1(cbx::digit()), [](auto&& is) {
    int acc = 0;
    for (auto i : is) {
      acc = acc * 10 + i;
    }
    return acc;
  });
}

cbx::parser<int, stream_type> factor() {
  return cbx::choice(number(), cbx::parens(cbx::lazy_fun(expression)));
}

cbx::parser<int, stream_type> expression() {
  return factor();
}


BOOST_AUTO_TEST_SUITE(combix)
BOOST_AUTO_TEST_SUITE(combinators)

  BOOST_AUTO_TEST_CASE(parens) {
    auto const src = std::string("(1234)");
    auto s = coco::combix::range_stream(src);
    auto const p = expression();
    BOOST_TEST(parse(p, s).unwrap() == 1234);
    BOOST_TEST(parse(p, s).is_error());
  }

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
