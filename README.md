# bjo

Данная библиотека призвана упростить процесс написания асинхронных приложений,
использующих
```boost::asio``` и ```boost::beast```. Использование bjo делает c++ код,
решающий типовые высокоуровневые задачи, более
читабельным и
простым для восприятия, без ущерба производительности.

Библиотека bjo предоставляет удобные и эффективные абстракции для написания
асинхронных приложений. Приоритетный способ
написания асинхронного кода в bjo - это корутины,
которые вошли в стандарт языка C++ в 2020 году. Предполагается, что пользователь
библиотеки будет использовать именно
их, в процессе написания кода, несмотря на то, что
альтернативные подходы к написанию асинхронного кода тоже поддерживаются.

# Установка

Чтобы использовать библиотеку в своем C++ проекте, ее нужно подключить как CMake
подпроект:

```cmake
add_subdirectory(bjo)
include_directories(bjo/include)
target_link_libraries(${PROJECT_NAME} bjo)
```

И установить зависимости с помощью ```conan```.

```shell
conan install <путь до conanfile.txt, который лежит в папке с библиотекой>
```

## Что такое корутины?

Корутина - это функция, которая может приостановить свое выполнение на какое-то
время или до наступления какого-то
события,
а после возобновиться с того места, где была приостановлена.

Корутины позволяют писать асинхронный код, который выглядит как синхронный. Это
очень круто, честно.

## Как написать корутину в boost::asio

* Чтобы ваша функция стала корутиной, нужно чтобы она
  возвращала ```boost::asio::awaitable<ТИП ДАННЫХ>``` (на самом деле много чего
  другого, но на базовом уровне этого хватает).
  После этого, в ее теле станет возможно использовать такие ключевые слова
  как ```co_return``` и ```co_await```. Но! уже
  нельзя использовать
  обычный ```return```.
* Чтобы запустить свою корутину, ее нужно создать(заспавнить) в executor-e. Для
  этого нужно написать следующий код:

```c++
boost::asio::co_spawn(
        экземпляр io_context, или другой executor из asio,
        вызов вашей корутины, 
        указать completion_token  
);
```

* Про completion_token-ы написано здесь:
  https://www.boost.org/doc/libs/develop/doc/html/boost_asio/overview/model/completion_tokens.html

Для работы с c++20 корутинами используются 2 токена:

* ```boost::asio::use_awaitable``` - для того, чтобы иметь возможность отпустить
  контекст, до тех пор, пока задача не
  будет выполнена
* ```boost::asio::detached``` - отправить корутину в "свободное плавание".
  Заспавнить ее в исполнителе и никак не
  дожидаться ее завершения

## Основные компоненты фреймворка:

### bjo::TaskProcessor

Важнейшим классом библиотеки является ```TaskProcessor```, который представляет
собой обертку
над ```boost::asio::io_context```, позволяющую исполнять асинхронные задачи в
собственном пуле потоков, не блокируя
вызвавший поток.

#### Зачем это нужно?

Иногда в асинхронном приложении может возникать необходимость запустить сложные
CPU-bound вычисления, или обратиться к
БД.
Возникает проблема. Например, если запустить такую операцию в потоке,
принимающем новые соединения (предположим, что мы
пишем http-сервер) мы заблокируемся до окончания тяжелой операции.
Это чревато тем, что мы перестанем обслуживать клиентов на какое-то время.

Исполнение задач в разных TaskProcessor-ах решает данную проблему. Пользователь
библиотеки может определить
TaskProcessor-ы на типы решаемых задач и
запускать таски(корутины, коллбэки, футуры) в соответствующих Executor-ах.

#### Что ещё?

Пользователь может выбрать то, каким образом запустить асинхронную задачу. Можно
использовать как стандартные корутины,
так и ```std::future``` или классические callback-и.

#### Пример использования:

```c++
// Run task in detached mode. Don't wait completion.
 task_processor_.ProcessTask(Listen(), asio::detached); 

// When we need to perform task in another executor and fetch results.
auto result =  co_await task_processor_.ProcessTask(Coro(), asio::use_awaitable);

// using futures
std::future<Тип> res = task_processor_.ProcessTask(FutureTask(), asio::use_future);
// to get results type res.get() 
```

### bjo::http::Server

bjo предоставляет пользователю удобную и эффективную высокоуровневую абстракцию,
позволяющую реализовывать http серверы.

#### Как этим пользоваться?

Для того чтобы запустить http::Server в bjo нужно:

* Задать обработчики на соответствующие таргеты
* Зарегистрировать их в сервере
* Запустить сервер

#### ПРИМЕР СОЗДАНИЯ СЕРВЕРА:

```c++
namespace asio = boost::asio;
namespace http = bjo::http;

  http::Server server(http::server::DefaultConfig());
  
  server.RegisterHandlers()
      (http::METHODS::GET, http::server::Route<"/">(), AsClass{})
      (http::METHODS::GET | http::METHODS::POST, http::server::Route<"/users/id=[0-9]+">(), AsFunc)
      (http::METHODS::GET, http::server::Route<"/values">(),
        [](const http::Request& req) -> asio::awaitable<http::Response> {
          // Handler can be implemented as lambda
        }
      )
  ;
  
  server.Serve(); // starts server

```

#### ВАЖНО

* Обработчик http запроса должен иметь сигнатуру
  вида ```asio::awaitable< http::Response > (const http::Request& req)```
* Объект типа http::server::Route принимает шаблонным параметром регулярное
  выражение, описывающее паттерн, с которым будет сравниваться роут из заголовка

#### ПРИМЕР ОБРАБОТЧИКА

```c++
namespace asio = boost::asio;
namespace http = bjo::http;

// As Function
asio::awaitable<http::Response> AsFunction(const http::Request& req);


// As Lambda, that returns asio::awaitable<bjo::http::Response>
[](const http::Request& req) -> asio::awaitable<http::Response> {} 


// As Class
struct Handler {
    asio::awaitable<http::Response> operator()(const http::Request& req) {} 
};
```

### bjo::http::Request и bjo::http::Response

Классы ```bjo::http::Request``` и ```bjo::http::Response``` - это alias-ы для
реализаций из boost::beast. 

