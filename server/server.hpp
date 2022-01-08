/*
 * Описание класса сервера
 */
#ifndef SERVER_HPP
#define SERVER_HPP

#include "../conclser/conclser.hpp"

class server : public conclser
{
private:
  // Приватные данные
  string code_port; // Порт кодов
  int backlog;      // Число разрешённых входных подключений во входной очереди
  struct sockaddr_in code_address; // Структура адреса
public:
  // Инициализация
  // Принимает Ip адреc, порт данных и порт кодов
  server(string const& ip, string const& dport, string const& cport, int backlog);
private:
  void listen_codeport();
  void listen_dataport();
public:
  int exec() override;
};
#endif
