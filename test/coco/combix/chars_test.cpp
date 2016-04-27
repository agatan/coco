#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE expected

#include <coco/combix/combinators.hpp>
#include <coco/combix/iterator_stream.hpp>
#include <coco/combix/primitives.hpp>
#include <coco/combix/chars.hpp>

#include <boost/test/unit_test.hpp>

#include <string>

namespace cbx = coco::combix;

BOOST_AUTO_TEST_SUITE(combix)
BOOST_AUTO_TEST_SUITE(combinators)

  BOOST_AUTO_TEST_CASE(parens) {
    auto const src = std::string("(a)");
    auto s = cbx::range_stream(src);
    auto const p = cbx::parens(cbx::alpha());
    BOOST_TEST(parse(p, s).unwrap() == 'a');
    BOOST_TEST(parse(p, s).is_error());
  }

  BOOST_AUTO_TEST_CASE(digit) {
    auto const src = std::string("1");
    auto s = cbx::range_stream(src);
    auto const p = cbx::digit();
    BOOST_TEST(parse(p, s).unwrap() == 1);
  }

  BOOST_AUTO_TEST_CASE(string) {
    auto const src = std::string("string");
    auto s = cbx::range_stream(src);
    auto const p = cbx::string("string");

    BOOST_TEST(parse(p, s).unwrap() == "string");
  }

  BOOST_AUTO_TEST_CASE(spaces) {
    auto const space = cbx::space();
    auto const spaces = cbx::spaces();

    {
      auto const src = std::string("non-space");
      auto s = cbx::range_stream(src);

      BOOST_TEST(parse(space, s).is_error());
    }
    {
      auto const src = std::string(" \t\n");
      auto s = cbx::range_stream(src);

      BOOST_TEST(parse(spaces, s).is_ok());
    }
  }

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
