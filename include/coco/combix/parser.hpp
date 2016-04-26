#ifndef COCO_COMBIX_PARSER_HPP_
#define COCO_COMBIX_PARSER_HPP_

#include <memory>

#include <coco/combix/parse_result.hpp>

namespace coco {
  namespace combix {

    template <typename Result, typename Stream>
    struct parser {
      template <typename P>
      parser(P const& p) : holder_(std::make_unique<holder<P>>(p)) {}

      template <typename P>
      parser(P&& p) : holder_(std::make_unique<holder<P>>(std::move(p))) {}

      parser(parser const& p) : holder_(p.holder_->copy()) {}
      parser(parser&& p) : holder_(std::move(p.holder_)) {}

      parser& operator=(parser const& p) {
        holder_ = p.holder_->copy();
        return *this;
      }
      parser& operator=(parser&& p) {
        holder_ = std::move(p.holder_);
        return *this;
      }

      ~parser() = default;

      parse_result<Result, Stream> operator()(Stream& s) const {
        return (*holder_)(s);
      }

    private:
      struct holder_base {
        virtual parse_result<Result, Stream> operator()(Stream&) const = 0;
        virtual ~holder_base() = default;

        virtual std::unique_ptr<holder_base> copy() const = 0;
      };

      template <typename P>
      struct holder : holder_base {
        holder(P const& p) : parser(p) {}
        holder(P&& p) : parser(std::move(p)) {}

        ~holder() = default;

        parse_result<Result, Stream> operator()(Stream& s) const override {
          return parser(s);
        }

        std::unique_ptr<holder_base> copy() const override {
          return std::make_unique<holder>(parser);
        }
      private:
        P parser;
      };

      std::unique_ptr<holder_base> holder_;
    };

  } // namespace combix
} // namespace coco

#endif
