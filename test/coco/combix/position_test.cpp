#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE expected

#include <coco/combix/combinators.hpp>
#include <coco/combix/iterator_stream.hpp>
#include <coco/combix/primitives.hpp>
#include <coco/combix/chars.hpp>
#include <coco/combix/parser.hpp>
#include <coco/combix/lazy_fun.hpp>
#include <coco/combix/positioned_stream.hpp>

#include <boost/test/unit_test.hpp>

#include <string>

namespace cbx = coco::combix;
template <typename>
class td;

BOOST_AUTO_TEST_SUITE(combix)
BOOST_AUTO_TEST_SUITE(position)

  BOOST_AUTO_TEST_CASE(parse) {
    std::string src{"1"};
    auto s = cbx::range_stream(src);
    cbx::positioned_stream<decltype(s), cbx::source_position> pos_stream{
        std::move(s)};

    auto const p = cbx::any();

    BOOST_TEST(cbx::parse(p, pos_stream).unwrap() == '1');
    BOOST_TEST(pos_stream.position().line == 1);
    BOOST_TEST(pos_stream.position().column == 1);
  }

  BOOST_AUTO_TEST_CASE(newline) {
    std::string src{"1\n2\n3"};
    auto s = cbx::make_positioned<cbx::source_position>(cbx::range_stream(src));

    auto const p = cbx::sep_by(cbx::digit(), cbx::token('\n'));

    BOOST_TEST(cbx::parse(p, s).is_ok());
    BOOST_TEST(s.position().line == 3);
    BOOST_TEST(s.position().column == 1);
  }

  BOOST_AUTO_TEST_CASE(error) {
    std::string src{"1\n2\na\n2"};
    auto s = cbx::make_positioned<cbx::source_position>(cbx::range_stream(src));

    auto const p = cbx::sep_by(cbx::digit(), cbx::token('\n'));

    auto res = cbx::parse(p, s);

    BOOST_TEST(res.is_error());
    auto&& pos = res.unwrap_error().position();
    BOOST_TEST(pos.line == 3);
    BOOST_TEST(pos.column == 1);
  }

  BOOST_AUTO_TEST_CASE(positioned) {
    std::string src{"(a)"};
    auto s = cbx::make_positioned<cbx::source_position>(cbx::range_stream(src));

    auto const p = cbx::positioned(
        cbx::between(cbx::token('('), cbx::token(')'), cbx::alpha()));

    auto res = cbx::parse(p, s);
    BOOST_TEST(res.is_ok());
    BOOST_TEST(res.unwrap().second == 'a');
    BOOST_TEST(res.unwrap().first.line == 1);
    BOOST_TEST(res.unwrap().first.column == 0);
  }

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
