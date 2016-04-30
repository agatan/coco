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

  BOOST_AUTO_TEST_CASE(choice) {
    auto src = std::string{"a1B"};
    auto s = cbx::iter_stream(std::begin(src), std::end(src));
    auto const alpha = 
        cbx::satisfy([](auto&& c) { return 'a' <= c && c <= 'z'; });
    auto const num =
        cbx::satisfy([](auto&& c) { return '0' <= c && c <= '9'; });
    auto const p = cbx::choice(
        alpha,
        num);

    BOOST_TEST(parse(p, s).unwrap() == 'a');
    BOOST_TEST(parse(p, s).unwrap() == '1');
    BOOST_TEST(parse(p, s).is_error());
  }

  BOOST_AUTO_TEST_CASE(map) {
    auto src = std::string{"1"};
    auto s = cbx::iter_stream(std::begin(src), std::end(src));
    auto const p = cbx::map(
        cbx::satisfy([](auto&& c) { return '0' <= c && c <= '9'; }),
        [](auto&& c) { return static_cast<int>(c) - '0'; });

    BOOST_TEST(parse(p, s).unwrap() == 1);
    BOOST_TEST(parse(p, s).is_error());
  }

  BOOST_AUTO_TEST_CASE(seq) {
    auto src = std::string{"123"};
    auto s = cbx::iter_stream(std::begin(src), std::end(src));
    auto const digit = cbx::map(
        cbx::satisfy([](auto&& c) { return '0' <= c && c <= '9'; }),
        [](auto&& c) { return static_cast<int>(c) - '0'; });
    auto const p = cbx::seq(digit, digit, digit);

    BOOST_ASSERT(parse(p, s).unwrap() == std::make_tuple(1, 2, 3));
    BOOST_TEST(std::string(s.begin(), s.end()) == "");
    BOOST_TEST(parse(p, s).is_error());
  }

  BOOST_AUTO_TEST_CASE(many) {
    auto src = std::string{"123"};
    auto s = cbx::iter_stream(std::begin(src), std::end(src));
    auto const digit = cbx::map(
        cbx::satisfy([](auto&& c) { return '0' <= c && c <= '9'; }),
        [](auto&& c) { return static_cast<int>(c) - '0'; });
    auto const p = cbx::many(digit);

    BOOST_TEST((p(s).unwrap() == std::list<int>{1, 2, 3}));
  }

  BOOST_AUTO_TEST_CASE(many_with_zero_input) {
    auto src = std::string{"abc"};
    auto s = cbx::iter_stream(std::begin(src), std::end(src));
    auto const digit = cbx::map(
        cbx::satisfy([](auto&& c) { return '0' <= c && c <= '9'; }),
        [](auto&& c) { return static_cast<int>(c) - '0'; });
    auto const p = cbx::many(digit);

    BOOST_TEST((p(s).unwrap() == std::list<int>{}));
  }

  BOOST_AUTO_TEST_CASE(many1) {
    auto src = std::string{"123"};
    auto s = cbx::iter_stream(std::begin(src), std::end(src));
    auto const digit = cbx::map(
        cbx::satisfy([](auto&& c) { return '0' <= c && c <= '9'; }),
        [](auto&& c) { return static_cast<int>(c) - '0'; });
    auto const p = cbx::many1(digit);

    BOOST_TEST((parse(p, s).unwrap() == std::list<int>{1, 2, 3}));
  }

  BOOST_AUTO_TEST_CASE(many1_with_zero_input) {
    auto src = std::string{"abc"};
    auto s = cbx::iter_stream(std::begin(src), std::end(src));
    auto const digit = cbx::map(
        cbx::satisfy([](auto&& c) { return '0' <= c && c <= '9'; }),
        [](auto&& c) { return static_cast<int>(c) - '0'; });
    auto const p = cbx::many1(digit);

    BOOST_TEST(parse(p, s).is_error());
  }

  BOOST_AUTO_TEST_CASE(chainl1) {
    auto src = std::string{"1+2+3"};
    auto s = cbx::range_stream(src);
    auto const digit = cbx::map(
        cbx::satisfy([](auto&& c) { return '1' <= c && c <= '9'; }),
        [](auto&& c) { return static_cast<int>(c) - '0'; });
    auto const add = cbx::map(cbx::token('+'), [](auto&&) {
      return std::plus<int>();
    });
    auto const expr = cbx::chainl1(digit, add);

    BOOST_TEST(parse(expr, s).unwrap() == 6);
    BOOST_TEST(std::string(s.begin(), s.end()) == "");
  }

  BOOST_AUTO_TEST_CASE(between) {
    auto src = std::string{"(a)"};
    auto s = cbx::range_stream(src);
    auto const alpha =
        cbx::satisfy([](auto&& c) { return 'a' <= c && c <= 'z'; });
    auto const p = cbx::between(cbx::token('('),
                                         cbx::token(')'), alpha);

    BOOST_TEST(parse(p, s).unwrap() == 'a');
    BOOST_TEST(std::string(s.begin(), s.end()) == "");
  }

  BOOST_AUTO_TEST_CASE(expected) {
    auto src = std::string("hoge");
    auto s = cbx::range_stream(src);
    auto const lower = cbx::expected(
        cbx::satisfy([](auto&& c) { return 'a' <= c && c <= 'z'; }),
        "lower");

    BOOST_TEST(parse(lower, s).unwrap() == 'h');
  }

  BOOST_AUTO_TEST_CASE(skip) {
    auto src = std::string("1 2 3");
    auto s = cbx::range_stream(src);

    auto const p = cbx::many(cbx::skip(cbx::digit(), cbx::spaces()));

    BOOST_TEST((parse(p, s).unwrap() == std::list<char>{'1', '2', '3'}));
  }

  BOOST_AUTO_TEST_CASE(skip_seq) {
    auto src = std::string{"1 a 2"};
    auto s = cbx::range_stream(src);

    auto const p =
        cbx::skip_seq(cbx::spaces())(cbx::digit(), cbx::alpha(), cbx::digit());

    BOOST_TEST((parse(p, s).unwrap() == std::make_tuple('1', 'a', '2')));
  }

  BOOST_AUTO_TEST_CASE(sep_by) {
    auto src = std::string("1,2,3");
    auto s = cbx::range_stream(src);

    auto const p = cbx::sep_by(cbx::digit(), cbx::token(','));

    BOOST_TEST((parse(p, s).unwrap() == std::list<char>{'1', '2', '3'}));
  }

  BOOST_AUTO_TEST_CASE(try_) {
    auto src = std::string{"1a"};
    auto s = cbx::range_stream(src);

    auto const p = cbx::seq(cbx::digit(), cbx::digit());
    auto const try_p = cbx::try_(p);

    BOOST_TEST(parse(try_p, s).is_error());
    BOOST_TEST(std::string(s.begin(), s.end()) == "1a");

    BOOST_TEST(parse(p, s).is_error());
    BOOST_TEST(std::string(s.begin(), s.end()) == "a");
  }

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
