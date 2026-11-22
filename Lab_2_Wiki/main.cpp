#include <iostream>
#include "InputHandler.h"
#include "WikipediaAPI.h"
#include "JSONParser.h"
#include "BrowserManager.h"
#include "StringConverter.h"
#include <windows.h> 

int main() {
    
    InputHandler inputHandler;
    WikipediaAPI wikiApi;
    JSONParser parser;

    while (true) {
        std::string query = inputHandler.getSearchQuery();

        try {
            std::cout << "Выполняем поиск...\n";

            std::string utf8Query = StringConverter::toUTF8(query);
            std::string jsonResponse = wikiApi.search(utf8Query);
            std::vector<SearchItem> results = parser.parse(jsonResponse);

            if (results.empty()) {
                std::cout << "По вашему запросу ничего не найдено.\n\n";
                continue;
            }

            for (auto& item : results) {
                item.title = StringConverter::toConsole(item.title);
            }

            inputHandler.displayResults(results);

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