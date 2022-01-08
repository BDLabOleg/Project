 // сlient.cpp : консольное приложение клиента
 
#include <winsock2.h>  
#include <stdio.h>
#include <conio.h>
#include <io.h>
 
int main()
{
    int rc, size, symbols, y=0, i=1; // rc- проверка соединения, size - позиция при считывании файла, symbols - число считанных символов, 
	                                 //y - ответ от сервера, i - счетчик
    WSADATA wsd;                     // структура WSADATA, в которую записываются данные по реализации сокета
    int iResult;                     
   
   // WSAStartup(0X0101, &wsd);
        
    // Initialize Winsock
    iResult = WSAStartup(0X0101, &wsd);  //инициализирует библиотеку для приложения
	                                     // 0X0101 - указывает старший номер версии библиотеки DLL, который используется
    
      
    if (iResult != 0) {     // проверка, запущен ли Winsock API: если 0 - то запущен 
       printf("WSAStartup failed: %d\n", iResult);     // иначе выводится ошибка
       printf("Error code: %d\n", WSAGetLastError());  // код ошибки инициализаии    
       return 1;
      }    
    
    
    //инициализация сокета для сервера TCP 
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);      
    // SOCK_STREAM - тип канала связи с сокетом, при котором гарантируется доставка байт в порядке их поступления до окончания передачи
    
    //описание структуры для работы сокета по протоколу TCP
    sockaddr_in peer;    
    peer.sin_family = AF_INET;       //  семейство протоколов
    peer.sin_port   = htons(1200);   // номер порта 1200
    peer.sin_addr.S_un.S_addr = inet_addr("192.168.2.104");   // IP адрес сервера
 
    //инициализация и подключение клиента
    rc = connect(s,(struct sockaddr*) &peer, sizeof(peer));   // s - дескриптор свободного сокета,  sockaddr* - указатель на структуру
	  														  // к которой устанавливается соединение -- peer, sizeof(peer) - длина структуры в байтах
       
    while (rc<0) {                                             //Если rc<0, то сервер не готов принимать запросы и файлы
               printf ("Error: server is not ready\n"); 
               getch();
                 }
 
    FILE *f;  // указатель на на файл
    
    f = fopen("file2send.txt","rb");   // открываем файл на чтение
    
    if (!f) printf ("File is not found");   // проверяем, найден ли файл
      else {    // если файл найден
	  
    while(!feof(f)) //пока не достигнут конец файла, передаем файл частями - по 100 байт в буфере
    {
    char bufer[100];  
    
    symbols=fread(bufer,1,100,f);   //в буфер считывается 100 байт из файла, возвращается считанное число символов
    
    size=ftell(f);  //функция возвращает текущую позицию в файле
    
    printf("read symbols: %d, part: %d, pos: %ld \n",symbols,i,size);
    
    send(s,bufer,symbols,0); //передача содержимого буфера на сервер
    
    printf("send symbols: %d, part: %d, pos: %ld \n\n",symbols,i,size);
    i++;
    
    char buf[100]; //буфер для приема сообщений сервера
    
    y = recv(s,buf,sizeof(buf),0); //прием ответа сервера (sizeof(buf) - максимально возможный принятый объем)
                                   // s - сокет, buf - буфер для приема данных
                                   // 0 - флаг
    
    //printf("%s\n", buf);
    }
    fclose (f);   // закрыть файл
    
    };
       
    printf ("Press any key...");
    getch();
       
    // размыкание соединения
    shutdown(s, 2);
    WSACleanup(); // очистка данных после закрытия сокета
    return 0;
}
