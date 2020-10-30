// Created by Alyona Dorodnyaya, Nyanyoka@yandex.ru

#ifndef LAB_OPTIM_2_SIMPLEX_METHOD_H
#define LAB_OPTIM_2_SIMPLEX_METHOD_H

#include <iomanip>
#include <iostream>
#include <vector>

// Cимплекс метод, task: 1 - прямая задача, 0 - обратная задача
double simplex_method(std::vector<std::vector<double>> A,
                      const std::vector<double> &c,
                      std::vector<double> b, bool task);

// Вывод двумерного массива
void print_2(const std::vector<std::vector<double>> &A);

// Вывод симплекс таблицы
void print_simplex(const std::vector<std::vector<double>> &A,
                   const std::vector<std::string> &sv,
                   const std::vector<std::string> &basis);

// Вывод одномерного массива
void print_1(const std::vector<double> &A);

// Изначальное заполнение симплекс-таблицы
void first_table(std::vector<std::vector<double>> &A,
                 std::vector<double> c, std::vector<double> &b,
                 std::vector<std::vector<double>> &simplex,
                 const std::vector<std::string> &sv,
                 const std::vector<std::string> &basis, bool task);

// Нахождение разрешающего столбца
size_t column(const std::vector<double> &c, bool task);

// Нахождение разрешающей строки по разрешающему столбцу
size_t string(size_t column_, const std::vector<std::vector<double>> &simplex);

// Избавляемся от отрицательных коэффициентов в векторе b
void problem(std::vector<std::vector<double>> A,
             const std::vector<double> &c,
             std::vector<double> b, bool task);
#endif//LAB_OPTIM_2_SIMPLEX_METHOD_H
