#pragma once
#include <string>

using namespace std;

namespace Glitch{
    void set_level(int level);
    int level();
    string mangle(const string& text);
    void draw_screen_fx();
}
