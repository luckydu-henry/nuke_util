#pragma once
#include <stdint.h>

#include <format>
#include <string>
#include <string_view>

namespace Hunt {
    namespace Util {
        // LogColor pre-defined 24bit rgb console color
        // All colors default are in hexdecimal form.
        // Actual LogShader support much more color than these.
        enum LogColor : uint32_t {
            eLogColorRed     = 0xff0000, // (255,000,255)
            eLogColorOrange  = 0xffa500, // (255,165,000)
            eLogColorYellow  = 0xffff00, // (255,255,000)
            eLogColorGreen   = 0x00ff00, // (000,255,000)
            eLogColorCyan    = 0x00ffff, // (000,255,255)
            eLogColorBlue    = 0x0000ff, // (000,000,255)
            eLogColorPurple  = 0x800080, // (128,000,128)
            eLogColorPink    = 0xffc0cb, // (255,192,203)
            eLogColorCrimson = 0xdc143c, // (255,020,060)
            eLogColorMagenta = 0xff00ff, // (255,000,255)
            eLogColorBlack   = 0x000000, // (000,000,000)
            eLogColorWhite   = 0xffffff, // (255,255,255)
            eLogColorGray    = 0x808080  // (128,128,128)
        };
        // Determine wheter to use foreground or background color
        enum LogColorStrType : uint8_t { eLogForeG = 3, eLogBackG };
        // Use this function to make color string.
        inline std::string MakeColorString(LogColorStrType logColorStringType, uint32_t hexColor) {
            uint8_t r = hexColor >> 16, g = hexColor >> 8, b = hexColor;
            return std::format("\033[{:d}8;2;{:d};{:d};{:d}m", (uint8_t)logColorStringType, r, g, b);
        }
    }
}