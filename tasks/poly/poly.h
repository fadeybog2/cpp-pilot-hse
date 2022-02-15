#pragma once

#include <map>
#include <ostream>
#include <vector>

class Poly {
public:
    Poly(std::initializer_list<int64_t> coefs = {});
    Poly(std::initializer_list<std::pair<int64_t, int64_t>> coefs);

    int64_t operator()(int64_t x) const;

    bool operator==(const Poly& other) const;

    void operator+=(const Poly& other);
    void operator-=(const Poly& other);
    Poly operator+(const Poly& other) const;
    Poly operator-(const Poly& other) const;

    Poly operator-() const;

    Poly operator*(const Poly& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Poly& poly);

private:
    std::map<int64_t, int64_t, std::greater<int64_t>> coefs_;
    void DeleteZeros();
};
