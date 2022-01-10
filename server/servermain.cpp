/*
 * Создание сервера
 */
#include "server.hpp" // Описание класса сервера
#include <iostream>   // cin//cout

int main()
{
  // Проверяем, что объект не бросил исключение при создании
  try {
    server s("0.0.0.0", "8000", "8001", 200); // ip, code port, data port, backlog
    return s.exec();                          // Запуск сервера
  } catch(...) {
    std::cout << "server was not created" << std::endl;
    return -1;
  }
  return 0;
}
