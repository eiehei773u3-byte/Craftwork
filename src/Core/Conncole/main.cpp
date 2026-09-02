#include <clocale>
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


int Conncole() {
    setlocale(LC_ALL, "Russian"); 




    return 0;
}