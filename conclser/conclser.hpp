/*
 * Описание базового класса для классов сервера и клиента
 */
#ifndef CONCLSER_HPP
#define CONCLSER_HPP

#include <string>       // string
#include <vector>       // vector
#include <netinet/in.h> // struct sockaddr_in

// чтобы не писать std
using std::string;
using std::vector;

// Базовый класс для клиента и для сервера
// conclser - connect client server
class conclser
{
protected:
  // Приватные данные
  string             ip;        // Ip адрес
  string             data_port; // Порт данных
  struct sockaddr_in address;   // Структура адреса
  int                fd;        // Файловый дескриптор сокета
  vector<char>       buffer;    // Буфера данных
public:
  // Инициализация
  // Принимает Ip адрес и Порт данных
  conclser(string const& ipa, string const& dport); // Конструктор
  virtual ~conclser();                              // Деструктор
public:
  // Публичные методы
  // Функция работы клиента/сервера
  // Главный цикл accept()
  virtual int exec();
};
#endif
