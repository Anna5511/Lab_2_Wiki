#include <iostream>
#include "InputHandler.h"
#include "WikipediaAPI.h"
#include "JSONParser.h"
#include "BrowserManager.h"

int main() {
    // Настраиваем корректный вывод кириллицы в консоль (для Windows)
#if defined(_WIN32)
    system("chcp 65001 > nul");
#endif

    InputHandler inputHandler;
    WikipediaAPI wikiApi;
    JSONParser parser;

    while (true) {
        std::string query = inputHandler.getSearchQuery();

        try {
            std::cout << "Выполняем поиск...\n";
            std::string jsonResponse = wikiApi.search(query);

            std::vector<SearchItem> results = parser.parse(jsonResponse);

            if (results.empty()) {
                std::cout << "По вашему запросу ничего не найдено.\n\n";
                continue;
            }

            inputHandler.displayResults(results);
            int choice = inputHandler.getUserSelection(results.size());

            if (choice == 0) {
                std::cout << "Выход из программы.\n";
                break;
            }

            int pageIdToOpen = results[choice - 1].pageId;
            std::cout << "Открываем статью в браузере...\n\n";
            BrowserManager::openArticle(pageIdToOpen);

        }
        catch (const std::exception& e) {
            // Обработка сетевых ошибок и ошибок парсинга (чтобы программа не упала)
            std::cerr << "\n[Критическая ошибка]: " << e.what() << "\nПопробуйте снова.\n\n";
        }
    }

    return 0;
}