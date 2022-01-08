 // �lient.cpp : ���������� ���������� �������
 
#include <winsock2.h>  
#include <stdio.h>
#include <conio.h>
#include <io.h>
 
int main()
{
    int rc, size, symbols, y=0, i=1; // rc- �������� ����������, size - ������� ��� ���������� �����, symbols - ����� ��������� ��������, 
	                                 //y - ����� �� �������, i - �������
    WSADATA wsd;                     // ��������� WSADATA, � ������� ������������ ������ �� ���������� ������
    int iResult;                     
   
   // WSAStartup(0X0101, &wsd);
        
    // Initialize Winsock
    iResult = WSAStartup(0X0101, &wsd);  //�������������� ���������� ��� ����������
	                                     // 0X0101 - ��������� ������� ����� ������ ���������� DLL, ������� ������������
    
      
    if (iResult != 0) {     // ��������, ������� �� Winsock API: ���� 0 - �� ������� 
       printf("WSAStartup failed: %d\n", iResult);     // ����� ��������� ������
       printf("Error code: %d\n", WSAGetLastError());  // ��� ������ ������������    
       return 1;
      }    
    
    
    //������������� ������ ��� ������� TCP 
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);      
    // SOCK_STREAM - ��� ������ ����� � �������, ��� ������� ������������� �������� ���� � ������� �� ����������� �� ��������� ��������
    
    //�������� ��������� ��� ������ ������ �� ��������� TCP
    sockaddr_in peer;    
    peer.sin_family = AF_INET;       //  ��������� ����������
    peer.sin_port   = htons(1200);   // ����� ����� 1200
    peer.sin_addr.S_un.S_addr = inet_addr("192.168.2.104");   // IP ����� �������
 
    //������������� � ����������� �������
    rc = connect(s,(struct sockaddr*) &peer, sizeof(peer));   // s - ���������� ���������� ������,  sockaddr* - ��������� �� ���������
	  														  // � ������� ��������������� ���������� -- peer, sizeof(peer) - ����� ��������� � ������
       
    while (rc<0) {                                             //���� rc<0, �� ������ �� ����� ��������� ������� � �����
               printf ("Error: server is not ready\n"); 
               getch();
                 }
 
    FILE *f;  // ��������� �� �� ����
    
    f = fopen("file2send.txt","rb");   // ��������� ���� �� ������
    
    if (!f) printf ("File is not found");   // ���������, ������ �� ����
      else {    // ���� ���� ������
	  
    while(!feof(f)) //���� �� ��������� ����� �����, �������� ���� ������� - �� 100 ���� � ������
    {
    char bufer[100];  
    
    symbols=fread(bufer,1,100,f);   //� ����� ����������� 100 ���� �� �����, ������������ ��������� ����� ��������
    
    size=ftell(f);  //������� ���������� ������� ������� � �����
    
    printf("read symbols: %d, part: %d, pos: %ld \n",symbols,i,size);
    
    send(s,bufer,symbols,0); //�������� ����������� ������ �� ������
    
    printf("send symbols: %d, part: %d, pos: %ld \n\n",symbols,i,size);
    i++;
    
    char buf[100]; //����� ��� ������ ��������� �������
    
    y = recv(s,buf,sizeof(buf),0); //����� ������ ������� (sizeof(buf) - ����������� ��������� �������� �����)
                                   // s - �����, buf - ����� ��� ������ ������
                                   // 0 - ����
    
    //printf("%s\n", buf);
    }
    fclose (f);   // ������� ����
    
    };
       
    printf ("Press any key...");
    getch();
       
    // ���������� ����������
    shutdown(s, 2);
    WSACleanup(); // ������� ������ ����� �������� ������
    return 0;
}
