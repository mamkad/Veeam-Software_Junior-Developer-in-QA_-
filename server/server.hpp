/*
 * Описание класса сервера
 */
#ifndef SERVER_HPP
#define SERVER_HPP

#include "../conclser/conclser.hpp" // Заголовочный файл базового класса

// Класс сервера
class server : public conclser
{
private:
  // Приватные данные
  int    backlog; // Число разрешённых входных подключений во входной очереди
  string logfile; // Имя логфайла
public:
  // Инициализация
                                 // Принимает
  server(string const& ip,       // Ip адреc
         string const& dport,    // Порт данных
         string const& cport,    // Порт кодов
         int           backlog); // Число входных подключений во входной очереди
  // Деструктор
  ~server();
private:
  // Приватные методы для внутренного функционирования класса
  // Назначить прослушивание данного адресаs
  void set_listen(struct sockaddr_in& address, int& fd);
  // Принять соединение
  bool accept_connect(int fd, int& newfd);
private:
  // Слушать порт кодов (куда приходит идентификатор пользователя)
  void listen_codeport();
  // Прочитать данные из порта кодов
  bool read_codeport(int fd, idcode_t& id);
  // Отправить данные по порту кодов
  bool write_codeport(int fd, idcode_t code);
private:
  // Слушать порт данных
  void listen_dataport();
  // Прочитать данные из порта данных
  int read_dataport(int fd, datapkg& readdata);
  // Отправить данные по порту данных
  bool write_dataport(int fd, idcode_t status);
private:
  // Сохранить в логфайл
  bool savetolog(datapkg& data);
private:
  // Вычислить код для заданного идентификатора
  idcode_t code(idcode_t id);
public:
  // Публичные методы
  // Функция работы сервера
  int exec() override;
};
#endif
