/*
 * Copyright (c) 2020 rxi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "Utlt/log.hpp"
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>

namespace CRS {
    namespace Utlt {
        // Global output file.
        static std::fstream s_fstream{g_log_file_path, std::ios::out};
        // Static instance initialize.
        dbt_logger dbt_logger::s_csinst{std::clog};
        dbt_logger dbt_logger::s_fsinst{s_fstream};

        std::mutex dbt_logger::s_mutex{};

        dbt_logger::dbt_logger(std::ostream& stream) : m_stream(stream) {}

        dbt_logger& dbt_logger::get(dbt_logger::logger_t lt) {
            switch (lt) {
                case dbt_logger::console: return dbt_logger::s_csinst;
                case dbt_logger::file:    return dbt_logger::s_fsinst;
            }
        }
        void dbt_logger::vlog(level_t lv, color_pack cl, std::string_view fmt, std::format_args args) {
            auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
            auto const id = std::this_thread::get_id();

            std::lock_guard<std::mutex> lkgd(s_mutex);
            cl.shade_up();
            m_stream << std::format(g_fmt_string, time, g_level_name[(int)lv]) 
                     << "TID: " << std::setw(6) << std::setfill('0') << id << ": "; // std::thread::id not supported by std yet.
            m_stream << std::vformat(fmt, args) << std::endl;
            cl.shade_dn();
        }
    }
}