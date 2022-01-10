/*
 * Создание клиента
 */
#include "client.hpp" // Описание класса клиента
#include <iostream>   // cin//cout

int main()
{
  // Проверяем, что объект не бросил исключение при создании
  try {
    client cl("0.0.0.0", "8000", "8001"); // ip, code port, data port, backlog
    return cl.exec();                     // Запуск клиента
  } catch(...) {
    std::cout << "server was not created" << std::endl;
    return -1;
  }
}
