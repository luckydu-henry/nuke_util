///
/// @file      format.hpp
/// @brief     Extension of std::format   
/// @details   ~
/// @author    HenryDu
/// @date      3.09.2023
/// @copyright © HenryDu 2023. All right reserved.
///
#pragma once
#include <format>

namespace std {
    // Since std library didn't provide many features of fmt
    // I will add them my self.
    namespace fmt_detail {
#if defined _MSC_VER
        using fixed_buffer_traits = _Fmt_fixed_buffer_traits;
        using iterator = _Fmt_it;
        using witerator = _Fmt_wit;
        template <typename OutputIt, typename Ty, typename Traits>
        using iterator_buffer = _Fmt_iterator_buffer<OutputIt, Ty, Traits>;
#define BUF_OUT     _Out
#define BUF_COUNT   _Count
#define BUF_IT(buf) fmt_detail::iterator(buf)
#define BUF_WIT(b)  fmt_detail::witerator(b)
#endif
    }
    // Std-library haven't provide vformat_to_n however this is avalible in fmt
    // So I add it myself. different platforms have different implementation so
    // I wrap them in to a fmt_detail namespace so that I can use it withoud difference.
    template <output_iterator<char> OutputIt>
    format_to_n_result<OutputIt> vformat_to_n(OutputIt out, size_t n, std::string_view fmt, std::format_args args) {
        using traits = fmt_detail::fixed_buffer_traits;
        auto buf = fmt_detail::iterator_buffer<OutputIt, char, traits>(out, n);
        vformat_to(BUF_IT(buf), fmt, args);
        return { buf.BUF_OUT(), buf.BUF_COUNT() };
    }
    // Wide char version.
    template <output_iterator<wchar_t> OutputIt>
    format_to_n_result<OutputIt> vformat_to_n(OutputIt out, size_t n, std::wstring_view fmt, std::wformat_args args) {
        using traits = fmt_detail::fixed_buffer_traits;
        auto buf = fmt_detail::iterator_buffer<OutputIt, wchar_t, traits>(out, n);
        vformat_to(BUF_WIT(buf), fmt, args);
        return { buf.BUF_OUT(), buf.BUF_COUNT() };
    }
}