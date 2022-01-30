#include "search.h"

#include <cctype>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <string>

struct RelevString {
    std::string_view str;
    std::vector<std::string_view> words;
    std::unordered_map<std::string_view, double> tf;
    double relevance = 0;
};

void SplitText(const std::string_view& text, const std::string_view& lower_text, std::vector<RelevString>& result) {
    size_t beg = 0;
    size_t str_beg = 0;
    bool is_word = std::isalpha(text[beg]);
    for (size_t end = 0; end < text.size(); ++end) {
        if (end == text.size() - 1) {
            if (is_word) {
                result.back().words.push_back(lower_text.substr(beg, end - beg + 1));
                is_word = false;
                beg = end;
            }
        } else if (std::isalpha(text[end])) {
            if (!is_word) {
                beg = end;
                is_word = true;
            }
        } else {
            if (is_word) {
                result.back().words.push_back(lower_text.substr(beg, end - beg));
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
                words.push_back(query.substr(beg, end - beg + 1));
                is_word = false;
                beg = end;
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
    std::vector<std::string_view> splitted_query(1);
    std::vector<RelevString> rel_strs(1);
    std::string lower_text;
    std::string lower_query;
    for (const auto& c : text) {
        lower_text.push_back(tolower(c));
    }
    for (const auto& c : query) {
        lower_query.push_back(tolower(c));
    }
    SplitText(text, lower_text, rel_strs);
    SplitQuery(lower_query, splitted_query);

    size_t not_empty_strings_number = rel_strs.size();
    for (size_t i = 0; i < rel_strs.size(); ++i) {
        if (rel_strs[i].words.empty()) {
            not_empty_strings_number--;
        }
    }

    std::unordered_map<std::string_view, double> idf;
    std::set<std::string_view> words_in_query(splitted_query.begin(), splitted_query.end());
    for (const auto& str : rel_strs) {
        for (const auto& word : str.words) {
            if (words_in_query.contains(word)) {
                idf[word] += 1.0 / not_empty_strings_number;
            }
        }
    }

    for (size_t i = 0; i < rel_strs.size(); ++i) {
        for (const auto& word : rel_strs[i].words) {
            if (words_in_query.contains(word)) {
                rel_strs[i].tf[word] += 1.0 / rel_strs[i].words.size();
            }
        }
    }
    for (auto& rel_str : rel_strs) {
        for (const auto& pair : rel_str.tf) {
            if (idf[pair.first] != 0) {
                rel_str.relevance += pair.second * std::log(1.0 / idf[pair.first]);
            }
        }
    }
    std::sort(rel_strs.begin(), rel_strs.end(), Comparator);
    for (size_t i = 0; i < results_count && rel_strs[i].relevance > 0; ++i) {
        answer.push_back(rel_strs[i].str);
    }
    return answer;
}
