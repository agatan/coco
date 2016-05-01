#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE expected

#include <coco/combix/iterator_stream.hpp>
#include <coco/combix/primitives.hpp>

#include <boost/test/unit_test.hpp>

#include <string>

BOOST_AUTO_TEST_SUITE(combix)
BOOST_AUTO_TEST_SUITE(primitives)

  BOOST_AUTO_TEST_CASE(any) {
    auto src = std::string{"test"};
    auto s = coco::combix::iter_stream(std::begin(src), std::end(src));
    auto const p = coco::combix::any();

    BOOST_TEST(parse(p, s).unwrap() == 't');
    BOOST_TEST(parse(p, s).unwrap() == 'e');
    BOOST_TEST(parse(p, s).unwrap() == 's');
    BOOST_TEST(parse(p, s).unwrap() == 't');
    BOOST_TEST(parse(p, s).is_error());
  }

  BOOST_AUTO_TEST_CASE(satisfy) {
    auto src = std::string{"test"};
    auto s = coco::combix::iter_stream(std::begin(src), std::end(src));
    auto const p =
        coco::combix::satisfy([](auto&& c) { return 'a' <= c && c <= 'z'; });

    BOOST_TEST(parse(p, s).is_ok());
    BOOST_TEST(parse(p, s).is_ok());
    BOOST_TEST(parse(p, s).is_ok());
    BOOST_TEST(parse(p, s).is_ok());
    BOOST_TEST(parse(p, s).is_error());

    src = std::string{"a1"};
    s = coco::combix::iter_stream(std::begin(src), std::end(src));

    BOOST_TEST(parse(p, s).unwrap() == 'a');
    BOOST_TEST(parse(p, s).is_error());
    BOOST_TEST(*s.begin() == '1');
  }

  BOOST_AUTO_TEST_CASE(token) {
    auto src = std::string{"test"};
    auto s = coco::combix::iter_stream(std::begin(src), std::end(src));
    auto const p = coco::combix::token('t');

    BOOST_TEST(parse(p, s).unwrap() == 't');
    BOOST_TEST(parse(p, s).is_error());
    BOOST_TEST(*(s.begin()) == 'e');
  }

  BOOST_AUTO_TEST_CASE(eof) {
    auto src = std::string{"a"};
    auto s = coco::combix::range_stream(src);
    auto const p = coco::combix::eof();

    BOOST_TEST(parse(p, s).is_error());

    src = "";
    s = coco::combix::range_stream(src);
    BOOST_TEST(parse(p, s).is_ok());
  }


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
