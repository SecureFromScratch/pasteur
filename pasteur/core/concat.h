#pragma once

#ifndef __CONCAT_H
#define __CONCAT_H

#include "paramstr.h"
#include "embed.h"

namespace pstr {

template<size_t FORMAT_STRINGS, size_t CSTR_SIZE_INCL_NULL, typename HeadT, typename... Args>
auto operator+(parameterized_string<FORMAT_STRINGS, HeadT, Args...>&& a_param, const char(&a_format)[CSTR_SIZE_INCL_NULL]) {
	//std::cout << "extent: " << std::extent<CharArrayT>::value << std::endl;
	return parameterized_string<FORMAT_STRINGS + 1, HeadT, Args...>( /*std::forward(*/ std::move(a_param), a_format, CSTR_SIZE_INCL_NULL - 1);
}

template<size_t FORMAT_STRINGS, typename AddedT, typename... Args>
auto operator+(parameterized_string<FORMAT_STRINGS, Args...>&& a_param, embedded_parameter<AddedT>&& a_additionalParam) {
	//std::cout << "extent: " << std::extent<CharArrayT>::value << std::endl;
	return parameterized_string<FORMAT_STRINGS, std::remove_reference_t<AddedT>, Args...>( /*std::forward(*/ std::move(a_param), std::move(a_additionalParam.get()));
}

template<size_t FORMAT_STRINGS, typename AddedT, typename... Args>
auto operator+(parameterized_string<FORMAT_STRINGS, Args...>&& a_param, const embedded_parameter<AddedT>& a_additionalParam) {
	//std::cout << "extent: " << std::extent<CharArrayT>::value << std::endl;
	return parameterized_string<FORMAT_STRINGS, std::remove_reference_t<AddedT>, Args...>( /*std::forward(*/ std::move(a_param), a_additionalParam.get());
}

template<size_t FORMAT_STRINGS, typename AddedT, typename... Args>
auto operator+(parameterized_string<FORMAT_STRINGS, Args...>&& a_param, AddedT&& a_additionalParam) {
	//std::cout << "extent: " << std::extent<CharArrayT>::value << std::endl;
	return parameterized_string<FORMAT_STRINGS, std::remove_reference_t<AddedT>, Args...>( /*std::forward(*/ std::move(a_param), std::move(a_additionalParam));
}

template<size_t FORMAT_STRINGS, typename AddedT, typename... Args>
auto operator+(parameterized_string<FORMAT_STRINGS, Args...>&& a_param, const AddedT& a_additionalParam) {
	//std::cout << "extent: " << std::extent<CharArrayT>::value << std::endl;
	return parameterized_string<FORMAT_STRINGS, std::remove_reference_t<AddedT>, Args...>( /*std::forward(*/ std::move(a_param), a_additionalParam);
}

template<size_t CSTR_SIZE_INCL_NULL, typename AddedT>//, std::enable_if<std::is_array<CharArrayT>::value, bool> = true>
auto operator+(const char(&a_format)[CSTR_SIZE_INCL_NULL], const embedded_parameter<AddedT>& a_param) {
	//std::cout << "extent: " << std::extent<CharArrayT>::value << std::endl;
	return parameterized_string<1, std::remove_reference_t<AddedT>>(a_format, CSTR_SIZE_INCL_NULL - 1, a_param.get());
}

template<size_t CSTR_SIZE_INCL_NULL, typename AddedT>//, std::enable_if<std::is_array<CharArrayT>::value, bool> = true>
auto operator+(const embedded_parameter<AddedT>& a_param, const char(&a_format)[CSTR_SIZE_INCL_NULL]) {
	//std::cout << "extent: " << std::extent<CharArrayT>::value << std::endl;
	return parameterized_string<1, std::remove_reference_t<AddedT>>("", 0, a_param.get()) + a_format;
}

} // pstr

#endif
