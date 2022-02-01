#include "search.h"

#include <cctype>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <functional>

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

struct RelevString {
    std::string_view str;
    std::vector<std::string_view> words;
    std::unordered_map<std::string_view, double, Hash, IsEqual> tf;
    double relevance = 0;
};

void SplitText(const std::string_view& text, std::vector<RelevString>& result) {
    size_t beg = 0;
    size_t str_beg = 0;
    bool is_word = std::isalpha(text[beg]);
    for (size_t end = 0; end < text.size(); ++end) {
        if (end == text.size() - 1) {
            if (is_word) {
                result.back().words.push_back(text.substr(beg, end - beg));
                is_word = false;
                beg = end;
            }
            result.back().str = text.substr(str_beg, end - str_beg + 1);
        } else if (std::isalpha(text[end])) {
            if (!is_word) {
                beg = end;
                is_word = true;
            }
        } else {
            if (is_word) {
                result.back().words.push_back(text.substr(beg, end - beg));
                is_word = false;
                beg = end;
            }
        }
        if (text[end] == '\n') {
            result.back().str = text.substr(str_beg, end - str_beg);
            str_beg = end + 1;
            result.push_back(RelevString());
        }
    }
}

void SplitQuery(const std::string_view& query, std::vector<std::string_view>& words) {
    size_t beg = 0;
    bool is_word = std::isalpha(query[beg]);
    for (size_t end = 0; end < query.size(); ++end) {
        if (end == query.size() - 1) {
            if (is_word) {
                if (std::isalpha(query[end])) {
                    end++;
                }
                words.push_back(query.substr(beg, end - beg));
            }
        } else if (std::isalpha(query[end])) {
            if (!is_word) {
                is_word = true;
                beg = end;
            }
        } else {
            if (is_word) {
                words.push_back(query.substr(beg, end - beg));
                is_word = false;
                beg = end;
            }
        }
    }
}

bool Comparator(const RelevString& lhs, const RelevString& rhs) {
    return lhs.relevance > rhs.relevance;
}

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    std::vector<std::string_view> answer;
    std::vector<std::string_view> splitted_query;
    std::vector<RelevString> rel_strs(1);
    SplitText(text, rel_strs);
    SplitQuery(query, splitted_query);

    size_t not_empty_strings_number = rel_strs.size();
    for (const auto& rel_str : rel_strs) {
        if (rel_str.words.empty()) {
            not_empty_strings_number--;
        }
    }

    std::unordered_map<std::string_view, double, Hash, IsEqual> idf;
    std::unordered_set<std::string_view, Hash, IsEqual> words_in_query(splitted_query.begin(), splitted_query.end());
    for (auto& rel_str : rel_strs) {
        for (const auto& word : rel_str.words) {
            if (words_in_query.contains(word)) {
                rel_str.tf[word] += 1.0 / rel_str.words.size();
            }
        }
    }

    for (auto& rel_str : rel_strs) {
        std::unordered_set<std::string_view, Hash, IsEqual> words_set(rel_str.words.begin(), rel_str.words.end());
        for (const auto& word : words_in_query) {
            if (words_set.contains(word)) {
                idf[word] += 1.0 / not_empty_strings_number;
            }
        }
    }
    for (auto& rel_str : rel_strs) {
        for (const auto& [word, tf] : rel_str.tf) {
            if (idf[word] != 0) {
                rel_str.relevance += tf * std::log(1.0 / idf[word]);
            }
        }
    }
    std::sort(rel_strs.begin(), rel_strs.end(), Comparator);
    for (size_t i = 0; i < results_count && rel_strs[i].relevance > 0; ++i) {
        answer.push_back(rel_strs[i].str);
    }
    return answer;
}
