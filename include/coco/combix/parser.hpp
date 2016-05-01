#ifndef COCO_COMBIX_PARSER_HPP_
#define COCO_COMBIX_PARSER_HPP_

#include <memory>
#include <type_traits>

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

      parse_result<Result, Stream> parse(Stream& s) const {
        return holder_->parse(s);
      }

      template <typename S>
      typename std::enable_if<std::is_same<S, Stream>::value, void>::type
      add_error(parse_error<S>& err) const {
        return holder_->add_error(err);
      }

     private:
      struct holder_base {
        virtual ~holder_base() = default;

        virtual parse_result<Result, Stream> parse(Stream&) const = 0;
        virtual void add_error(parse_error<Stream>&) const = 0;

        virtual std::unique_ptr<holder_base> copy() const = 0;
      };

      template <typename P>
      struct holder : holder_base {
        holder(P const& p) : parser(p) {}
        holder(P&& p) : parser(std::move(p)) {}

        ~holder() = default;

        parse_result<Result, Stream> parse(Stream& s) const override {
          return parser.parse(s);
        }

        void add_error(parse_error<Stream>& err) const override {
          return parser.template add_error<Stream>(err);
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
