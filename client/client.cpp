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
   cout << "client stops working" << endl;
 }

 // Подключиться к серверу
 bool client::set_connect(struct sockaddr_in& address, int fd)
 {
   if (connect(fd, (struct sockaddr*)&address, sizeof(address)) <= 0)
     return false;
   return true;
 }

 // Прочитать порт кодов
 bool client::read_codeport()
 {
   if (write(code_fd, (char*)&data.id, sizeof(data.id)) <= 0)
     return false;
   if (read (code_fd, (char*)&data.code,  sizeof(data.code)) <= 0)
     return false;
    return true;
 }

 // Записать на порт данных
 bool client::write_dataport()
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

 // Прочитать данные из ввода
 bool client::setdfrmsg(string const& msg)
 {
   data.size = msg.size();
   data.buffer = new char[data.size];
   if (!data.buffer)
    return false;
   memcpy(data.buffer, msg.c_str(), msg.size());
   return true;
 }

 // Получить код от сервера
 bool client::getcode()
 {
   // Устнавливаем соединение с портом кодов
   if (!set_connect(code_address, code_fd)) {
     cout << "Connect error" << endl;
     return false;
   }

   cout << "connection to code port is established" << endl;
   cout << endl << "enter the ID (Must be a positive integer) : ";
   // Вводим id
   if (!(cin >> data.id)) {
     cout << "wrong input" << endl;
     return false;
   }
   // Пытаемся отправить id  серверу и получить код
   if (!read_codeport()) {
      cout << "error of get code" << endl;
      return false;
   } else {
     cout << "code received: "<< data.code << endl;
   }
   // Закрываем соединение
   cls_connect(code_fd);
   return true;
 }

 // Отправить данные
void client::sendmsg()
{
  // Устнавливаем соединение с портом данных
  if (!set_connect(data_address, data_fd)) {
    cout << "Connect error" << endl;
    return;
  }

  cout << endl << "send data from a file - 0" << endl << "enter a message manually - 1" << endl << endl;

  int answer;
  if (!(cin >> answer)) {
    cout << "wrong input" << endl;
    // Закрываем соединение
    cls_connect(data_fd);
    return;
  }
  cin.clear();

  switch(answer) {
    case 0:
    {
      cout << "enter the filename : ";
      string fname;
      cin >> fname;

      if (!openfile(fname, data)) {
        cout << "error of open file" << endl;
        return;
      }
      cout << "file uploaded" << endl;
      break;
    }
    case 1:
    {
      cout << "enter the msg : ";
      string msg;
      getline(cin, msg);
      if (!setdfrmsg(msg)) {
        cout << "error of ge msg" << endl;
        return;
      }
      break;
    }
    default:
      cout << "unknown answer" << endl;
      return;
  }
  // Пытаемся отправить данные  cерверу
  if(!write_dataport()) {
    cout << "error of send data"<< endl;
  } else {
      delete [] data.buffer;
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
