#pragma once

#ifndef __CONCAT_HEAD_H
#define __CONCAT_HEAD_H

#include "concat.h"


namespace pstr {

namespace detail {
struct concat_head_t { 
	template<size_t CSTR_SIZE_INCL_NULL>//, std::enable_if<std::is_array<CharArrayT>::value, bool> = true>
	auto operator/(const char(&a_format)[CSTR_SIZE_INCL_NULL]) const {
		return parameterized_string<1, detail::argskip>(a_format, CSTR_SIZE_INCL_NULL - 1, detail::argskip());
	}
	template<typename AddedT>//, std::enable_if<std::is_array<CharArrayT>::value, bool> = true>
	auto operator/(AddedT&& a_param) const {
		return parameterized_string<1, std::remove_reference_t<AddedT>>("", 0, std::forward(a_param));
	}
	template<typename AddedT>//, std::enable_if<std::is_array<CharArrayT>::value, bool> = true>
	auto operator/(const AddedT& a_param) const {
		return parameterized_string<1, std::remove_reference_t<AddedT>>("", 0, a_param);
	}
};
} // detail
inline detail::concat_head_t concat;

} // pstr

#endif
