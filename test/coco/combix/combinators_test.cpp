#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE expected

#include <coco/combix/combinators.hpp>
#include <coco/combix/iterator_stream.hpp>
#include <coco/combix/primitives.hpp>

#include <boost/test/unit_test.hpp>

#include <string>

BOOST_AUTO_TEST_SUITE(combix)
BOOST_AUTO_TEST_SUITE(combinators)

  BOOST_AUTO_TEST_CASE(choice) {
    auto src = std::string{"a1B"};
    auto s = coco::combix::iter_stream(std::begin(src), std::end(src));
    auto const alpha = 
        coco::combix::satisfy([](auto&& c) { return 'a' <= c && c <= 'z'; });
    static_assert(coco::combix::is_parser_v<decltype(alpha), decltype(s)>, "");
    auto const num =
        coco::combix::satisfy([](auto&& c) { return '0' <= c && c <= '9'; });
    auto const p = coco::combix::choice(
        alpha,
        num);

    BOOST_TEST(p(s).unwrap() == 'a');
    BOOST_TEST(p(s).unwrap() == '1');
    BOOST_TEST(p(s).is_error());
  }

  BOOST_AUTO_TEST_CASE(map) {
    auto src = std::string{"1"};
    auto s = coco::combix::iter_stream(std::begin(src), std::end(src));
    auto const p = coco::combix::map(
        coco::combix::satisfy([](auto&& c) { return '0' <= c && c <= '9'; }),
        [](auto&& c) { return static_cast<int>(c) - '0'; });

    BOOST_TEST(p(s).unwrap() == 1);
    BOOST_TEST(p(s).is_error());
  }

  BOOST_AUTO_TEST_CASE(seq) {
    auto src = std::string{"123"};
    auto s = coco::combix::iter_stream(std::begin(src), std::end(src));
    auto const digit = coco::combix::map(
        coco::combix::satisfy([](auto&& c) { return '0' <= c && c <= '9'; }),
        [](auto&& c) { return static_cast<int>(c) - '0'; });
    auto const p = coco::combix::seq(digit, digit, digit);

    BOOST_ASSERT(p(s).unwrap() == std::make_tuple(1, 2, 3));
    BOOST_TEST(std::string(s.begin(), s.end()) == "");
    BOOST_TEST(p(s).is_error());
  }

  BOOST_AUTO_TEST_CASE(many) {
    auto src = std::string{"123"};
    auto s = coco::combix::iter_stream(std::begin(src), std::end(src));
    auto const digit = coco::combix::map(
        coco::combix::satisfy([](auto&& c) { return '0' <= c && c <= '9'; }),
        [](auto&& c) { return static_cast<int>(c) - '0'; });
    auto const p = coco::combix::many(digit);

    BOOST_TEST((p(s).unwrap() == std::vector<int>{1, 2, 3}));
  }

  BOOST_AUTO_TEST_CASE(many_with_zero_input) {
    auto src = std::string{"abc"};
    auto s = coco::combix::iter_stream(std::begin(src), std::end(src));
    auto const digit = coco::combix::map(
        coco::combix::satisfy([](auto&& c) { return '0' <= c && c <= '9'; }),
        [](auto&& c) { return static_cast<int>(c) - '0'; });
    auto const p = coco::combix::many(digit);

    BOOST_TEST((p(s).unwrap() == std::vector<int>{}));
  }

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
