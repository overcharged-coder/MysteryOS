#pragma once
#include <string>
#include <vector>
#include <functional>
#include "vfs.h"

struct InjectedFile{
    std::string path, content;
    bool corrupted = false;
};

struct StageUnlock{
    int stage = 0;
    std::string password;
    std::vector<std::string> unlock_paths;
    std::vector<std::string> unlock_apps;
    int glitch_level = 0;
    std::vector<InjectedFile> inject_files;
};

class PuzzleState{
    public:
        bool load(const std::string& json_path);
        bool try_password(const std::string& input, VFS& vfs);
        int stage() const {return stage_;}
        int glitch_level() const {return glitch_level_;}
        bool is_app_unlocked(const std::string& name) const;
        using Callback = std::function<void(const StageUnlock&)>;
        void set_callback(Callback cb) {callback_ = std::move(cb);}
    private:
        int stage_ = 0;
        int glitch_level_ = 0;
        std::vector<StageUnlock> stages_;
        std::vector<std::string> unlocked_apps_;
        Callback callback_;
        void apply(const StageUnlock& su, VFS& vfs);
};
