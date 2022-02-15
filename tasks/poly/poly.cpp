#include "poly.h"

#include <cmath>

Poly::Poly(std::initializer_list<int64_t> il) {
    int64_t degree = 0;
    for (const auto& coef : il) {
        if (coef != 0) {
            coefs_[degree] = coef;
        }
        ++degree;
    }
    DeleteZeros();
}

Poly::Poly(std::initializer_list<std::pair<int64_t, int64_t>> il) {
    for (const auto& [degree, coef] : il) {
        coefs_[degree] = coef;
    }
}

int64_t Poly::operator()(int64_t x) const {
    int64_t result = 0;
    for (const auto& [degree, coef] : coefs_) {
        result += coef * powl(x, degree);
    }
    return result;
}

bool Poly::operator==(const Poly& other) const {
    return coefs_ == other.coefs_;
}

void Poly::operator+=(const Poly& other) {
    for (const auto& [degree, coef] : other.coefs_) {
        coefs_[degree] += coef;
    }
    DeleteZeros();
}

void Poly::operator-=(const Poly& other) {
    for (const auto& [degree, coef] : other.coefs_) {
        coefs_[degree] -= coef;
    }
    DeleteZeros();
}

Poly Poly::operator+(const Poly& other) const {
    Poly result = *this;
    result += other;
    return result;
}

Poly Poly::operator-(const Poly& other) const {
    Poly result = *this;
    result -= other;
    return result;
}

Poly Poly::operator-() const {
    return Poly() - *this;
}

Poly Poly::operator*(const Poly& other) const {
    Poly result;
    for (const auto& [lhs_degree, lhs_coef] : coefs_) {
        for (const auto& [rhs_degree, rhs_coef] : other.coefs_) {
            result.coefs_[lhs_degree + rhs_degree] += lhs_coef * rhs_coef;
        }
    }
    result.DeleteZeros();
    return result;
}
std::ostream& operator<<(std::ostream& os, const Poly& poly) {
    os << "y = ";
    if (poly.coefs_.empty()) {
        os << "0";
        return os;
    }
    for (const auto& [degree, coef] : poly.coefs_) {
        if (degree == 0) {
            os << coef;
            continue;
        }
        if (coef != 1) {
            os << coef;
        }
        os << "x";
        if (degree != 1) {
            os << "^" << degree;
        }
        os << " + ";
    }
    return os;
}

void Poly::DeleteZeros() {
    std::vector<int64_t> degrees_to_delete;
    for (const auto& [degree, coef] : coefs_) {
        if (coef == 0) {
            degrees_to_delete.push_back(degree);
        }
    }
    for (const auto degree : degrees_to_delete) {
        coefs_.erase(degree);
    }
}
