#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE expected

#include <coco/combix/iterator_stream.hpp>
#include <coco/combix/primitives.hpp>
#include <coco/combix/combinators.hpp>
#include <coco/combix/parser.hpp>

#include <boost/test/unit_test.hpp>

#include <string>

BOOST_AUTO_TEST_SUITE(combix)
BOOST_AUTO_TEST_SUITE(parser)

  BOOST_AUTO_TEST_CASE(any) {
    using stream_type = coco::combix::iterator_stream<std::string::const_iterator>;

    auto const src = std::string{"test"};
    auto s = coco::combix::iter_stream(std::begin(src), std::end(src));
    auto const p = coco::combix::any();
    auto const wrapped = coco::combix::parser<char, stream_type>(p);

    BOOST_TEST(parse(wrapped, s).unwrap() == 't');
    BOOST_TEST(parse(wrapped, s).unwrap() == 'e');
    BOOST_TEST(parse(wrapped, s).unwrap() == 's');
    BOOST_TEST(parse(wrapped, s).unwrap() == 't');
    BOOST_TEST(parse(wrapped, s).is_error());
  }

  BOOST_AUTO_TEST_CASE(complicated) {
    using stream_type = coco::combix::iterator_stream<std::string::const_iterator>;

    auto const src = std::string{"a1B"};
    auto s = coco::combix::iter_stream(std::begin(src), std::end(src));
    auto const alpha = coco::combix::parser<char, stream_type>(
        coco::combix::satisfy([](auto&& c) { return 'a' <= c && c <= 'z'; }));
    auto const num = coco::combix::parser<char, stream_type>(
        coco::combix::satisfy([](auto&& c) { return '0' <= c && c <= '9'; }));
    auto const choice = coco::combix::parser<char, stream_type>(
        coco::combix::choice(alpha, num));

    BOOST_TEST(parse(choice, s).unwrap() == 'a');
    BOOST_TEST(parse(choice, s).unwrap() == '1');
    BOOST_TEST(parse(choice, s).is_error());
  }

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
