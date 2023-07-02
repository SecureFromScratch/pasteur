#pragma once

#ifndef __PSTR_EMBED_H
#define __PSTR_EMBED_H

#include <type_traits>

namespace pstr {

template<size_t FORMAT_STRINGS, typename... Args>
struct parameterized_string;

template<typename AddedT>
struct embedded_parameter {
	//embedded_parameter(const AddedT &a_other) : m_embedded(a_other) { }
	embedded_parameter(const AddedT& a_other) : m_embedded(a_other) {}
	embedded_parameter(AddedT&& a_other) : m_embedded(std::forward<AddedT>(a_other)) {}
	embedded_parameter(const embedded_parameter&) = delete;
	auto operator=(const embedded_parameter&) = delete;
	~embedded_parameter() = default;
	AddedT&& get()&& { return m_embedded; }
	const AddedT& get() const& { return m_embedded; }

	operator parameterized_string<1, AddedT>() { return parameterized_string<1, std::remove_reference_t<AddedT>>("", 0, m_embedded); }

	const AddedT m_embedded;
};
template<typename T>
embedded_parameter<std::remove_reference_t<T>> embed(T&& a_val) {
	return embedded_parameter<std::remove_reference_t<T>>{std::forward<T&&>(a_val)};
}

} // pstr

#endif
