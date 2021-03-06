// Created by Alyona Dorodnyaya, Nyanyoka@yandex.ru


#include "simplex_method.h"

// Вывод двумерного массива
void print_2(const std::vector<std::vector<double>> &A) {
    for (size_t i = 0; i < 35; i++) std::cout << "-";
    std::cout << std::endl;
    for (const auto &i : A) {
        for (double j : i)
            std::cout << std::right << std::setw(8) << std::setprecision(4) << j << " ";
        std::cout << std::endl;
    }
    for (size_t i = 0; i < 35; i++) std::cout << "-";
    std::cout << '\n'
              << '\n';
}

void print_simplex(const std::vector<std::vector<double>> &A,
                   const std::vector<std::string> &sv,
                   const std::vector<std::string> &basis) {
    for (size_t i = 0; i < 50; i++) std::cout << "-";
    std::cout << std::endl;
    for (const auto &i : sv)
        std::cout << std::right << std::setw(8) << i << " ";
    std::cout << std::endl;
    size_t k = 1;
    for (const auto &i : A) {
        std::cout << std::right << std::setw(8) << basis[k];
        for (double j : i)
            std::cout << std::right << std::setw(8) << std::setprecision(4) << j << " ";
        std::cout << std::endl;
        k++;
    }
    for (size_t i = 0; i < 50; i++) std::cout << "-";
    std::cout << '\n'
              << '\n';
}

// Вывод одномерного массива
void print_1(const std::vector<double> &A) {
    for (size_t i = 0; i < 35; i++) std::cout << "-";
    std::cout << std::endl;
    for (const auto &i : A)
        std::cout << std::setw(8) << i << " ";
    std::cout << std::endl;
    for (size_t i = 0; i < 35; i++) std::cout << "-";
    std::cout << '\n'
              << '\n';
}

// Изначальное заполнение симплекс-таблицы
// task: true - прямая задача, false - двойственная задача
void first_table(std::vector<std::vector<double>> &A,
                 std::vector<double> c, std::vector<double> &b,
                 std::vector<std::vector<double>> &simplex,
                 const std::vector<std::string> &sv,
                 const std::vector<std::string> &basis, bool task) {
    simplex.reserve(A.size() + 1);
    size_t k = 0;
    for (auto &i : A) {
        if (!task)
            for (auto &j : i) j = -j;// если двойственная, умножаем матрицу A на -1, приводим к знаку <=
        // если двойственная, умножаем вектор b на -1, приводим к знаку <=
        if (task) i.insert(i.begin(), b[k]);
        else
            i.insert(i.begin(), -b[k]);
        simplex.push_back(i);
        k++;
    }
    for (double &i : c) i = -i;
    c.insert(c.begin(), 0);
    simplex.push_back(c);
    std::cout << std::setw(20) << "Simplex" << std::endl;
    print_simplex(simplex, sv, basis);
}

// Разрешающий столбец
// task: true - прямая задача (ищем минимальный отрицательный),
// false - двойственная задача (ищем максимальный положительный)
size_t column(const std::vector<double> &c, bool task) {
    size_t column = 10;// в случае оптимального решения функция возвращает 10
    double s = c.at(1);
    for (size_t i = 1; i < c.size(); i++) {
        if ((task && c.at(i) < 0 && c.at(i) <= s) || ((!task && c.at(i) > 0 && c.at(i) >= s))) {
            s = c.at(i);
            column = i;
        }
    }
    return column;
}

// Разрешающая строка
size_t string(size_t column_, const std::vector<std::vector<double>> &simplex) {
    size_t string_ = 0;// в случае, когда функция не ограничена на множестве решений вернет -1
    double C = -1;
    for (size_t i = 0; i < simplex.size() - 1; i++) {
        if (simplex.at(i).at(0) > 0 && simplex.at(i).at(column_) > 0) {
            if (C == -1) string_ = i;
            if (simplex.at(i).at(0) / simplex.at(i).at(column_) < C)
                string_ = i;
            C = simplex.at(i).at(0) / simplex.at(i).at(column_);
        }
    }
    if (C == -1) return -1;
    std::cout << "Element: " << simplex[string_][column_] << " (in column: " << column_ + 1
              << " and string: " << string_ + 1 << "). Ratio "
              << simplex.at(string_).at(0) << "/" << simplex.at(string_).at(column_)
              << " = " << simplex.at(string_).at(0) / simplex.at(string_).at(column_) << std::endl;
    std::cout << '\n';
    return string_;
}

