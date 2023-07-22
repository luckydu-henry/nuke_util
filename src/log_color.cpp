#include "Utlt/log.hpp"
#if defined _WIN32
#    include <windows.h>
#endif
namespace CRS {
    namespace Utlt {
#if defined _WIN32
        unsigned char color_info[8] = { 0x0,0x4,0x2,0x6,0x1,0xd,0xb,0x7 };
        void color_pack::shade_up() {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_info[(int)fg] | color_info[(int)bg]);
        }
        void color_pack::shade_dn() {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), default_fg_color | default_bg_color);
        }
#endif
    }
}