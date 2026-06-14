#include "kernel.h"
#include "imgui.h"
#include "fx/glitch.h"
#include "apps/file_explorer.h"
#include "apps/image_viewer.h"
#include "apps/text_editor.h"
#include "apps/terminal.h"
#include "apps/password_dialog.h"
#include <algorithm>

using namespace std;

bool Kernel::init() {
    if (!vfs_.load("/data/filesystem.json")) return false;
    if (!puzzle_.load("/data/puzzles.json")) return false;
    puzzle_.set_callback([this](const StageUnlock&) {
        Glitch::set_level(puzzle_.glitch_level());
    });
    return true;
}

void Kernel::render(){
    Glitch::draw_screen_fx();
    render_taskbar();
    for (auto& w : windows_){
        if (!w.open) continue;
        float offset = (w.id % 6) * 30.0f;
        ImGui::SetNextWindowPos({80.0f + offset, 50.0f + offset}, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize({600, 400}, ImGuiCond_FirstUseEver);
        bool open = true;
        if (ImGui::Begin(w.app->title(), &open)) w.app->render(*this);
        ImGui::End();
        if (!open) w.open = false;
    }
    windows_.erase(remove_if(windows_.begin(), windows_.end(), [](const WinEntry& w){return !w.open;}), windows_.end());
    for (auto& [name, arg] : pending_launches_) {
        auto app = make_app(name, arg);
        if (app) windows_.push_back({next_id_++, true, move(app)});
    }
    pending_launches_.clear();
}

void Kernel::render_taskbar(){
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({ImGui::GetIO().DisplaySize.x, 24});
    ImGui::Begin("##taskbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImGui::Text("MysteryOS");
    ImGui::SameLine();
    if (ImGui::SmallButton("File Explorer")) launch("file_explorer");
    ImGui::SameLine();
    if (ImGui::SmallButton("Terminal")) launch("terminal");
    ImGui::End();
}

unique_ptr<App> Kernel::make_app(const string& name, const string& arg){
    if (name == "file_explorer") return make_unique<FileExplorer>();
    if (name == "text_editor") return make_unique<TextEditor>(arg);
    if (name == "terminal") return make_unique<Terminal>();
    if (name == "password_dialog") return make_unique<PasswordDialog>(arg);
    if (name == "image_viewer") return make_unique<ImageViewer>(arg);
    return nullptr;
}

void Kernel::launch(const string& name, const string& arg){
    pending_launches_.push_back({name, arg});
}
