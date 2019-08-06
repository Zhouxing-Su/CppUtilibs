////////////////////////////////
/// usage : 1.	print colorful string in console.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_COLOR_STR_H
#define SZX_CPPUTILIBS_COLOR_STR_H


#include <iostream>
#include <string>


namespace szx {

struct ColorStr {
    enum CmdColor {
        Reset = 0,
        // foreground color.
        BlackFG = 30, RedFG, GreenFG, YellowFG, BlueFG, MagentaFG, CyanFG, WhiteFG,
        BrightBlackFG = 90, BrightRedFG, BrightGreenFG, BrightYellowFG, BrightBlueFG, BrightMagentaFG, BrightCyanFG, BrightWhiteFG,
        // background color.
        BlackBG = 40, RedBG, GreenBG, YellowBG, BlueBG, MagentaBG, CyanBG, WhiteBG,
        BrightBlackBG = 100, BrightRedBG, BrightGreenBG, BrightYellowBG, BrightBlueBG, BrightMagentaBG, BrightCyanBG, BrightWhiteBG
    };


    static std::string get(const std::string &s, CmdColor fcolor, CmdColor bcolor) {
        return colorStr(fcolor) + colorStr(bcolor) + s + "\x1b[0m";
    }
    static std::string get(const std::string &s, CmdColor color) {
        return colorStr(color) + s + "\x1b[0m";
    }

    friend std::ostream& operator<<(std::ostream &os, CmdColor color) {
        return os << "\x1b[" << static_cast<int>(color) << "m";
    }


protected:
    static std::string colorStr(CmdColor color) { return "\x1b[" + std::to_string(color) + "m"; }
};

}


#endif // SZX_CPPUTILIBS_COLOR_STR_H
