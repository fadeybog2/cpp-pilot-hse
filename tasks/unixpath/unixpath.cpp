#include "unixpath.h"

#include <vector>

void RemoveLastSlash(std::string &str) {
    if (str.ends_with("/") && str.size() > 1) {
        str.erase(str.size() - 1, 1);
    }
}

void SplitBySlash(std::string_view &str, std::vector<std::string_view> &result) {
    while (!str.empty()) {
        size_t sl_idx = str.find("/", 1);
        if (sl_idx == std::string_view::npos) {
            if (str.starts_with("/")) {
                result.push_back(str.substr(1));
            } else {
                result.push_back(str.substr(0));
            }
            break;
        } else {
            if (str.starts_with("/")) {
                result.push_back(str.substr(1, sl_idx - 1));
            } else {
                result.push_back(str.substr(0, sl_idx));
            }
        }
        str.remove_prefix(sl_idx);
    }
}

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    std::string answer("/");
    std::vector<std::string_view> dirs;
    if (path.starts_with("/")) {
        SplitBySlash(path, dirs);
    } else {
        SplitBySlash(current_working_dir, dirs);
        SplitBySlash(path, dirs);
    }

    for (size_t i = 0; i < dirs.size();) {
        if (dirs[i] == "." || dirs[i].empty()) {
            dirs.erase(dirs.begin() + i);
        } else if (dirs[i] == "..") {
            if (i == 0) {
                dirs.erase(dirs.begin(), dirs.begin() + 1);
            } else {
                dirs.erase(dirs.begin() + i - 1, dirs.begin() + i + 1);
                i--;
            }
        } else {
            i++;
        }
    }
    for (const auto &dir : dirs) {
        answer.append(dir);
        answer += "/";
    }
    RemoveLastSlash(answer);
    return answer;
}
