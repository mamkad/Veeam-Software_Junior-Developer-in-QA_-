/*
 * Реализация базового класса для классов сервера и клиента
 */
#include "conclser.hpp"

#include <string.h>    // memset
#include <iostream>    // cin/cout
#include <arpa/inet.h> // inet_pton
#include <exception>   // invalid_argument
#include <unistd.h>    // close/read/write

using std::cout;
using std::cin;
using std::endl;

// Конструктор
conclser::conclser(string const& ipa, string const& dport, string const& cport)
  : ip(ipa), data_port(dport), code_port(cport)
{
  // Инициализируем структуры адресов
  // Создаём сокеты
  try {
    // адреса
    set_addres(data_address, data_port);
    set_addres(code_address, code_port);
    // сокеты
    create_socket(data_fd);
    create_socket(code_fd);
  } catch(...) {
    throw std::invalid_argument("err"); // Бросаем исключением в случае неудачной инициализации
  }
}

// Деструктор
conclser::~conclser()
{
  // Закрываем сокеты
  cls_connect(code_fd);
  cls_connect(data_fd);
  cout << endl << "descriptors are closed" << endl;
}

// Инициализация структуры адреса
void conclser::set_addres(struct sockaddr_in& address, string const& port)
{
  memset(&address, 0, sizeof(address)); // Заполняем address нулями
  address.sin_family = AF_INET;         // Протокол (IPv4)

  // Проверка корректности ip
  if (inet_pton(AF_INET, ip.c_str(), &address.sin_addr) <= 0) {
    cout << endl << "inet_pton error for " << ip.c_str() << endl;
    throw std::invalid_argument("err"); // Бросаем исключением в случае неправильного адреса
  }

  // Проверка корректности порта данных
  int iport;
  try {
    iport = std::stoi(port);
    // Проверяем, что порт не равено 0
    if (!iport) {
      throw;
    }
  } catch(...) {
    cout  << endl << "Backlog must be a numeric value greater than zero" << endl;
    throw std::invalid_argument("err");
  }
  // Присваиваем порт
  address.sin_port = htons(iport);
}

// Создание сокетов
void conclser::create_socket(int& fd)
{
  if ( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    cout  << endl << "Error of socket" << endl;
    throw std::invalid_argument("err"); // Бросаем исключением в случае неудачного создания сокета
  }
}

// Закрыть соединение
void conclser::cls_connect(int fd)
{
  close(fd);
}

// Функция работы клиента/сервера
// Главный цикл accept()
int conclser::exec()
{
  return 0;
}
