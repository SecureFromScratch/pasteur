#pragma once

#include "spdlog/spdlog.h"
#include "parameterized.hpp"

namespace pstr {

template<typename FormatContext>
struct spdlog_parameterized_string_renderer {
	FormatContext& m_ctx;

	spdlog_parameterized_string_renderer(FormatContext& a_ctx) : m_ctx(a_ctx) {}

	void render_string(const char* a_str, size_t a_len) {
        //fmt::format_to(ctx.out(), "({:.1f}, {:.1f})", p.x, p.y)
		//m_strBuilder.write(a_str, a_len);
		fmt::format_to(m_ctx.out(), "{}", std::string_view(a_str, a_len)); 
	}

	struct FilteredOut
	{
		struct Proxy
		{
			decltype(m_ctx.out()) m_out;
			template<class T>
			Proxy& operator=(T a_value)
			{
				*m_out = isprint(static_cast<unsigned char>(a_value)) ? a_value : '?';
				return *this;
			}
		};

		using iterator_category = std::output_iterator_tag;
		using value_type        = typename decltype(m_ctx.out())::value_type;
		using difference_type   = typename decltype(m_ctx.out())::difference_type;
		using pointer           = typename decltype(m_ctx.out())::pointer;
		using reference         = typename decltype(m_ctx.out())::reference;

		decltype(m_ctx.out()) m_out;
		//auto operator++() { m_out.operator++(); return *this; }
		FilteredOut operator++(int) { return FilteredOut{m_out++}; }
		Proxy operator*() { return Proxy{m_out}; }
		//decltype(*m_out) operator*() { return *m_out; }
	};

	template<typename ArgT>
	void render_arg(const ArgT& a_arg) {
		//typename std::iterator_traits<FilteredOut>::iterator_category x;
		//decltype(*std::declval<FilteredOut>() = std::declval<char>()) x2;
		// sanitize arg here
		fmt::format_to(FilteredOut{m_ctx.out()}, "{}", a_arg); 
	}
};

} // pstr

template<size_t NUM_FORMAT_STRINGS, typename... Args>
struct fmt::formatter<pstr::parameterized_string<NUM_FORMAT_STRINGS, Args...>> {
  constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
    auto it = ctx.begin(), end = ctx.end();
    if (it != end) throw format_error("formatting flags not supported");
    return it;
  }

  template <typename FormatContext>
  auto format(const cpp2::parameterized_string<NUM_FORMAT_STRINGS, Args...>& a_parameterized, FormatContext& ctx) const -> decltype(ctx.out()) {
	pstr::spdlog_parameterized_string_renderer<FormatContext> renderer{ctx};
	pstr::render_parameterized_string(renderer, a_parameterized);
	// ctx.out() is an output iterator to write to.
	return ctx.out();
  }
};
