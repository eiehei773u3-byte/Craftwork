#include "../Classes/Root.hpp"
#include <string>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h> // Для isatty и readlink
#endif

void ensureConsole() {
#ifdef _WIN32
    // --- ВЕРСИЯ ДЛЯ WINDOWS ---
    HWND hwnd = GetConsoleWindow();
    if (hwnd == NULL) {
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);
        std::string cmd = "start cmd /k \"" + std::string(path) + "\"";
        system(cmd.c_str());
        exit(0);
    }
#else
    // --- ВЕРСИЯ ДЛЯ LINUX (С акцентом на XFCE) ---
    // Если программа запущена двойным кликом из файлового менеджера (Thunar), консоли нет
    if (!isatty(fileno(stdout))) {
        char path[1024];
        ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
        if (len != -1) {
            path[len] = '\0';
            
            // Специально для XFCE используем exo-open или xfce4-terminal, 
            // а в конце добавляем read, чтобы окно не закрылось мгновенно после завершения
            std::string cmd = "exo-open --terminal -- '" + std::string(path) + "' || "
                              "xfce4-terminal --execute '" + std::string(path) + "'";
            
            system(cmd.c_str());
            exit(0);
        }
    }
#endif
}



int main() {
    printf("\n");
    ensureConsole();


    return 0;
}