#include <iostream>
#include <windows.h>
#include "InputHandler.h"
#include "WikipediaAPI.h"
#include "JSONParser.h"
#include "BrowserManager.h"

int main() {
    // Теперь ВСЁ в UTF-8: и литералы (благодаря /utf-8), и консоль, и API
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    InputHandler inputHandler;
    WikipediaAPI wikiApi;
    JSONParser parser;

    while (true) {
        std::string query = inputHandler.getSearchQuery(); // уже UTF-8

        try {
            std::cout << "Выполняем поиск...\n";

            std::string jsonResponse = wikiApi.search(query); // query уже UTF-8, urlEncode сам сделает %XX
            std::vector<SearchItem> results = parser.parse(jsonResponse);

            if (results.empty()) {
                std::cout << "По вашему запросу ничего не найдено.\n\n";
                continue;
            }

            inputHandler.displayResults(results); // title уже UTF-8 — не трогаем

            int choice = inputHandler.getUserSelection(results.size());
            if (choice == 0) {
                std::cout << "Выход из программы.\n";
                break;
            }

            std::cout << "Открываем статью в браузере...\n\n";
            BrowserManager::openArticle(results[choice - 1].pageId);
        }
        catch (const std::exception& e) {
            std::cerr << "\n[Критическая ошибка]: " << e.what() << "\nПопробуйте снова.\n\n";
        }
    }
    return 0;
}