#pragma once

#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cctype>

struct Hash {
    size_t operator()(std::string_view str) const {
        std::size_t hash = 0;
        for (const auto& c : str) {
            hash += std::hash<char>{}(std::tolower(c));
        }
        return hash;
    }
};

struct IsEqual {
    bool operator()(std::string_view lhs, std::string_view rhs) const {
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                          [](char l, char r) { return std::tolower(l) == std::tolower(r); });
    }
};

using CustomSet = std::unordered_set<std::string_view, Hash, IsEqual>;
using CustomMap = std::unordered_map<std::string_view, double, Hash, IsEqual>;

class SearchEngine {
public:
    void BuildIndex(std::string_view text);
    std::vector<std::string_view> Search(std::string_view query, size_t results_count) const;

private:
    CustomSet SplitQuery(std::string_view query) const;
    class Index {
    public:
        void BuildIndex(std::string_view text);
        CustomMap idf;
        struct Document {
            std::string_view doc_text;
            std::vector<std::string_view> words;
            CustomMap words_tf;
        };
        std::vector<Document> docs;
    };
    Index index_;
};
