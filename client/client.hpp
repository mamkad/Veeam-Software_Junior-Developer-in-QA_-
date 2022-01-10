/*
 * Описание класса клиента
 */
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../conclser/conclser.hpp" // Заголовочный файл базового класса

// Класс клиента
class client : public conclser
{
private:
  datapkg  data;        // Id клиента, Код полученный от сервера, буфер данных
public:
  // Инициализация
                                // Принимает
  client(string const& ip,      // Ip адреc
         string const& dport,   // Порт данных
         string const& cport);  // Порт кодов
  // Деструктор
  ~client();
private:
  // Приватные методы для внутренного функционирования класса
  // Подключиться к серверу
  bool set_connect(struct sockaddr_in& address, int fd);
  // Прочитать данные из файла
  bool openfile(string const& fname, datapkg& data);
  // Прочитать данные из ввода
  bool setdfrmsg(string const& msg);
private:
  // Прочитать порт кодов
  bool read_codeport();
  // Записать на порт данных
  bool write_dataport();
  // Получить код от сервера
  bool getcode();
  // Отправить данные
  void sendmsg();
public:
  // Публичные методы
  // Функция работы клиента
  int exec() override;
};
#endif
