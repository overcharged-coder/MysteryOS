#pragma once
#include "kernel/app.h"
#include "fx/glitch.h"
#include <string>
class TextEditor : public App {
public:
    explicit TextEditor(const std::string& path);
    void render(Kernel& k) override;
    const char* title() const override;
private:
    std::string path_, title_;
    Glitch::TextCorruptor corruptor_;
    bool counted_ = false;
};
