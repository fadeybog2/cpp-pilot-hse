#include "word2vec.h"

#include <vector>

int ScalarMultiplication(std::vector<int> vector1, std::vector<int> vector2) {
    int answer = 0;
    for (size_t i = 0; i < vector1.size(); ++i) {
        answer += vector1[i] * vector2[i];
    }
    return answer;
}
std::vector<std::string> FindClosestWords(const std::vector<std::string>& words,
                                          const std::vector<std::vector<int>>& vectors) {
    std::vector<int> first_vec = vectors.front();
    std::vector<std::string> answer;
    int max_sc_mp = -1;
    for (size_t i = 0; i < vectors.size(); ++i) {
        auto vec = vectors[i];
        int sc_mp = ScalarMultiplication(first_vec, vec);
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
