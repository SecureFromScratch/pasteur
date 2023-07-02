#pragma once

#ifndef __PSTR_PARAMSTR_H
#define __PSTR_PARAMSTR_H

#include "paramstr_texts.h"
#include "paramstr_args.h"
#include <tuple>

namespace pstr {

template<size_t FORMAT_STRINGS, typename... Args>
struct parameterized_string;

template<size_t FORMAT_STRINGS, typename HeadT, typename... Args>
class parameterized_string<FORMAT_STRINGS, HeadT, Args...> : public parameterized_string_texts<FORMAT_STRINGS>, public parameterized_string_args<HeadT, Args...> {
public:
	explicit parameterized_string(parameterized_string<FORMAT_STRINGS - 1, HeadT, Args...>&& a_param, const char* a_fmt, size_t a_len)
		: parameterized_string_texts<FORMAT_STRINGS>(std::move(a_param), a_fmt, a_len)
		, parameterized_string_args<HeadT, Args...>(std::move(a_param)) {
	}

	explicit parameterized_string(parameterized_string<FORMAT_STRINGS, Args...>&& a_param, HeadT&& a_additionalParam)
		: parameterized_string_texts<FORMAT_STRINGS>(std::move(a_param))
		, parameterized_string_args<HeadT, Args...>(std::move(a_param), std::forward(a_additionalParam)) {
	}

	explicit parameterized_string(parameterized_string<FORMAT_STRINGS, Args...>&& a_param, const HeadT& a_additionalParam)
		: parameterized_string_texts<FORMAT_STRINGS>(std::move(a_param))
		, parameterized_string_args<HeadT, Args...>(std::move(a_param), a_additionalParam) {
	}

	/*~parameterized_string() {
		std::cerr << "parameterized_string " << FORMAT_STRINGS << "is dead\n";
	}*/
};

template<typename T>
class parameterized_string<1, T> : public parameterized_string_texts<1>, public parameterized_string_args<T> {
public:
	explicit parameterized_string(const char* a_fmt, size_t a_len, T&& a_param) : parameterized_string_texts<1>(a_fmt, a_len), parameterized_string_args<T>(std::move(a_param)) { }
	explicit parameterized_string(const char* a_fmt, size_t a_len, const T& a_param) : parameterized_string_texts<1>(a_fmt, a_len), parameterized_string_args<T>(a_param) { }
};

namespace detail {
struct argskip { };
} // detail

template<>
class parameterized_string<1, detail::argskip> : public parameterized_string_texts<1>, public parameterized_string_args<detail::argskip> {
public:
	template<size_t CSTR_SIZE_INCL_NULL>
	parameterized_string(const char(&a_format)[CSTR_SIZE_INCL_NULL]) : this(a_format, CSTR_SIZE_INCL_NULL - 1) { } // not explicit on purpose

