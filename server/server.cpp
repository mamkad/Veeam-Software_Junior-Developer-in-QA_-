#include "server.hpp"

#include <iostream>    // cin/cout
#include <exception>   // invalid_argument
#include <thread>      // thread
#include <unistd.h>    // read/write
#include <fstream>     // ofstream
#include <chrono>      // Для даты
#include <sstream>     // stringstream
#include <ctime>   // localtime
#include <iomanip> // put_time

using std::cout;
using std::endl;

// Конструктор
server::server(string const& ip, string const& dport, string const& cport, int blog)
  : conclser(ip,  dport, cport), backlog(blog), logfile("server_log.txt")
{
  try {
    set_listen(code_address, code_fd);
    set_listen(data_address, data_fd);
  } catch(...) {
    throw std::invalid_argument("err"); // Бросаем исключением в случае неудачной инициализации
  }
  cout << "server was successfully created" << endl;
}

// Деструктор
server::~server()
{
  cout << "server stops working" << endl;
}

// Назначить прослушивание данного адреса
void server::set_listen(struct sockaddr_in& address, int& fd)
{
  // Связываем дескриптор и адрес
  if (bind(fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    cout << "error of bind" << endl;
    throw std::invalid_argument("err"); // Бросаем исключением в случае неудачи bind
  }
  // Назначаем слушать данный дескриптор
  if (listen(fd, backlog) < 0) {
    cout << "error of bind" << endl;
    throw std::invalid_argument("err"); // Бросаем исключением в случае неудачи listen
  }
}

// Принять соединение
bool server::accept_connect(int fd, int& newfd)
{
  if ( (newfd = accept(fd, (struct sockaddr*)NULL, NULL)) < 0 )
    return false;
  return true;
}

// Слушать порт кодов (куда приходит идентификатор пользователя)
void server::listen_codeport()
{
  for(;;) {
    // Принимаем подключения по порту кодов
    // Проверяем на успешное завершение
    int connfd;
    if(!accept_connect(code_fd, connfd)) {
      cout << "error of accept" << endl;
      continue;
    }
    cout << "got connecton to code port" << endl;
    // Это порождённые процессы
		// Сервер сможет обрабатывать несколько соединений одновременно
    if (!fork()) {
      cls_connect(code_fd); // Его слушать не нужно
      // Читаем id, который отправил пользователь
      // Проверяем на успешное завершение
      idcode_t id;
      if (!read_codeport(connfd, id)) {  // Ошибка чтения
        cout << "error of read" << endl;
        exit(1);  // Завершаем дочерний процесс
      }
      //Вычисляем код для идентификатора
      idcode_t codeusr = code(id);
      // Отправляем клиенту код
      // Проверяем на успешное завершение
      if(!write_codeport(connfd, codeusr)) { // Ошибка записи
        cout << "error of write" << endl;
        exit(1); // Завершаем дочерний процесс
      }
      // Закрываем сокет
			// Завершаем процесс
      cls_connect(connfd);
      exit(0);
    }
    cls_connect(connfd);  // Родителю это не нужно
  }
}

// Прочитать данные из порта кодов
bool server::read_codeport(int fd, idcode_t& id)
{
  if (read(fd, (char*)&id, sizeof(id)) <= 0)
    return false;
  return true;
}

// Отправить данные по порту кодов
bool server::write_codeport(int fd, idcode_t code)
{
  if(write(fd, (char*)&code, sizeof(code)) <= 0)
    return false;
  return true;
}

// Слушать порт данных
void server::listen_dataport()
{
  for(;;) {
    // Принимаем подключения по порту данных
    // Проверяем на успешное завершение
    int connfd;
    if(!accept_connect(data_fd, connfd)) {
      cout << "error of accept" << endl;
      continue;
    }
    cout << "got connecton to data port" << endl;
    // Это порождённые процессы
		// Сервер сможет обрабатывать несколько соединений одновременно
    if (!fork()) {
      cls_connect(data_fd); // Его слушать не нужно
      // Считываем данные от клиента
      // Проверяем на успешное завершение
      datapkg readdata;
      int rv = read_dataport(connfd, readdata);
      // Проверяем, что чтение прошло успешно
      if (rv == -1) {
        cout << "error of read dataport" << endl;
        exit(1);
      }
      //  Проверяем, что проверка кода пользователя прошла успешно
      if (rv == -2) {
        cout << "codes don't match" << endl;
        if(!write_dataport(connfd, WRONGCODE)) {
          cout << "error of write" << endl;
          exit(1);
        }
      } else {
        // Всё прошло успешно
        // Записываем данные в логфайл
        cout << "data received successfully" << endl;
        // Записываем в лог и проверяем, что удалось записать
        if(!savetolog(readdata)) {
          cout << "error of open logfile: "<< logfile << endl;
          exit(1);
        }
        // Отправляем клиенту информацию о результате соединения
        if(!write_dataport(connfd, SUCCESS)) {
          cout << "error of write" << endl;
          exit(1);
        }
      }
      // Закрываем сокет
			// Завершаем процесс
      cls_connect(connfd);
      exit(0);
    }
    cls_connect(connfd);
  }
}

// Прочитать данные из порта данных
int server::read_dataport(int fd, datapkg& readdata)
{
  // Считываем id и code от пользователя
  if (read(fd, (char*)&readdata.id, sizeof(readdata.id)) <= 0)
    return -1;
  if (read(fd, (char*)&readdata.code, sizeof(readdata.code)) <= 0)
    return -1;
  // Проверяем правильность кода
  if (readdata.code != code(readdata.id))
    return -2;
  // Считываем размер буфера
  if (read(fd, (char*)&readdata.size, sizeof(readdata.size)) <= 0)
    return -1;
  // Выделяем память под буфер
  readdata.buffer = new char[readdata.size];
  if (!readdata.buffer)
    return -1;
  // Считываем данные в буфер
  if (read(fd, readdata.buffer, readdata.size) <= 0) {
      delete [] readdata.buffer;
      return -1;
  }

  return 0;
}

// Отправить данные о результате по порту данных
bool server::write_dataport(int fd, idcode_t status)
{
  if(write(fd, (char*)&status, sizeof(status)) <= 0)
    return false;
  return true;
}

// Функция для получения даты и времени
static string getdate()
{
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
  return ss.str();
}

// Сохранить в логфайл
bool server::savetolog(datapkg& data)
{
  // Открываем файл
  std::ofstream fout(logfile, std::ios_base::binary | std::ios_base::app);
  // Проверяем, что прошло успешно
  if (!fout) {
      delete [] data.buffer;
      return false;
  }
  // Записываем данные в файл
  fout << "..:::==============================================================================================:::.." << endl << endl;
  fout << "\t\t\tconnection" << endl << endl;
  fout << "\t\t\tuser id: " << data.id << endl;
  fout << "\t\t\tuser code: " << data.code << endl;
  fout << "\t\t\ttime: " << getdate() << endl << endl;
  fout << ".:msg:." << endl;
  fout << "****************************************************************************************************" << endl << endl;
  fout.write(data.buffer, data.size);
  fout << endl << "****************************************************************************************************" << endl;
  fout << ".:end:." << endl;
  fout << "..:::==============================================================================================:::.." << endl << endl << endl;
  fout.close();
  delete [] data.buffer;
  return true;
}

// Вычислить код для заданного идентификатора
server::idcode_t server::code(idcode_t id)
{
  return 3 * (id - 1)  + (id / 2);
}

// Публичные методы
// Функция работы сервера
int server::exec()
{
  std::thread codeport_thread(&server::listen_codeport, this); // Создаём отдельный поток для прослушивания порта кодов
  std::thread dataport_thread(&server::listen_dataport, this); // Создаём отдельный поток для прослушивания порта данных
  // Ожидание завершения потоков
  codeport_thread.join();
  dataport_thread.join();
  return 0;
}
