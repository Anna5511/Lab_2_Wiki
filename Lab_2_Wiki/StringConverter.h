#pragma once
#include <string>

#if defined(_WIN32)
#include <windows.h>
#endif

class StringConverter {
public:
    // Перевод из кодировки консоли Windows в UTF-8 (для отправки запроса в API)
    static std::string toUTF8(const std::string& consoleStr) {
#if defined(_WIN32)
        if (consoleStr.empty()) return "";

        int cp = GetConsoleCP(); // Получаем кодировку ввода консоли
        int wlen = MultiByteToWideChar(cp, 0, consoleStr.c_str(), -1, NULL, 0);
        std::wstring wstr(wlen, 0);
        MultiByteToWideChar(cp, 0, consoleStr.c_str(), -1, &wstr[0], wlen);

        int utf8len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
        std::string utf8str(utf8len, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8str[0], utf8len, NULL, NULL);

        // Удаляем нулевой символ в конце строки, который добавляет Windows API
        if (!utf8str.empty() && utf8str.back() == '\0') {
            utf8str.pop_back();
        }
        return utf8str;
#else
        return consoleStr; // Для Linux/Mac конвертация не нужна
#endif
    }

    // Перевод из UTF-8 (от API) в кодировку консоли Windows (для красивого вывода)
    static std::string toConsole(const std::string& utf8Str) {
#if defined(_WIN32)
        if (utf8Str.empty()) return "";

        int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, NULL, 0);
        std::wstring wstr(wlen, 0);
        MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wstr[0], wlen);

        int cp = GetConsoleOutputCP(); // Получаем кодировку вывода консоли
        int locallen = WideCharToMultiByte(cp, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
        std::string localStr(locallen, 0);
        WideCharToMultiByte(cp, 0, wstr.c_str(), -1, &localStr[0], locallen, NULL, NULL);

        if (!localStr.empty() && localStr.back() == '\0') {
            localStr.pop_back();
        }
        return localStr;
#else
        return utf8Str;
#endif
    }
};