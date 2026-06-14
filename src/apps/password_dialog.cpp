#include "password_dialog.h"
#include "kernel/kernel.h"
#include "imgui.h"
#include <cmath>
#include <cstring>

using namespace std;

PasswordDialog::PasswordDialog(const string& path) : target_path_(path) {}

void PasswordDialog::render(Kernel& k){
    if (shake_timer_ > 0.0f){
        shake_timer_ -= ImGui::GetIO().DeltaTime;
        float offset = sinf(shake_timer_ * 40.0f) * 4.0f;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);
    }
    ImGui::TextWrapped("This item is locked.\nEnter the password:");
    ImGui::Spacing();
    ImGui::SetNextItemWidth(200);
    bool submitted = ImGui::InputText("##pw", input_, sizeof(input_), ImGuiInputTextFlags_Password | ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SameLine();
    submitted |= ImGui::Button("Unlock");
    if (submitted && input_[0] != '\0'){
        if (k.puzzle().try_password(input_, k.vfs())) message_ = "Unlocked!";
        else{
            fail_count_++;
            shake_timer_ = 0.4f;
            message_ = "Wrong password. (" + to_string(fail_count_) + " attempt" + (fail_count_ == 1 ? "" : "s") + ")";
        }
        memset(input_, 0, sizeof(input_));
    }
    if (!message_.empty()){
        ImGui::Spacing();
        bool success = (message_[0] == 'U');
        ImGui::TextColored(success ? ImVec4(0.3f, 1, 0.3f, 1) : ImVec4(1, 0.3f, 0.3f, 1), "%s", message_.c_str());
    }
}
