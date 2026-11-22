#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "SearchItem.h"

class InputHandler {
public:
    // Считывание запроса (с защитой от пустого ввода)
    std::string getSearchQuery() {
        std::string query;
        while (true) {
            std::cout << "Введите поисковый запрос для Википедии: ";
            std::getline(std::cin, query);
            if (!query.empty()) {
                return query;
            }
            std::cout << "Ошибка: запрос не может быть пустым. Попробуйте снова.\n";
        }
    }

    // Вывод результатов
    void displayResults(const std::vector<SearchItem>& results) {
        std::cout << "\n--- Результаты поиска ---\n";
        for (size_t i = 0; i < results.size(); ++i) {
            std::cout << "[" << i + 1 << "] " << results[i].title << "\n";
        }
        std::cout << "-------------------------\n";
    }

    // Выбор статьи пользователем
    int getUserSelection(int maxItems) {
        int choice = 0;
        std::string input;
        while (true) {
            std::cout << "\nВведите номер статьи для открытия в браузере (или 0 для выхода): ";
            std::getline(std::cin, input);

            try {
                choice = std::stoi(input); // Пытаемся перевести строку в число
                if (choice >= 0 && choice <= maxItems) {
                    return choice;
                }
                else {
                    std::cout << "Ошибка: введите число от 0 до " << maxItems << ".\n";
                }
            }
            catch (const std::invalid_argument&) {
                std::cout << "Ошибка: некорректный ввод. Ожидается число.\n";
            }
            catch (const std::out_of_range&) {
                std::cout << "Ошибка: слишком большое число.\n";
            }
        }
    }
};