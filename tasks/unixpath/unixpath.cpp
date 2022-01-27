#include "unixpath.h"

void ReplaceDoubleSlash(std::string &str) {
    while (str.find("//") != std::string::npos) {
        str.replace(str.find("//"), 2, "/");
    }
}

void RemoveLastSlash(std::string &str) {
    if (str.ends_with("/") && str.size() > 1) {
        str.erase(str.size() - 1, 1);
    }
}

void AddLastSlash(std::string &str) {
    if (!str.ends_with("/")) {
        str.append("/");
    }
}
std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    std::string norm_dir{current_working_dir};
    std::string norm_path{path};

    ReplaceDoubleSlash(norm_dir);
    ReplaceDoubleSlash(norm_path);
    RemoveLastSlash(norm_dir);
    RemoveLastSlash(norm_path);
    AddLastSlash(norm_path);
    AddLastSlash(norm_dir);

    std::string answer;
    if (norm_path.starts_with("/")) {
        answer = norm_path;
    } else {
        answer = norm_dir + norm_path;
    }
    for (size_t i = 1; i < answer.size();) {
        if (answer.substr(i).starts_with("../")) {
            if (answer.find_last_of("/", i) == 0) {
                answer.erase(0, 3);
            } else {
                size_t idx = answer.find_last_of("/", answer.find_last_of("/", i) - 1);
                answer.erase(idx, 2 + i - idx);
                i = idx;
            }
        } else if (answer.substr(i).starts_with("./")) {
            answer.erase(i, 2);
        } else {
            i = answer.find("/", i) + 1;
        }
    }
    RemoveLastSlash(answer);
    return answer;
}
