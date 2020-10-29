// Created by Alyona Dorodnyaya, Nyanyoka@yandex.ru

#include "transpose.h"
#include "simplex_method.h"

std::vector<std::vector<double>> transpose_2(const std::vector<std::vector<double>> &A) {
    std::vector<std::vector<double>> transpose;
    for (size_t i = 0; i < A.size(); ++i) {
        std::vector<double> t;
        t.reserve(A.size());
        for (const auto &j : A)
            t.push_back(j.at(i));
        transpose.push_back(t);
    }
    return transpose;
}