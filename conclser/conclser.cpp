/*
 * Реализация базового класса для классов сервера и клиента
 */
#include "conclser.hpp"

#include <string.h>    // memset
#include <iostream>    // cin/cout
#include <arpa/inet.h> // inet_pton
#include <exception>   // invalid_argument

using std::cout;
using std::cin;
using std::endl;

// Конструктор
conclser::conclser(string const& ipa, string const& dport)
  : ip(ipa), data_port(dport)
{
  memset(&address, 0, sizeof(address)); // Заполняем address нулями
  address.sin_family = AF_INET;        // Протокол (IPv4)

  // Проверка корректности ip
  if (inet_pton(AF_INET, ipa.c_str(), &address.sin_addr) <= 0) {
    cout << "inet_pton error for " << ipa.c_str() << endl;
    throw std::invalid_argument("err"); // Бросаем исключение в случае неправильного адреса
  }

  // Проверка корректности порта данных
  if ( !(data_port.find_first_not_of("0123456789") == std::string::npos) ) {
      cout << "Backlog must be a numeric value" << endl;
      throw std::invalid_argument("err"); // Бросаем исключение в случае неправильного порта
  }

  int diport;
  if ( !(diport = atoi(data_port.c_str())) ) {
    cout << "Backlog must be a numeric value greater than zero" << endl;
    throw std::invalid_argument("err"); // Бросаем исключение в случае неправильного порта
  }
  address.sin_port = htons(diport);

  // Создаём сокет
  if ( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    cout << "Error of socket" << endl;
    throw std::invalid_argument("err"); // Бросаем исключением в случае неудачного создания сокета
  }
}

// Деструктор
conclser::~conclser()
{

}

// Функция работы клиента/сервера
// Главный цикл accept()
int conclser::exec()
{
  return 0;
}
