#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE expected

#include <coco/combix/iterator_stream.hpp>
#include <coco/combix/primitives.hpp>
#include <coco/combix/combinators.hpp>

#include <boost/test/unit_test.hpp>

#include <string>

BOOST_AUTO_TEST_SUITE(combix)
BOOST_AUTO_TEST_SUITE(combinators)

  BOOST_AUTO_TEST_CASE(choice) {
    auto src = std::string{"a1B"};
    auto s = coco::combix::iter_stream(std::begin(src), std::end(src));
    auto const p = coco::combix::choice(
        coco::combix::satisfy([](auto&& c) { return 'a' <= c && c <= 'z'; }),
        coco::combix::satisfy([](auto&& c) { return '0' <= c && c <= '9'; }));

    BOOST_TEST(p.parse(s).unwrap() == 'a');
    BOOST_TEST(p.parse(s).unwrap() == '1');
    BOOST_TEST(p.parse(s).is_error());
  }

  BOOST_AUTO_TEST_CASE(map) {
    auto src = std::string{"1"};
    auto s = coco::combix::iter_stream(std::begin(src), std::end(src));
    auto const p = coco::combix::map(
        coco::combix::satisfy([](auto&& c) { return '0' <= c && c <= '9'; }),
        [](auto&& c) { return static_cast<int>(c) - '0'; });

    BOOST_TEST(p.parse(s).unwrap() == 1);
    BOOST_TEST(p.parse(s).is_error());
  }

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
