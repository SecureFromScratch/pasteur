#pragma once

#ifndef __PSTR_PARAMSTR_ARGS_H
#define __PSTR_PARAMSTR_ARGS_H

namespace pstr {

template<typename... Args>
struct parameterized_string_args;

template<typename HeadT, typename... Args>
class parameterized_string_args<HeadT, Args...> {
	template<size_t Idx, typename HeadT2, typename... Args2>
	friend const typename std::tuple_element_t<sizeof...(Args2) - Idx /*+1 for HeadT2, -1*/, std::tuple<HeadT2, Args2...>>& get_arg(const parameterized_string_args<HeadT2, Args2...>& a_parameterized);

	using ArgsTuple = std::tuple<HeadT, Args...>;

	template<typename... ArgsT2>
	friend class parameterized_string_args;

public:
	explicit parameterized_string_args(parameterized_string_args<Args...>&& a_param, HeadT&& a_additionalParam)
		: m_args(std::tuple_cat(std::tuple<HeadT>{std::move(a_additionalParam)}, std::move(a_param.m_args))) {
	}
	explicit parameterized_string_args(parameterized_string_args<Args...>&& a_param, const HeadT& a_additionalParam)
		: m_args(std::tuple_cat(std::tuple<HeadT>{a_additionalParam}, std::move(a_param.m_args))) {
	}

	parameterized_string_args(const parameterized_string_args& a_other) = default;
	parameterized_string_args(parameterized_string_args&& a_other) = default;

	static constexpr size_t num_args() { return std::tuple_size_v<ArgsTuple>; }

private:
	const ArgsTuple m_args;
};

template<typename T>
class parameterized_string_args<T> {
	template<size_t Idx, typename HeadT2, typename... Args2>
	friend const typename std::tuple_element_t<sizeof...(Args2) - Idx /*+1 for HeadT2, -1*/, std::tuple<HeadT2, Args2...>>& get_arg(const parameterized_string_args<HeadT2, Args2...>& a_parameterized);

	template<typename... ArgsT2>
	friend class parameterized_string_args;

public:
	explicit parameterized_string_args(T&& a_param) : m_args(std::move(a_param)) { }
	explicit parameterized_string_args(const T& a_param) : m_args(a_param) { }
	parameterized_string_args(const parameterized_string_args& a_other) = default;
	parameterized_string_args(parameterized_string_args&& a_other) = default;

	static constexpr size_t num_args() { return 1; }

private:
	const std::tuple<T> m_args;
};

} // pstr

#endif
