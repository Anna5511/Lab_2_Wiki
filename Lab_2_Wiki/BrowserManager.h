#pragma once
#include <string>
#include <cstdlib>

class BrowserManager {
public:
    // Метод формирует ссылку по pageId и открывает её
    static void openArticle(int pageId) {
        std::string url = "https://ru.wikipedia.org/w/index.php?curid=" + std::to_string(pageId);

        // Кроссплатформенный выбор команды терминала для открытия ссылки
#if defined(_WIN32)
        std::string cmd = "start " + url;
#elif defined(__APPLE__)
        std::string cmd = "open " + url;
#else
        std::string cmd = "xdg-open " + url;
#endif

        // Выполняем системную команду
        std::system(cmd.c_str());
    }
};