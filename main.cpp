#include "simplex_method.h"
#include "transpose.h"
#include <iostream>

int main() {
    // Прямая задача

    // F = 3x1 + 1x2 + 4x3
    // Ax <= b

    std::vector<std::vector<double>> A = {{2, 1, 1}, {1, 4, 0}, {0, 0.5, 1}};
    std::vector<double> c = {3, 1, 4};
    std::vector<double> b = {6, 4, 1};

    simplex_method(A, c, b, true);

    // Двойственная задача

    // G = 6y1 + 4y2 + y3
    // A(t)y >= c(t) (t) - трансплонированная

    std::vector<std::vector<double>> A_transpose = transpose_2(A);
    const std::vector<double>& c_transpose = c;
    const std::vector<double>& b_transpose = b;

    simplex_method(A_transpose, b_transpose, c_transpose, false);

    return 0;
}
