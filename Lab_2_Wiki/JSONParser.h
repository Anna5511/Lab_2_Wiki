#pragma once
#include <string>
#include <vector>
#include "SearchItem.h"
#include "json.hpp"

using json = nlohmann::json;

class JSONParser {
public:
    std::vector<SearchItem> parse(const std::string& jsonString) {
        std::vector<SearchItem> results;
        try {
            // Парсим строку в JSON объект
            json j = json::parse(jsonString);

            // Проверяем, есть ли поле "query" и "search"
            if (j.contains("query") && j["query"].contains("search")) {
                // Проходимся по массиву результатов
                for (const auto& item : j["query"]["search"]) {
                    SearchItem searchItem;
                    searchItem.pageId = item["pageid"];
                    searchItem.title = item["title"];
                    results.push_back(searchItem);
                }
            }
        }
        catch (const json::parse_error& e) {
            std::cerr << "Ошибка парсинга JSON: " << e.what() << '\n';
        }
        return results;
    }
};