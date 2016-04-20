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
  BOOST_ASSERT(exp.is_error());
  BOOST_ASSERT(!static_cast<bool>(exp));
  exp = std::string{"error message 2"};
  BOOST_ASSERT(exp.is_error());
  BOOST_ASSERT(!static_cast<bool>(exp));

  coco::expected<int, std::string> exp2{"error"};
  BOOST_ASSERT(exp2.is_error());
}

BOOST_AUTO_TEST_CASE(default_value) {
  coco::expected<int, std::string> exp;
  BOOST_ASSERT(exp.is_error());
}

BOOST_AUTO_TEST_CASE(ok) {
  coco::expected<int, std::string> exp(1);
  BOOST_TEST(*(exp.ok()) == 1);
  *(exp.ok()) = 2;
  BOOST_TEST(*(exp.ok()) == 2);

  auto const e = exp;
  BOOST_TEST(*(e.ok()) == 2);
}

BOOST_AUTO_TEST_CASE(unwrap) {
  coco::expected<int, std::string> exp(1);
  BOOST_TEST(exp.unwrap() == 1);
  exp.unwrap() = 2;
  BOOST_TEST(exp.unwrap() == 2);

  auto const e = exp;
  BOOST_TEST(e.unwrap() == 2);
}

BOOST_AUTO_TEST_CASE(error) {
  coco::expected<int, std::string> exp("err");
  BOOST_TEST(*(exp.error()) == "err");
  *(exp.error()) = "error";
  BOOST_TEST(*(exp.error()) == "error");
}

BOOST_AUTO_TEST_CASE(unwrap_error) {
  coco::expected<int, std::string> exp("err");
  BOOST_TEST(exp.unwrap_error() == "err");
  exp.unwrap_error() = "error";
  BOOST_TEST(exp.unwrap_error() == "error");
}

BOOST_AUTO_TEST_CASE(map) {
  auto const e =
      coco::expected<int, std::string>{1}.map([](auto v) { return v + 1; });
  BOOST_TEST(e.unwrap() == 2);

  auto const e2 = coco::expected<int, std::string>{"error"}.map(
      [](auto v) { return v + 1; });
  BOOST_TEST(e2.unwrap_error() == "error");
}

BOOST_AUTO_TEST_CASE(map_err) {
  auto const e = coco::expected<int, std::string>{1}.map_err(
      [](auto v) { return v + "!"; });
  BOOST_TEST(e.unwrap() == 1);

  auto const e2 = coco::expected<int, std::string>{"error"}.map_err(
      [](auto v) { return v + "!"; });
  BOOST_TEST(e2.unwrap_error() == "error!");
}

BOOST_AUTO_TEST_CASE(flat_map) {
  auto div = [](auto v) {
    if (v == 0) {
      return coco::expected<int, std::string>("zero division");
    }
    return coco::expected<int, std::string>{v / 2};
  };
  auto const e =
      coco::expected<int, std::string>{4}.flat_map(div).flat_map(div);
  BOOST_TEST(e.unwrap() == 1);

  BOOST_TEST(std::move(e).flat_map(div).flat_map(div).unwrap_error() ==
               "zero division");
}

BOOST_AUTO_TEST_CASE(or_) {
  auto const e1 = coco::expected<int, std::string>{0};
  auto const e2 = coco::expected<int, std::string>{"error"};
  auto const e3 = coco::expected<int, double>{coco::make_error(0.5)};

  BOOST_TEST(e1.or_(e3).unwrap() == 0);
  BOOST_TEST(e2.or_(e3).unwrap_error() == 0.5);
}

BOOST_AUTO_TEST_CASE(unwrap_or) {
  auto const e1 = coco::expected<int, std::string>{1};
  auto const e2 = coco::expected<int, std::string>{"error"};

  BOOST_TEST(e1.unwrap_or(2) == 1);
  BOOST_TEST(e2.unwrap_or(2) == 2);
}

BOOST_AUTO_TEST_CASE(operator_bitor) {
  auto const e1 = coco::expected<int, std::string>{0};
  auto const e2 = coco::expected<int, std::string>{"error"};
  auto const e3 = coco::expected<int, double>{coco::make_error(0.5)};

  BOOST_TEST((e1 | e2).unwrap() == 0);
  BOOST_TEST((e2 | e3).unwrap_error() == 0.5);
  BOOST_TEST((e2 | e1 | e3).unwrap() == 0);
}

BOOST_AUTO_TEST_CASE(operator_flat_map) {
  auto div = [](auto v) {
    if (v == 0) {
      return coco::expected<int, std::string>("zero division");
    }
    return coco::expected<int, std::string>{v / 2};
  };
  auto const e = coco::expected<int, std::string>{4} >> div >> div;
  BOOST_TEST(e.unwrap() == 1);

  BOOST_TEST((std::move(e) >> div >> div).unwrap_error() == "zero division");
}

BOOST_AUTO_TEST_CASE(operator_star) {
  auto e = coco::expected<int, std::string>{0};
  BOOST_TEST(*e == 0);
  *e = 1;
  BOOST_TEST(*e == 1);
}

BOOST_AUTO_TEST_SUITE_END()