	explicit parameterized_string(const char* a_fmt, size_t a_len) : parameterized_string_texts<1>(a_fmt, a_len), parameterized_string_args<detail::argskip>(detail::argskip{}) { }
};

template<size_t Idx, size_t FORMAT_STRINGS2>
const char* const get_text(const parameterized_string_texts<FORMAT_STRINGS2>& a_parameterized) {
	static_assert(Idx >= 0 && Idx < FORMAT_STRINGS2);
	return a_parameterized.m_fmt[Idx];
}

template<size_t Idx, size_t FORMAT_STRINGS2>
size_t const get_text_len(const parameterized_string_texts<FORMAT_STRINGS2>& a_parameterized) {
	static_assert(Idx >= 0 && Idx < FORMAT_STRINGS2);
	return a_parameterized.m_len[Idx];
}

template<size_t Idx, typename HeadT2, typename... Args2>
const typename std::tuple_element_t<sizeof...(Args2) - Idx /*+1 for HeadT2, -1*/, std::tuple<HeadT2, Args2...>>& get_arg(const parameterized_string_args<HeadT2, Args2...>& a_parameterized) {
	static_assert(Idx >= 0 && Idx < sizeof...(Args2) + 1 /*HeadT2*/);
	return std::get<sizeof...(Args2) + 1 - Idx - 1>(a_parameterized.m_args);
}

/*std::string replaceSubstitutionMarkers(const std::string_view& a_format) {
	std::ostringstream newFormat;
	size_t fromPos = 0;
	size_t idx = 0;
	while (true) {
		const size_t argPos = a_format.find("{}", fromPos);
		if (argPos == std::string::npos) {
			newFormat << a_format.substr(fromPos);
			return newFormat.str();
		}

		std::cout << a_format.substr(fromPos, argPos - fromPos) << " : " << fromPos << "-" << argPos << std::endl;
		newFormat << a_format.substr(fromPos, argPos - fromPos);
		newFormat << " :arg" << idx << " ";
		std::cout << newFormat.str() << std::endl;
		++idx;
		fromPos = argPos + 2;
	}
}
*/

/*template<short Idx, typename SociT, typename TupleT>
struct UseArg {
	static auto useArg(SociT& a_sql, const std::string &a_format, const TupleT &a_tuple) {
		auto useResult = use(std::get<Idx>(a_tuple));
		return UseArg<Idx-1, SociT, TupleT>::useArg(a_sql, a_format, a_tuple), useResult;
	}

};

template<typename SociT, typename TupleT>
struct UseArg<-1, SociT, TupleT> {
	static auto useArg(SociT& a_sql, const std::string& a_format, const TupleT &a_tuple) {
		return a_sql << a_format;
	}
};*/

/*template<typename... Args>
auto operator<<(session& a_sql, Parameterized<Args...> a_formatedArgs) {
	std::string format = replaceSubstitutionMarkers(a_formatedArgs.m_fmt);
	using ArgsTuple = Parameterized<Args...>::ArgsTuple;
	return UseArg<std::tuple_size<ArgsTuple>::value - 1, soci::session, ArgsTuple>::useArg(a_sql, format, a_formatedArgs.m_args);
}

template<typename SociT, typename... Args>
auto operator<<(SociT& a_sql, Parameterized<Args...> a_formatedArgs) {
	std::string format = replaceSubstitutionMarkers(a_formatedArgs.m_fmt);
	using ArgsTuple = Parameterized<Args...>::ArgsTuple;
	return UseArg<std::tuple_size<ArgsTuple>::value - 1, SociT, ArgsTuple>::useArg(a_sql, format, a_formatedArgs.m_args);
}
*/

template<typename RendererT, short ReverseIdx, size_t NUM_FORMAT_STRINGS, typename... Args>
struct parameterized_string_rendering_loop {
	static void iterate(RendererT&& a_renderer, const parameterized_string<NUM_FORMAT_STRINGS, Args...>& a_parameterized) {
		constexpr size_t TEXT_IDX = NUM_FORMAT_STRINGS - ReverseIdx - 1 - (NUM_FORMAT_STRINGS > sizeof...(Args));
		const char* const text = get_text<TEXT_IDX>(a_parameterized);
		const size_t textLen = get_text_len<TEXT_IDX>(a_parameterized);
		a_renderer.render_string(text, textLen);
		constexpr size_t ARG_IDX = sizeof...(Args) - ReverseIdx - 1;
		a_renderer.render_arg(get_arg<ARG_IDX>(a_parameterized));
		parameterized_string_rendering_loop<RendererT, ReverseIdx - 1, NUM_FORMAT_STRINGS, Args...>::iterate(std::forward<RendererT&&>(a_renderer), a_parameterized);
	}
};

template<typename RendererT, size_t NUM_FORMAT_STRINGS, typename... Args>
struct parameterized_string_rendering_loop<RendererT, -1, NUM_FORMAT_STRINGS, Args...> {
	static void iterate(RendererT&& a_renderer, const parameterized_string<NUM_FORMAT_STRINGS, Args...>& a_parameterized) {
		if (NUM_FORMAT_STRINGS > sizeof...(Args)) {
			constexpr size_t TEXT_IDX = NUM_FORMAT_STRINGS - 1;
			a_renderer.render_string(get_text<NUM_FORMAT_STRINGS - 1>(a_parameterized), get_text_len<NUM_FORMAT_STRINGS - 1>(a_parameterized));
		}
	}
};

template<typename RendererT, size_t NUM_FORMAT_STRINGS, typename... Args>
void render_parameterized_string(RendererT&& a_renderer, const parameterized_string<NUM_FORMAT_STRINGS, Args...>& a_parameterized) {
	parameterized_string_rendering_loop<RendererT, sizeof...(Args) - 1, NUM_FORMAT_STRINGS, Args...>::iterate(std::forward<RendererT&&>(a_renderer), a_parameterized);
}

} // pstr

#endif
