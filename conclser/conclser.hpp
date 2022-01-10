/*
 * Описание базового класса для классов сервера и клиента
 */
#ifndef CONCLSER_HPP
#define CONCLSER_HPP

#include <string>       // string
#include <netinet/in.h> // struct sockaddr_in
#include <memory>       // shared_ptr

// чтобы не писать std
using std::string;
using std::shared_ptr;
using std::make_shared;

// Базовый класс для клиента и для сервера
// conclser - connect client server
class conclser
{
protected:
  // Приватные данные
  string             ip;             // Ip адрес
  string             data_port;      // Порт данных
  string             code_port;      // Порт кодов
  struct sockaddr_in data_address;   // Структура адреса порта данных
  struct sockaddr_in code_address;   // Структура адреса порта кодов
  int                data_fd;        // Дескриптор сокета порта данных
  int                code_fd;        // Дескриптор сокета порта кодов
protected:
  // Приватные общие типы
  typedef uint16_t idcode_t; // Псевдоним для типа вводимого пользователем идентификатора и возвращаемого сервером кода
  // Структура пакета данных
  struct datapkg{
    idcode_t              id;  // Id клиента
    idcode_t            code;  // Код клиента
    uint                size;  // Размер буфера текстовых данных
    char* buffer;              // Буфер данных
  };
  // константы
  enum {SUCCESS = 0, WRONGCODE = 1, ERROR = 2};
public:
  // Инициализация
                                 // Принимает
  conclser(string const& ipa,    // Ip адрес
           string const& dport,  // Порт данных
           string const& cport); // Порт кодов
  // Деструктор
  virtual ~conclser();
private:
  // Приватные методы для внутренного функционирования класса
  // Инициализация структуры адреса
  void set_addres(
           struct sockaddr_in& address, // Адрес
           string const&       port);   // Порт
  // Создание сокетов
  void create_socket(int& fd);
public:
  // Публичные методы
  // Закрыть соединение
  void cls_connect(int fd);
  // Функция работы клиента/сервера
  // Главный цикл accept()
  virtual int exec();
};
#endif
