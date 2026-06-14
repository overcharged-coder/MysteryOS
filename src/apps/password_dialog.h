#pragma once
#include "kernel/app.h"
#include <string>
class PasswordDialog : public App {
    public:
        explicit PasswordDialog(const std::string& target_path);
        void render(Kernel& k) override;
        const char* title() const override {return "Enter Password";}
    private:
        std::string target_path_;
        char input_[64] = {};
        int fail_count_ = 0;
        float shake_timer_ = 0.0f;
        std::string message_;
};
