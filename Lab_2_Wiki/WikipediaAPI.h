#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <array>
#include <memory>
#include <cstdio>
class WikipediaAPI {
private:
    std::string urlEncode(const std::string& value) {
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;
        for (char c : value) {
            if (isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' || c == '~') {
                escaped << c;
            }
            else {
                escaped << std::uppercase << '%' << std::setw(2) << int((unsigned char)c) << std::nouppercase;
            }
        }
        return escaped.str();
    }

public:
    std::string search(const std::string& query) {
        std::string encodedQuery = urlEncode(query);
        std::string url = "https://ru.wikipedia.org/w/api.php?action=query&list=search&utf8=&format=json&srsearch=" + encodedQuery;

        // Формируем команду для вызова встроенного в Windows curl (ключ -s для скрытия прогресс-бара)
        std::string cmd = "curl -s \"" + url + "\"";

        std::array<char, 128> buffer;
        std::string result;

        // Открываем пайп для чтения вывода консольной команды
        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);

        if (!pipe) {
            throw std::runtime_error("Не удалось выполнить сетевой запрос.");
        }

        // Читаем JSON-ответ порциями
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }

        if (result.empty()) {
            throw std::runtime_error("Получен пустой ответ от сервера.");
        }

        return result;
    }
};