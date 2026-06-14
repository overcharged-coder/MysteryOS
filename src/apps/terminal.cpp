#include "terminal.h"
#include "kernel/kernel.h"
#include "kernel/vfs.h"
#include "fx/glitch.h"
#include "imgui.h"
#include <sstream>
#include <cstring>
#include <cstdlib>

using namespace std;

void Terminal::print(const string& text) {
    lines_.push_back(text);
    scroll_to_bottom_ = true;
}

void Terminal::render(Kernel& k) {
    float h = ImGui::GetContentRegionAvail().y - 32;
    ImGui::BeginChild("##out", {0, h}, false);
    for (auto& line : lines_) {
        string displayed = (Glitch::level() >= 4 && (rand() % 40 == 0)) ? Glitch::mangle(line) : line;
        ImGui::TextUnformatted(displayed.c_str());
    }
    if (Glitch::level() >= 4 && (rand() % 200 == 0)) ImGui::TextColored({0.5f, 0, 0, 1}, "ERR: unauthorized process detected");
    if (scroll_to_bottom_) { ImGui::SetScrollHereY(1.0f); scroll_to_bottom_ = false; }
    ImGui::EndChild();
    ImGui::Separator();
    ImGui::SetNextItemWidth(-60);
    bool run = ImGui::InputText("##cmd", input_, sizeof(input_), ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SameLine();
    run |= ImGui::Button("Run");
    if (run && input_[0] != '\0') {
        print(string("> ") + input_);
        execute(input_, k);
        memset(input_, 0, sizeof(input_));
        ImGui::SetKeyboardFocusHere(-1);
    }
}

void Terminal::execute(const string& raw, Kernel& k) {
    istringstream ss(raw);
    string cmd, arg;
    ss >> cmd >> arg;
    if (cmd == "help") {
        print("Commands: help  ls [path]  cat <file>  unlock <password>  run <app>");
        if (k.puzzle().stage() >= 1)
            print("  [hint] try: cat /System/Archive/classified.txt");
        return;
    }
    if (cmd == "ls") {
        VFSNode* dir = k.vfs().get(arg.empty() ? "/" : arg);
        if (!dir || !dir->is_dir) { print("ls: not a directory: " + arg); return; }
        for (auto& [name, child] : dir->children)
            print("  " + name + (child->is_dir ? "/" : "") + (child->locked ? "  [locked]" : ""));
        return;
    }
    if (cmd == "cat") {
        if (arg.empty()) { print("cat: missing path"); return; }
        VFSNode* node = k.vfs().get(arg);
        if (!node)        { print("cat: not found: " + arg); return; }
        if (node->locked) { print("cat: permission denied"); return; }
        if (node->is_dir) { print("cat: is a directory"); return; }
        print(node->corrupted ? Glitch::mangle(node->content) : node->content);
        return;
    }
    if (cmd == "unlock") {
        if (arg.empty()) { print("unlock: missing password"); return; }
        if (k.puzzle().try_password(arg, k.vfs()))
            print("Access granted. Stage " + to_string(k.puzzle().stage()) + " unlocked.");
        else
            print("unlock: incorrect password.");
        return;
    }
    if (cmd == "run") {
        if (arg.empty()) { print("run: missing app name"); return; }
        k.launch(arg);
        print("Launching " + arg + "...");
        return;
    }
    print("Unknown command: " + cmd + ". Type 'help' for a list.");
}