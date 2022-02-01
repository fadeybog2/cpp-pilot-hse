#pragma once

#include <string>
#include <string_view>
#include <vector>

class UnixPath {
public:
    UnixPath(std::string_view initial_dir);

    void ChangeDirectory(std::string_view path);

    std::string GetAbsolutePath() const;
    std::string GetRelativePath() const;

private:
    std::vector<std::string_view> init_path_;
    std::vector<std::string_view> cur_path_;
    void SplitBySlash(std::string_view str, std::vector<std::string_view>& result);
    void Normalize(std::vector<std::string_view>& dirs) const;
};
