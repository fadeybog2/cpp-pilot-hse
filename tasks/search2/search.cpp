#include "search.h"

#include <cmath>

void SearchEngine::BuildIndex(std::string_view text) {
    index_.BuildIndex(text);
}
void SearchEngine::Index::BuildIndex(std::string_view text) {
    size_t beg = 0;
    size_t str_beg = 0;
    bool is_word = std::isalpha(text[beg]);
    docs.resize(1);
    for (size_t end = 0; end < text.size(); ++end) {
        if (end == text.size() - 1) {
            if (is_word) {
                docs.back().words.push_back(text.substr(beg, end - beg));
                is_word = false;
                beg = end;
            }
            docs.back().doc_text = text.substr(str_beg, end - str_beg + 1);
        } else if (std::isalpha(text[end])) {
            if (!is_word) {
                beg = end;
                is_word = true;
            }
        } else {
            if (is_word) {
                docs.back().words.push_back(text.substr(beg, end - beg));
                is_word = false;
                beg = end;
            }
        }
        if (text[end] == '\n') {
            docs.back().doc_text = text.substr(str_beg, end - str_beg);
            str_beg = end + 1;
            docs.push_back(Document());
        }
    }

    size_t not_empty_strings_number =
        std::count_if(docs.begin(), docs.end(), [](const Document& doc) { return !doc.words.empty(); });
    for (auto& doc : docs) {
        for (const auto& word : doc.words) {
            doc.words_tf[word] += 1.0 / doc.words.size();
            idf[word] += 1.0 / not_empty_strings_number;
        }
    }
}

CustomSet SearchEngine::SplitQuery(std::string_view query) const {
    CustomSet words;
    size_t beg = 0;
    bool is_word = std::isalpha(query[beg]);
    for (size_t end = 0; end < query.size(); ++end) {
        if (end == query.size() - 1) {
            if (is_word) {
                if (std::isalpha(query[end])) {
                    end++;
                }
                words.insert(query.substr(beg, end - beg));
            }
        } else if (std::isalpha(query[end])) {
            if (!is_word) {
                is_word = true;
                beg = end;
            }
        } else {
            if (is_word) {
                words.insert(query.substr(beg, end - beg));
                is_word = false;
                beg = end;
            }
        }
    }
    return words;
}
std::vector<std::string_view> SearchEngine::Search(std::string_view query, size_t results_count) const {
    CustomSet words_in_query = SplitQuery(query);
    std::vector<std::pair<const Index::Document*, double>> result;
    for (const auto& doc : index_.docs) {
        result.push_back(std::pair(&doc, 0));
    }
    for (size_t i = 0; i < index_.docs.size(); ++i) {
        const auto& doc = index_.docs[i];
        result[i].first = &index_.docs[i];
        for (const auto& word : doc.words) {
            if (words_in_query.contains(word)) {
                result[i].second += doc.words_tf.at(word) * std::log(1.0 / index_.idf.at(word));
            }
        }
    }
    std::sort(result.begin(), result.end(), [](const auto& lhs, const auto& rhs) { return lhs.second > rhs.second; });
    std::vector<std::string_view> answer;
    for (size_t i = 0; i < results_count && result[i].second > 0; ++i) {
        answer.push_back(result[i].first->doc_text);
    }
    return answer;
}