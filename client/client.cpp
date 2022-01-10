/*
 * Реализация класса клиента
 */
#include "client.hpp"
#include <unistd.h>    // close/read/write
#include <iostream>    // cin/cout/endl
#include <fstream>     // ifstream
#include <cstring>     // memcopy

using std::cin;
using std::cout;
using std::endl;

// Конструктор
 client::client(string const& ip, string const& dport, string const& cport)
 : conclser(ip,  dport, cport)
 {
   cout << "сlient was successfully created" << endl;
 }

 // Деструктор
 client::~client()
 {
   cout << endl << "client stops working" << endl;
 }

 // Подключиться к серверу
 bool client::set_connect(struct sockaddr_in& address, int fd)
 {
   if (connect(fd, (struct sockaddr*)&address, sizeof(address)) < 0)
     return false;
   return true;
 }

 // Прочитать порт кодов
 bool client::read_codeport(idcode_t& id, idcode_t& code)
 {
   if (write(code_fd, (char*)&id, sizeof(id)) <= 0)
     return false;
   if (read (code_fd, (char*)&code,  sizeof(code)) <= 0)
     return false;
    return true;
 }

 // Записать на порт данных
 bool client::write_dataport(datapkg&  data)
 {
   if (write(data_fd, (char*)&data.id,   sizeof(data.id)) <= 0)
    return false;
   if (write(data_fd, (char*)&data.code, sizeof(data.code)) <= 0)
    return false;
   if (write(data_fd, (char*)&data.size, sizeof(data.size)) <= 0)
    return false;
   if ((write(data_fd, data.buffer,       data.size) <= 0))
    return false;

   return true;
 }

 // Прочитать порт данных
 bool client::read_dataport(int& ret_code)
 {
   if (read (data_fd, (char*)&ret_code,  sizeof(ret_code)) <= 0)
     return false;
  return true;
 }

 // Прочитать данные из файла
 bool client::openfile(string const& fname, datapkg& data)
 {
   std::ifstream fin(fname, std::ios_base::binary);
   if (!fin)
     return false;

   fin.seekg(0, std::ios::end);

   data.size = fin.tellg();
   data.buffer = new char[data.size];
   if (!data.buffer)
    return false;

   fin.seekg(0, std::ios::beg);
   fin.read(&data.buffer[0], data.size);
   fin.close();
   return true;
 }

 // Получить код от сервера
 bool client::getcode()
 {
   // Устнавливаем соединение с портом кодов
   if (!set_connect(code_address, code_fd)) {
     cout << endl << "Connect error" << endl;
     return false;
   }

   cout << "connection to code port is established" << endl;
   cout << endl << "enter the ID (Must be a positive integer) : ";
   // Вводим id
   idcode_t id, code;
   if (!(cin >> id)) {
     cout << endl << "wrong input" << endl;
     return false;
   }
   // Пытаемся отправить id  серверу и получить код
   if (!read_codeport(id, code)) {
      cout << endl << "error of get code" << endl;
      return false;
   } else {
     cout << endl << "code received: "<< code << endl;
   }
   // Закрываем соединение
   cls_connect(code_fd);
   return true;
 }

 // Отправить данные
void client::sendmsg()
{
  // Устанавливаем соединение с портом данных
  if (!set_connect(data_address, data_fd)) {
    cout << endl << "connect error" << endl;
    return;
  }
  cout << endl << "======data entry======" << endl;
  datapkg data;  // Id клиента, Код полученный от сервера, буфер данных)
  // Ввод имени файла
  cout << endl << "enter the file name to send : ";
  string fname;
  cin >> fname;
  cout << endl;
  // Открываем файл
  if (!openfile(fname, data)) {
    cout << endl << "error of open file" << endl;
    return;
  }
  cout << "file uploaded" << endl;
  // Ввод id и code
  cout << endl << "enter the your id and code that you got from server separated by a space: ";
  if (!(cin >> data.id >> data.code)) {
    cout << endl << "wrong input" << endl;
    return;
  }
  // Пытаемся отправить данные  cерверу
  if (!write_dataport(data)) {
    cout << endl <<  "error of send data"<< endl;
  } else {
      delete [] data.buffer;
  }

  // Читаем ответ от сервера, совпали ли коды
  int ret_code;
  if (!read_dataport(ret_code)) {
    cout << endl <<  "error of read data port"<< endl;
  } else {
    if (ret_code == SUCCESS) {
      cout << endl << "server has successfully received the file data" << endl;
    } else {
      cout << endl << "codes didn't match" << endl;
    }
  }
  // Закрываем соединение
  cls_connect(data_fd);
}

 // Функция работы клиента
 int client::exec()
 {
   if(getcode()) {
     sendmsg();
   }
   return 0;
 }
