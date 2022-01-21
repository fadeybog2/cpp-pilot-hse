#include "associative_operation.h"

bool IsAssociative(const std::vector<std::vector<size_t>>& table) {
    size_t n = table.size();
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            for (size_t k = 0; k < n; ++k) {
                if (table[table[i][j]][k] != table[i][table[j][k]]) {
                    return false;
                }
            }
        }
    }
    return true;
}
