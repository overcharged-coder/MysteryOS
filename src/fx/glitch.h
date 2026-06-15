#pragma once
#include <string>

using namespace std;

namespace Glitch{
    class TextCorruptor {
        public:
            explicit TextCorruptor(float refresh_seconds = 0.137f);
            const string& render(const string& source, float now_seconds);
        private:
            float refresh_seconds_ = 0.137f;
            float next_refresh_ = -1.0f;
            string source_;
            string rendered_;
    };

    void set_level(int level);
    int level();
    string mangle(const string& text);
    void draw_screen_fx();
}
