#include "server.hpp"

#include <iostream>    // cin/cout
#include <exception>   // invalid_argument

using std::cout;
using std::endl;

// Конструктор
server::server(string const& ip, string const& dport, string const& cport, int blog)
  : conclser(ip,  dport), code_port(cport), backlog(blog)
{
  memset(&code_address, 0, sizeof(code_address)); // Заполняем address нулями
  code_address.sin_family = AF_INET;              // Протокол (IPv4)

  // Проверка корректности ip
  if (inet_pton(AF_INET, ipa.c_str(), &code_address.sin_addr) <= 0) {
    cout << "inet_pton error for " << ipa.c_str() << endl;
    throw std::invalid_argument("err"); // Бросаем исключением в случае неправильного адреса
  }

  if (bind(fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    cout << "error of bind" << endl;
    throw std::invalid_argument("err"); // Бросаем исключением в случае неудачи bind
  }

  if (listen(fd, backlog) < 0) {
    cout << "error of bind" << endl;
    throw std::invalid_argument("err"); // Бросаем исключением в случае неудачи listen
  }
}

void server::listen_codeport()
{
  for(;;) {

  }
}

void server::listen_dataport()
{
  for(;;) {

  }
}

int server::exec()
{

  return 0;
}
