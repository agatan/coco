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

BOOST_AUTO_TEST_SUITE(combix)
BOOST_AUTO_TEST_SUITE(combinators)

  BOOST_AUTO_TEST_CASE(parens) {
    auto const src = std::string("()");
    auto s = coco::combix::range_stream(src);
    auto const p = cbx::choice(
        cbx::alpha(), cbx::map(cbx::digit(), [](auto) { return '!'; }));
    auto res = parse(p, s);
    BOOST_TEST(res.is_error());
    std::stringstream ss;
    ss << res.unwrap_error();
    BOOST_TEST(ss.str() == "Unexpected \"(\"\nExpected alphabet, digit\n");
  }

  BOOST_AUTO_TEST_CASE(seq) {
    auto const src = std::string{"a1c"};
    auto s = cbx::range_stream(src);
    auto const p = cbx::seq(cbx::alpha(), cbx::digit(), cbx::digit());

    auto res = parse(p, s);
    BOOST_TEST(res.is_error());
    std::stringstream ss;
    ss << res.unwrap_error();
    BOOST_TEST(ss.str() == "Unexpected \"c\"\nExpected digit\n");
  }

  BOOST_AUTO_TEST_CASE(expected) {
    auto const src = std::string{"1"};
    auto s = cbx::range_stream(src);
    auto const p = cbx::expected(cbx::alpha(), "letter");

    auto res = parse(p, s);
    BOOST_TEST(res.is_error());
    std::stringstream ss;
    ss << res.unwrap_error();
    BOOST_TEST(ss.str() == "Unexpected \"1\"\nExpected letter\n");
  }

  BOOST_AUTO_TEST_CASE(consumed) {
    auto const src = std::string{"1b2"};
    auto s = cbx::range_stream(src);
    auto const p = cbx::seq(cbx::digit(), cbx::digit(), cbx::digit());

    auto res = cbx::parse(p, s);
    BOOST_TEST(res.is_error());
    BOOST_TEST(res.unwrap_error().consumed());
    BOOST_TEST(std::string(s.begin(), s.end()) == "b2");

    auto const manyp = cbx::many(p);
    s = cbx::range_stream(src);

    auto res2 = cbx::parse(manyp, s);
    BOOST_TEST(res2.is_error());
    BOOST_TEST(res2.unwrap_error().consumed());
    BOOST_TEST(std::string(s.begin(), s.end()) == "b2");
  }

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
