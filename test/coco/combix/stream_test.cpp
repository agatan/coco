#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE expected

#include <coco/combix/iterator_stream.hpp>
#include <coco/combix/stream_trait.hpp>

#include <boost/test/unit_test.hpp>

#include <string>

BOOST_AUTO_TEST_SUITE(combix)
BOOST_AUTO_TEST_SUITE(stream)

  BOOST_AUTO_TEST_CASE(iterator_stream) {
    std::string src{"123"};
    auto stream = coco::combix::iter_stream(std::begin(src), std::end(src));

    BOOST_TEST(stream.uncons().unwrap() == '1');
    BOOST_TEST(stream.uncons().unwrap() == '2');
    auto saved = stream.save();
    BOOST_TEST(stream.uncons().unwrap() == '3');
    BOOST_TEST(stream.uncons().is_error());
    BOOST_TEST(saved.uncons().unwrap() == '3');
  }

  BOOST_AUTO_TEST_CASE(stream_trait) {
    std::string src{"123"};
    auto stream = coco::combix::iter_stream(std::begin(src), std::end(src));

    BOOST_TEST(coco::combix::uncons(stream).unwrap() == '1');
    BOOST_TEST(coco::combix::uncons(stream).unwrap() == '2');

    auto saved = coco::combix::save(stream);
    BOOST_TEST(coco::combix::uncons(stream).unwrap() == '3');
    BOOST_TEST(coco::combix::uncons(saved).unwrap() == '3');

    static_assert(coco::combix::is_stream_trait_v<decltype(saved)>,
                  "iteratr stream should implement as stream trait");
    static_assert(!coco::combix::is_stream_trait_v<int>,
                  "int type should not be stream trait type");
  }

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
