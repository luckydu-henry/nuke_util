#pragma once
#include <cstdint>
#include <string_view>

namespace nkutil {
    // console_colors pre-defined 24bit rgb console color
    // All colors default are in hexdecimal form.
    // Actual LogShader support much more color than these.
    enum hex_colors : uint32_t {
        hex_red     = 0xff0000, // (255,000,255)
        hex_orange  = 0xffa500, // (255,165,000)
        hex_yellow  = 0xffff00, // (255,255,000)
        hex_green   = 0x00ff00, // (000,255,000)
        hex_cyan    = 0x00ffff, // (000,255,255)
        hex_blue    = 0x0000ff, // (000,000,255)
        hex_purple  = 0x800080, // (128,000,128)
        hex_pink    = 0xffc0cb, // (255,192,203)
        hex_crimson = 0xdc143c, // (255,020,060)
        hex_magenta = 0xff00ff, // (255,000,255)
        hex_black   = 0x000000, // (000,000,000)
        hex_white   = 0xffffff, // (255,255,255)
        hex_gray    = 0x808080  // (128,128,128)
    };
    // Determine wheter to use foreground or background color
    enum console_color_usage : uint8_t { console_fg = 3, console_bg };
    std::string make_ansi_console_color(console_color_usage cug, uint32_t c);
    std::string make_ansi_console_color();
}