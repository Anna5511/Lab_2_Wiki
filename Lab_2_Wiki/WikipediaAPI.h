#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#define CPPHTTPLIB_OPENSSL_SUPPORT // Включаем поддержку HTTPS
#include "httplib.h"

class WikipediaAPI {
private:
    // Вспомогательный метод для Percent-encoding (URL Encoding)
    std::string urlEncode(const std::string& value) {
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;
        for (char c : value) {
            // Оставляем буквы, цифры и базовые символы как есть
            if (isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' || c == '~') {
                escaped << c;
            }
            else {
                // Остальное кодируем в %XX
                escaped << std::uppercase << '%' << std::setw(2) << int((unsigned char)c) << std::nouppercase;
            }
        }
        return escaped.str();
    }

public:
    // Метод делает запрос к серверу и возвращает сырой JSON
    std::string search(const std::string& query) {
        // Создаем клиент для HTTPS соединения с Википедией
        httplib::SSLClient cli("ru.wikipedia.org");
        cli.set_connection_timeout(5); // Таймаут 5 секунд

        std::string encodedQuery = urlEncode(query);
        std::string path = "/w/api.php?action=query&list=search&utf8=&format=json&srsearch=" + encodedQuery;

        auto res = cli.Get(path.c_str());

        if (res) {
            if (res->status == 200) {
                return res->body; // Возвращаем JSON
            }
            else {
                throw std::runtime_error("Ошибка сервера: Код " + std::to_string(res->status));
            }
        }
        else {
            auto err = res.error();
            throw std::runtime_error("Сетевая ошибка при подключении к Википедии.");
        }
    }
};