void problem(std::vector<std::vector<double>> &simplex, bool task,
             std::vector<std::string> &sv,
             std::vector<std::string> &basis) {
    size_t column_, string_;
    bool ok = true;
    while (ok) {
        size_t u = -1;
        double min = 0;
        for (size_t i = 0; i < simplex.size() - 1; ++i) {
            if (simplex.at(i).at(0) < min) {
                min = simplex.at(i).at(0);
                u = i;
            }
        }
        if (u == -1) ok = false;
        else {
            string_ = u;
            u = 1;
            while (u < simplex.at(1).size() && simplex.at(string_).at(u) >= 0) u++;
            if (u != simplex.at(1).size()) {
                column_ = u;

                double element = simplex.at(string_).at(column_);
                // Меняем переменные
                std::swap(sv.at(column_ + 1), basis.at(string_ + 1));
                std::cout << "Swap " << sv.at(column_ + 1) << " <-> " << basis.at(string_ + 1) << std::endl;

                // Изменяем таблицу:
                // - элементы разрешающий строки / разрешающий элемент
                // - элементы разрешающего столбца / противоположный разрешающиму элементу
                // - разрешающий элемент заменяется противоположным самому себе (1/element)
                for (size_t i = 0; i < simplex.size(); i++)
                    for (size_t j = 0; j < simplex.at(1).size(); j++)
                        if (i != string_ && j != column_)
                            simplex.at(i).at(j) -= simplex.at(string_).at(j) * simplex.at(i).at(column_) / simplex.at(string_).at(column_);

                for (size_t i = 0; i < simplex.size(); i++)
                    for (size_t j = 0; j < simplex.at(1).size(); j++) {
                        if (i == string_ && j != column_) simplex.at(i).at(j) /= element;
                        if (i == string_ && j == column_) simplex.at(i).at(j) = 1 / element;
                        if (i != string_ && j == column_) simplex.at(i).at(j) /= (-1) * element;
                    }
                // Вывод полученной симплекс-таблицы
                std::cout << std::setw(20) << "Simplex (b<0)" << std::endl;
                print_simplex(simplex, sv, basis);
            }
        }
    }
}

