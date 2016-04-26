#ifndef COCO_COMBIX_LAZY_FUN_HPP_
#define COCO_COMBIX_LAZY_FUN_HPP_

#include <coco/combix/parse_result.hpp>
#include <coco/combix/parser_traits.hpp>

namespace coco {
  namespace combix {

    template <typename F>
    struct func_parser {
      explicit func_parser(F&& f) : f(std::move(f)) {}
      explicit func_parser(F const& f) : f(f) {}

      template <typename Stream>
      parse_result<parse_result_of_t<decltype(std::declval<F>()()), Stream>, Stream>
      operator()(Stream& s) const {
        return parse(f(), s);
      }

      template <typename S>
      expected_list<typename stream_traits<S>::value_type> expected_info()
          const {
        return parser_traits<decltype(std::declval<F>()()), S>::expected_info(
            f());
      }

    private:
      F f;
    };

    template <typename F>
    func_parser<std::decay_t<F>> lazy_fun(F&& f) {
      return func_parser<std::decay_t<F>>(f);
    }

  } // namespace combix
} // namespace coco

#endif
