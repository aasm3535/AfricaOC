#ifndef SHELL_H
#define SHELL_H

/**
 * @brief Инициализирует командный интерпретатор.
 * (В будущем здесь можно будет, например, загружать переменные окружения)
 */
void shell_init();

/**
 * @brief Выполняет введенную пользователем команду.
 * @param input Строка, введенная пользователем.
 */
void shell_execute(const char* input);

#endif // SHELL_H
