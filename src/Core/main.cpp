#include "../Classes/Root.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Вспомогательная функция для компиляции шейдеров
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    
    // Проверка ошибок компиляции
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << std::endl;
    }
    return shader;
}

int main(int argc, char* argv[]) {
    // 1. Создаем окно через твой класс Root
    Root root("Craftwork - 3D Cube", 800, 600);

    // 2. Включаем тест глубины (Z-Buffer), чтобы задние грани куба не рисовались поверх передних
    glEnable(GL_DEPTH_TEST);

    // 3. Координаты вершин куба (X, Y, Z) + Цвета (R, G, B) для каждой вершины
    float vertices[] = {
        // Задняя грань (Красная)
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,

        // Передняя грань (Зеленая)
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    };

    // Индексы для соединения вершин в треугольники (по 2 треугольника на грань)
    unsigned int indices[] = {
        // Задняя грань
        0, 1, 2,  2, 3, 0,
        // Передняя грань
        4, 5, 6,  6, 7, 4,
        // Левая грань
        0, 4, 7,  7, 3, 0,
        // Правая грань
        1, 5, 6,  6, 2, 1,
        // Нижняя грань
        0, 1, 5,  5, 4, 0,
        // Верхняя грань
        3, 2, 6,  6, 7, 3
    };

    // 4. Создаем VAO, VBO, EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Атрибут координат вершин (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Атрибут цвета вершин (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 5. Исходный код шейдеров
    const char* vertexShaderSrc = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;

        out vec3 ourColor;
        uniform mat4 u_MVP;

        void main() {
            gl_Position = u_MVP * vec4(aPos, 1.0);
            ourColor = aColor;
        }
    )";

    const char* fragmentShaderSrc = R"(
        #version 330 core
        in vec3 ourColor;
        out vec4 FragColor;

        void main() {
            FragColor = vec4(ourColor, 1.0);
        }
    )";

    // Сборка шейдерной программы
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLint mvpLoc = glGetUniformLocation(shaderProgram, "u_MVP");

    float angle = 0.0f;

    // 6. Главный цикл программы
    while (!root.isClosed()) {
        root.pollEvents();

        // Очищаем экран и буфер глубины
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 7. Матрицы трансформации (Вращение и перспектива)
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view       = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.5f)); // Отодвигаем камеру назад
        
        angle += 0.015f; // Скорость вращения
        glm::mat4 model      = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.5f, 1.0f, 0.0f)); // Вращаем по осям X и Y

        glm::mat4 mvp = projection * view * model;

        // Передаем матрицу в шейдер
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

        // 8. Отрисовка куба
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        root.swapBuffers();
    }

    // Очистка памяти видеокарты
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    return 0;
}
