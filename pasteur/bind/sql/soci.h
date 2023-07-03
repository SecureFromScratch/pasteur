///////////////////////////////////////////////////////////////////////////////
/// \file soci.h
/// Defines a binding for soci (a sql library) so using parameterized_string with
/// soci automatically converts the string literals into a parameterized sql
/// statement and passes the embedded arguments as parameterized sql query
/// arguments.
/// This helps prevent [CWE-89](https://cwe.mitre.org/data/definitions/89.html) -
///   improperly neutralizing special elements in SQL commands (SQL injection). (ranked #3 in 2022)
//
//  Copyright 2023 Yariv Tal. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#pragma once

#include "parameterized.hpp"

namespace pstr {

namespace soci {

template<short Idx, typename SociT, typename... ArgsT>
struct UseArg {
    static auto useArg(SociT& a_sql, const pstr::parameterized_string_args<ArgsT...> &a_args) {
		auto newSql = UseArg<Idx-1, SociT, ArgsT...>::useArg(a_sql, a_args);
        auto useResult = ::soci::use(pstr::get_arg<Idx>(a_args));
        return (newSql, useResult);
    }

};

template<typename SociT, typename... ArgsT>
struct UseArg<-1, SociT, ArgsT...> {
    static auto useArg(SociT& a_sql, const pstr::parameterized_string_args<ArgsT...> &a_args) {
        return a_sql;
    }
};

std::string generate_args_string(const char * const a_fmt[], const size_t a_len[], const size_t a_numFragments) {
    //size_t expectedSize = std::accumulate(a_len, a_len + a_numFragments, 0ul) + a_numFragments * 6;
    std::stringstream builder;
    for (size_t i = 0 ; i != a_numFragments ; ++i) {
        builder.write(a_fmt[i], a_len[i]);
        builder << (isspace(a_fmt[i][a_len[i] - 1]) ? ":arg" : " :arg");
        builder << i;
        builder << ' ';
    }
    return builder.str();
}

} // soci

} // pstr

template<size_t FORMAT_STRINGS, typename... Args>
auto operator<<(decltype(std::declval<soci::session>().prepare)& a_sql, const pstr::parameterized_string<FORMAT_STRINGS, Args...> &a_formatedArgs) {
    std::string format = pstr::soci::generate_args_string(a_formatedArgs.get_texts(), a_formatedArgs.get_text_lengths(), FORMAT_STRINGS);
    auto newSql = a_sql << format;
    //using ArgsTuple = pstr::parameterized_string<FORMAT_STRINGS, Args...>::ArgsTuple;
    //return UseArg<std::tuple_size<ArgsTuple>::value - 1, decltype(newSql), ArgsTuple>::useArg(newSql, a_formatedArgs.m_args);
    return pstr::soci::UseArg<a_formatedArgs.num_args() - 1, decltype(newSql), Args...>::useArg(newSql, a_formatedArgs);
}

template<size_t FORMAT_STRINGS, typename... Args>
auto operator<<(soci::session& a_sql, const pstr::parameterized_string<FORMAT_STRINGS, Args...> &a_formatedArgs) {
    std::string format = pstr::soci::generate_args_string(a_formatedArgs.get_texts(), a_formatedArgs.get_text_lengths(), FORMAT_STRINGS);
    auto newSql = a_sql.prepare << format;
    //using ArgsTuple = pstr::parameterized_string<FORMAT_STRINGS, Args...>::ArgsTuple;
    //return UseArg<std::tuple_size<ArgsTuple>::value - 1, decltype(newSql), ArgsTuple>::useArg(newSql, a_formatedArgs.m_args);
    return pstr::soci::UseArg<a_formatedArgs.num_args() - 1, decltype(newSql), Args...>::useArg(newSql, a_formatedArgs);
}
