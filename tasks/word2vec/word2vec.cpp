#include "word2vec.h"

#include <vector>
#include <numeric>
#include <limits>

std::vector<std::string> FindClosestWords(const std::vector<std::string>& words,
                                          const std::vector<std::vector<int>>& vectors) {
    if (vectors.empty()) {
        return std::vector<std::string>();
    }
    const auto& first_vec = vectors.front();
    std::vector<std::string> answer;
    int max_sc_mp = std::numeric_limits<int>::min();
    for (size_t i = 1; i < vectors.size(); ++i) {
        const auto& vec = vectors[i];
        int sc_mp = std::inner_product(first_vec.begin(), first_vec.end(), vec.begin(), 0);
        if (sc_mp > max_sc_mp) {
            max_sc_mp = sc_mp;
            answer.clear();
        }
        if (sc_mp == max_sc_mp) {
            answer.push_back(words[i]);
        }
    }
    return answer;
}
