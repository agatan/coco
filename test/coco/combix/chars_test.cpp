#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE expected

#include <coco/combix/combinators.hpp>
#include <coco/combix/iterator_stream.hpp>
#include <coco/combix/primitives.hpp>
#include <coco/combix/chars.hpp>

#include <boost/test/unit_test.hpp>

#include <string>

BOOST_AUTO_TEST_SUITE(combix)
BOOST_AUTO_TEST_SUITE(combinators)

  BOOST_AUTO_TEST_CASE(parens) {
    auto const src = std::string("(a)");
    auto s = coco::combix::range_stream(src);
    auto const p = coco::combix::parens(coco::combix::alpha());
    BOOST_TEST(parse(p, s).unwrap() == 'a');
    BOOST_TEST(parse(p, s).is_error());
  }

  BOOST_AUTO_TEST_CASE(digit) {
    auto const src = std::string("1");
    auto s = coco::combix::range_stream(src);
    auto const p = coco::combix::digit();
    BOOST_TEST(parse(p, s).unwrap() == 1);
  }

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
