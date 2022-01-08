// server.cpp :  консольное приложение сервера
 
#include <winsock2.h>  // библиотека для работы с сокетами
// #include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>

using namespace std;
 
int  main()
{
    int rc, i=1;   // rc- проверка соединения, i - счетчик
    WSADATA wsd;       // структура WSADATA, в которую записываются данные по реализации сокета
    WSAStartup(0X0101, &wsd);   //инициализирует библиотеку для приложения
	                            // 0X0101 - указывает старший номер версии библиотеки DLL, который используется
    
    FILE *f;   // указатель на на файл
    
    //инициализация сокета для в случае сервера/клиента TCP. Интернет, тип сокета TCP, протокол
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    
    // SOCK_STREAM - тип канала связи с сокетом, при котором гарантируется доставка байт в порядке их поступления до окончания передачи
	    
    //структура для инициализация сервера
    sockaddr_in myCom;
    myCom.sin_family = AF_INET;  // установить семейство адресов
    myCom.sin_port   = htons(1200);   //установить порт 1200
    myCom.sin_addr.S_un.S_addr = inet_addr("192.168.2.104"); // IP адрес сервера
    
    
    struct in_addr inaddr;      // структура для преобразования и вывода адреса клиента в виде символьной переменной strAdd2
    inaddr.S_un.S_addr = myCom.sin_addr.S_un.S_addr;
    char *strAdd2 = inet_ntoa(inaddr);
 
    rc = bind(s,(struct sockaddr*)&myCom,sizeof(myCom));  // связвает сокет с конкретным адресом
                                                          // s - дескриптор свободного сокета,  sockaddr* - указатель на структуру
	  													  // к которой устанавливается соединение -- myCom, sizeof(peer) - длина структуры в байтах
    
    
    if (rc<0) printf ("Error: server %s", strAdd2," is not ready...");    //Если rc<0, то клиент не готов принимать и отправлять запросы и файлы
       else { 
	   printf ("Server %s", strAdd2);
	   printf (" is ready...\n");
       }
       
	
    listen(s, 5);   // подготовка сокета s к приему входящих соединений, 5 - максимальное число установденных соединений 
 
    //принятие соединения, вызов сокета
    SOCKET s1; 
    
    if ((s1 = accept(s, (struct sockaddr*)&myCom, NULL)) ==1) printf ("Error: socket is not ready...");  // принятие запроса на установление соединения
    
 
    while (true)
    {
    f = fopen("filerec.txt","ab+");   // открыть файл на запись
    char buf[100];     // буфер на 100 байт
    int r = recv(s1, buf, sizeof(buf), 0);  //прием файла от клиента. Функция возвращает число считанных байтов 
    if (r <= 0) //если нет данных

    {
      cout<<"0 bytes"<<endl;
      cout<<"Error: "<<WSAGetLastError()<<endl; //вывод кода ошибки
      return false; //выход из цикла
    }
    
    //printf (buf);
    
    fwrite(buf,1,r,f);   // записать полученные данные из буфера в файл, r - число считанных файлов
    cout<<"Received bytes: "<<r<<", part: "<<i<<endl;
    send(s1,"ready ro receive",6*sizeof(char),0);  // отправка информации клиенту о полученных данных
  
    i++;
    fclose (f);   // закрытие файла
     //  cout <<"Ready to receive..."<<endl;
    }
    
	//размыкание соединения
    shutdown(s1, 2);
    WSACleanup();  // очистка данных после закрытия сокета
    return 0;
}
