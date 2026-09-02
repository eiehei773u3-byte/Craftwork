#include "../../Classes/functions.hpp"

#include <clocale>
#include <cstdio>
#include <iostream>
#include <thread>   
#include <chrono>   

void animatedPrint(char symbol, int count) {
    // Цикл повторяется столько раз, сколько указано в числе
    for (int i = 0; i < count; ++i) {
        // Выводим один символ без переноса строки
        std::cout << symbol;
        
        // Сбрасываем буфер вывода, чтобы символ сразу появился на экране, а не ждал конца строки
        std::cout << std::flush; 
        
        // Задержка в 0.05 секунды (50 миллисекунд)
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
    
    // В самом конце добавляем перенос строки (\n)
    std::cout << std::endl; 
}

// Макрос для красивого вывода с уровнем
#define LOG(level_color, level_name, format, ...) \
    printf("%s[%s]%s " format "%s\n", level_color, level_name, DEV_RESET, ##__VA_ARGS__)

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


    

void Console() {
    ensureConsole();
    setlocale(LC_ALL, "Russian");     
    #if defined(_WIN32)
        system("cls");
        printf("build:\n");
        printf("- Windows (x32)\n");
    #elif defined(__WIN64)
        system("cls");
        printf("build:\n");
        printf("- Windows (x64)\n");
    #else
        system("clear");
        printf("build:\n");
        printf("- Linux/MacOS\n");
    #endif
    printf("- version: ");
    std::cout << VERSION << "\n\n";
    animatedPrint('-', 20);


    
}