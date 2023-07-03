///////////////////////////////////////////////////////////////////////////////
/// \file paramstr_texts.h
/// Defines parameterized_string_texts - container for cstring literals
//
//  Copyright 2023 Yariv Tal. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#pragma once

#ifndef __PSTR_PARAMSTR_TEXTS_H
#define __PSTR_PARAMSTR_TEXTS_H

namespace pstr {

template<size_t FORMAT_STRINGS>
class parameterized_string_texts {
	template<size_t Idx, size_t FORMAT_STRINGS2>
	friend const char* const get_text(const parameterized_string_texts<FORMAT_STRINGS2>& a_parameterized) noexcept;
	template<size_t Idx, size_t FORMAT_STRINGS2>
	friend size_t const get_text_len(const parameterized_string_texts<FORMAT_STRINGS2>& a_parameterized) noexcept;

	friend class parameterized_string_texts<FORMAT_STRINGS+1>;

public:
	explicit parameterized_string_texts(const parameterized_string_texts<FORMAT_STRINGS - 1>& a_param, const char* a_fmt, size_t a_len) noexcept {
		std::copy(a_param.m_fmt, a_param.m_fmt + FORMAT_STRINGS - 1, m_fmt);
		m_fmt[FORMAT_STRINGS - 1] = a_fmt;
		std::copy(a_param.m_len, a_param.m_len + FORMAT_STRINGS - 1, m_len);
		m_len[FORMAT_STRINGS - 1] = a_len;
	}

	parameterized_string_texts(const parameterized_string_texts& a_other) noexcept = default;
	parameterized_string_texts(parameterized_string_texts&& a_other) noexcept = default;

	const char* const * const get_texts() const noexcept { return m_fmt; }
	const size_t* const get_text_lengths() const noexcept { return m_len; }
	static constexpr size_t num_texts() noexcept { return FORMAT_STRINGS; }

private:
	const char* m_fmt[FORMAT_STRINGS];
	size_t m_len[FORMAT_STRINGS];
};

template<>
class parameterized_string_texts<1> {
	template<size_t Idx, size_t FORMAT_STRINGS2>
	friend const char* const get_text(const parameterized_string_texts<FORMAT_STRINGS2>& a_parameterized) noexcept;
	template<size_t Idx, size_t FORMAT_STRINGS2>
	friend size_t const get_text_len(const parameterized_string_texts<FORMAT_STRINGS2>& a_parameterized) noexcept;

	friend class parameterized_string_texts<2>;

public:
	explicit parameterized_string_texts(const char* a_fmt, size_t a_len) noexcept {
		m_fmt[0] = a_fmt;
		m_len[0] = a_len;
	}

	parameterized_string_texts(const parameterized_string_texts& a_other) noexcept = default;
	parameterized_string_texts(parameterized_string_texts&& a_other) noexcept = default;

	const char* const * const get_texts() const noexcept { return m_fmt; }
	const size_t* const get_text_lengths() const noexcept { return m_len; }
	static constexpr size_t num_texts() noexcept { return 1; }

private:
	const char* m_fmt[1];
	size_t m_len[1];
};

} // pstr

#endif