// task: true - прямая задача, false - двойственная задача
double simplex_method(std::vector<std::vector<double>> A,
                      const std::vector<double> &c,
                      std::vector<double> b, bool task) {

    double ans = 0;// для хранения результата

    // Вывод матриц из условия
    if (task) std::cout << std::setw(18) << "A" << std::endl;
    else
        std::cout << std::setw(18) << "A_transpose" << std::endl;
    print_2(A);
    if (task) std::cout << std::setw(18) << "c" << std::endl;
    else
        std::cout << std::setw(18) << "с_transpose" << std::endl;
    print_1(c);
    if (task) std::cout << std::setw(18) << "b" << std::endl;
    else
        std::cout << std::setw(18) << "b_transpose" << std::endl;
    print_1(b);

    // Векторa для хранения базисных и свободных переменных:
    // изначально свободные переменные - x1, x2, x3
    std::vector<std::string> sv, basis;

    if (task) {
        sv = {" ", "b", "x1", "x2", "x3", "x4"};
        basis = {"-", "x5", "x6", "x7", "x8", "x9", "F"};
    } else {
        sv = {" ", "b", "y1", "y2", "y3", "y4", "y5"};
        basis = {"-", "y6", "y7", "y8", "y9", "F"};
    }

    // Создание и заполенение симплекс таблицы
    std::vector<std::vector<double>> simplex;
    first_table(A, c, b, simplex, sv, basis, task);
    size_t column_, string_;

    problem(simplex, task, sv, basis);

    // Самое первое решение - опорное
    size_t k = 1;
    // Проверка оптимальности
    while (column(simplex.at(simplex.size() - 1), task) != 10) {
        std::cout << k << ")" << std::endl;

        // Находим разрешающий столбец, строку и элемент
        column_ = column(simplex.at(simplex.size() - 1), task);
        std::cout << "Minimum is " << simplex.at(simplex.size() - 1).at(column_) << std::endl;
        string_ = string(column_, simplex);
        if (string_ == -1) std::cout << "There is no optimal solution" << std::endl;// проверка на то, имеет ли пример вообще оптимальное решение на данном множестве
        else {
            double element = simplex.at(string_).at(column_);
            // Меняем переменные
            std::swap(sv.at(column_ + 1), basis.at(string_ + 1));
            std::cout << "Swap " << sv.at(column_ + 1) << " <-> " << basis.at(string_ + 1) << std::endl;

            // Изменяем таблицу:
            // - элементы разрешающий строки / разрешающий элемент
            // - элементы разрешающего столбца / противоположный разрешающиму элементу
            // - разрешающий элемент заменяется противоположным самому себе (1/element)
            for (size_t i = 0; i < simplex.size(); i++)
                for (size_t j = 0; j < simplex.at(1).size(); j++)
                    if (i != string_ && j != column_)
                        simplex.at(i).at(j) -= simplex.at(string_).at(j) * simplex.at(i).at(column_) / simplex.at(string_).at(column_);

            for (size_t i = 0; i < simplex.size(); i++)
                for (size_t j = 0; j < simplex.at(1).size(); j++) {
                    if (i == string_ && j != column_) simplex.at(i).at(j) /= element;
                    if (i == string_ && j == column_) simplex.at(i).at(j) = 1 / element;
                    if (i != string_ && j == column_) simplex.at(i).at(j) /= (-1) * element;
                }
            // Вывод полученной симплекс-таблицы
            std::cout << std::setw(20) << "Simplex" << std::endl;
            print_simplex(simplex, sv, basis);

            // Подведение итога оптимизации. Если итоговый ответ - оптимальный, выписывается ответ
            problem(simplex, task, sv, basis);
            if (column(simplex.at(simplex.size() - 1), task) != 10) {
                if (k == 1) std::cout << "This is the reference solution" << '\n';
                std::cout << "This is't the optimal solution" << std::endl;
            } else {
                std::cout << "This is the optimal solution" << '\n'
                          << '\n';
                ans = simplex.at(simplex.size() - 1).at(0);
                if (!task) {
                    std::cout << "ANSWER: min G = min("
                              << c.at(0) << "y1 + "
                              << c.at(1) << "y2 + "
                              << c.at(2) << "y3 + "
                              << c.at(3) << "y4 + "
                              << c.at(4) << "y5) = "
                              << simplex.at(simplex.size() - 1).at(0) << "\n"
                              << sv[2] << ", " << sv[3] << ", " << sv[4] << ", " << sv[5] << ", " << sv[6] << " = 0, "
                              << basis[1] << " = " << simplex.at(0).at(0) << ", "
                              << basis[2] << " = " << simplex.at(1).at(0) << ", "
                              << basis[3] << " = " << simplex.at(2).at(0) << ", "
                              << basis[4] << " = " << simplex.at(3).at(0);
                } else {
                    std::cout << "ANSWER: max F = max("
                              << c.at(0) << "x1 + "
                              << c.at(1) << "x2 + "
                              << c.at(2) << "x3 + "
                              << c.at(3) << "x4) = "
                              << simplex.at(simplex.size() - 1).at(0) << "\n"
                              << sv[2] << ", " << sv[3] << ", " << sv[4] << ", " << sv[5] << ", "
                              << " = 0, "
                              << basis[1] << " = " << simplex.at(0).at(0) << ", "
                              << basis[2] << " = " << simplex.at(1).at(0) << ", "
                              << basis[3] << " = " << simplex.at(2).at(0) << ", "
                              << basis[4] << " = " << simplex.at(3).at(0) << ", "
                              << basis[5] << " = " << simplex.at(4).at(0);
                }
            }
            std::cout << std::endl;

            k++;
        }
    }
    return ans;
}