#include "simplex_method.h"

#include <iostream>

// Трансплонирование матрицы A
std::vector<std::vector<double>> transpose(const std::vector<std::vector<double>> &A) {
    std::vector<std::vector<double>> transpose;
    for (size_t i = 0; i < A.at(0).size(); ++i) {
        std::vector<double> t;
        t.reserve(A.size());
        for (const auto &j : A)
            t.push_back(j.at(i));
        transpose.push_back(t);
    }
    return transpose;
}

int main() {
    // Прямая задача

    // F = 3x1 + 1x2 + 4x3
    // Ax <= b

    std::vector<std::vector<double>> A = {{16, 0, 10, 2}, {3, 6, 3, 3,}, {14, 17, 4, 9}, {4, 0, 16, 11}, {8, 12, 2, 19}};
    std::vector<double> c = {1, 1, 1, 1};
    std::vector<double> b = {1, 1, 1, 1, 1};

    //std::cout << "For B" << std::endl;

    double F = simplex_method(A, c, b, true);

    // Двойственная задача

    // G = 6y1 + 4y2 + y3
    // A(t)y >= c(t) (t) - трансплонированная

    std::vector<std::vector<double>> A_transpose = transpose(A);
    const std::vector<double> &c_transpose = b;
    const std::vector<double> &b_transpose = c;

    std::cout << "For A" << std::endl;

    double G = simplex_method(A_transpose, c_transpose, b_transpose, false);


    std::cout << "\nCONCLUSION: the solution of the direct problem (F = " << F
              << ") is equal to the solution of the dual problem (G = " << G << ")." << std::endl;
    return 0;
}
