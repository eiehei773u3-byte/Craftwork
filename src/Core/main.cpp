#include "../Classes/Root.hpp"

int main(int argc, char* argv[]) {
    // Root создаёт и инициализирует окно
    Root root("Craftwork", 800, 600);

    // Цикл работы с окном
    while (!root.isClosed()) {
        root.pollEvents();
        root.clear(0.1f, 0.1f, 0.15f, 1.0f);

        // Вызовы OpenGL рендеринга...

        root.swapBuffers();
    }

    return 0;
}
