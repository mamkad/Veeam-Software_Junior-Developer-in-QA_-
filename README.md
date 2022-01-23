# Клиент-серверная система
## Задание
Написать клиент-серверную систему, работающую по следующему алгоритму:

1. Сервер держит открытыми порты 8000 и 8001.

2. При запуске клиент выбирает для себя уникальный идентификатор.

3. Клиент подключается к серверу к порту 8000, передает ему свой идентификатор и получает от сервера уникальный код.

4. Клиент подключается к серверу к порту 8001 и передает произвольное текстовое сообщение, свой идентификатор и код, полученный на шаге 2.

5. Если переданный клиентом код не соответствует его уникальному идентификатору, сервер возвращает клиенту сообщение об ошибке.

6. Если код передан правильно, сервер записывает полученное сообщение в лог.

Сервер должен поддерживать возможность одновременной работы с хотя бы 50 клиентами.
Для реализации взаимодействия между сервером и клиентом системы допускается (но не требуется) использование высокоуровнего протокола (например, HTTP).
## Решение
**Сборка и запуск**

Для сборки достаточно выполнить makefile, исполняемые файлы будут созданы в папке bin.

Программы должны собираться на Ubuntu.

Чтобы запустить сервер, нужно запустить исполняемый файл server в папке bin. Чтобы запустить клиента и отправить сообщение, в той же папке нужно запустить client.

Перед запуском клиента сервер должен быть запущен.

После запуска клиента, будет предложено ввести id.

Текстовы данные клиент берёт из текствого файла, его имя будет предложенно ввести.

Логфайл сервера будет создан в папке с сервером.

**Составные части программы**

Код сервера [server](https://github.com/mamkad/Veeam-Software_Junior-Developer-in-QA_-/tree/main/server)

Код клиента [client](https://github.com/mamkad/Veeam-Software_Junior-Developer-in-QA_-/tree/main/client)

Базовый класс клиента и сервера [conclser](https://github.com/mamkad/Veeam-Software_Junior-Developer-in-QA_-/tree/main/conclser)
