#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE expected

#include <coco/expected.hpp>

#include <boost/test/unit_test.hpp>

#include <string>

BOOST_AUTO_TEST_SUITE(expected)

BOOST_AUTO_TEST_CASE(creation) {
  coco::expected<int, std::string> exp;
  exp = 1;
  BOOST_ASSERT(exp.is_ok());
  BOOST_ASSERT(static_cast<bool>(exp));
  exp = coco::make_error(std::string{"error message"});
  BOOST_ASSERT(exp.is_err());
  BOOST_ASSERT(!static_cast<bool>(exp));
  exp = std::string{"error message 2"};
  BOOST_ASSERT(exp.is_err());
  BOOST_ASSERT(!static_cast<bool>(exp));

  coco::expected<int, std::string> exp2{"error"};
  BOOST_ASSERT(exp2.is_err());
}

BOOST_AUTO_TEST_CASE(default_value) {
  coco::expected<int, std::string> exp;
  BOOST_ASSERT(exp.is_err());
}

BOOST_AUTO_TEST_CASE(get) {
  coco::expected<int, std::string> exp(1);
  BOOST_TEST(*(exp.get()) == 1);
  *(exp.get()) = 2;
  BOOST_TEST(*(exp.get()) == 2);

  auto const e = exp;
  BOOST_TEST(*(e.get()) == 2);
}

BOOST_AUTO_TEST_CASE(get_exn) {
  coco::expected<int, std::string> exp(1);
  BOOST_TEST(exp.get_exn() == 1);
  exp.get_exn() = 2;
  BOOST_TEST(exp.get_exn() == 2);

  auto const e = exp;
  BOOST_TEST(e.get_exn() == 2);
}

BOOST_AUTO_TEST_CASE(get_err) {
  coco::expected<int, std::string> exp("err");
  BOOST_TEST(*(exp.get_err()) == "err");
  *(exp.get_err()) = "error";
  BOOST_TEST(*(exp.get_err()) == "error");
}

BOOST_AUTO_TEST_CASE(get_err_exn) {
  coco::expected<int, std::string> exp("err");
  BOOST_TEST(exp.get_err_exn() == "err");
  exp.get_err_exn() = "error";
  BOOST_TEST(exp.get_err_exn() == "error");
}

BOOST_AUTO_TEST_CASE(map) {
  auto const e =
      coco::expected<int, std::string>{1}.map([](auto v) { return v + 1; });
  BOOST_TEST(e.get_exn() == 2);

  auto const e2 = coco::expected<int, std::string>{"error"}.map(
      [](auto v) { return v + 1; });
  BOOST_TEST(e2.get_err_exn() == "error");
}

BOOST_AUTO_TEST_CASE(map_err) {
  auto const e = coco::expected<int, std::string>{1}.map_err(
      [](auto v) { return v + "!"; });
  BOOST_TEST(e.get_exn() == 1);

  auto const e2 = coco::expected<int, std::string>{"error"}.map_err(
      [](auto v) { return v + "!"; });
  BOOST_TEST(e2.get_err_exn() == "error!");
}

BOOST_AUTO_TEST_CASE(and_then) {
  auto div = [](auto v) {
    if (v == 0) {
      return coco::expected<int, std::string>("zero division");
    }
    return coco::expected<int, std::string>{v / 2};
  };
  auto const e =
      coco::expected<int, std::string>{4}.and_then(div).and_then(div);
  BOOST_TEST(e.get_exn() == 1);

  BOOST_TEST(std::move(e).and_then(div).and_then(div).get_err_exn() ==
               "zero division");
}

BOOST_AUTO_TEST_CASE(or_) {
  auto const e1 = coco::expected<int, std::string>{0};
  auto const e2 = coco::expected<int, std::string>{"error"};
  auto const e3 = coco::expected<int, double>{coco::make_error(0.5)};

  BOOST_TEST(e1.or_(e3).get_exn() == 0);
  BOOST_TEST(e2.or_(e3).get_err_exn() == 0.5);
}

BOOST_AUTO_TEST_CASE(get_or_else) {
  auto const e1 = coco::expected<int, std::string>{1};
  auto const e2 = coco::expected<int, std::string>{"error"};

  BOOST_TEST(e1.get_or_else(2) == 1);
  BOOST_TEST(e2.get_or_else(2) == 2);
}

BOOST_AUTO_TEST_SUITE_END()
