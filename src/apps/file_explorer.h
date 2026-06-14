#pragma once
#include "kernel/app.h"
#include "kernel/vfs.h"
#include <string>
class FileExplorer : public App{
    public:
        void render(Kernel& k) override;
        const char* title() const override {return "File Explorer";}
    private:
        std::string selected_dir_ = "/";
        void render_tree(Kernel& k, VFSNode* node, const std::string& path);
        void render_contents(Kernel& k, VFSNode* dir, const std::string& path);
};