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

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
