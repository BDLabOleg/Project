// server.cpp :  ���������� ���������� �������
 
#include <winsock2.h>  // ���������� ��� ������ � ��������
// #include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>

using namespace std;
 
int  main()
{
    int rc, i=1;   // rc- �������� ����������, i - �������
    WSADATA wsd;       // ��������� WSADATA, � ������� ������������ ������ �� ���������� ������
    WSAStartup(0X0101, &wsd);   //�������������� ���������� ��� ����������
	                            // 0X0101 - ��������� ������� ����� ������ ���������� DLL, ������� ������������
    
    FILE *f;   // ��������� �� �� ����
    
    //������������� ������ ��� � ������ �������/������� TCP. ��������, ��� ������ TCP, ��������
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    
    // SOCK_STREAM - ��� ������ ����� � �������, ��� ������� ������������� �������� ���� � ������� �� ����������� �� ��������� ��������
	    
    //��������� ��� ������������� �������
    sockaddr_in myCom;
    myCom.sin_family = AF_INET;  // ���������� ��������� �������
    myCom.sin_port   = htons(1200);   //���������� ���� 1200
    myCom.sin_addr.S_un.S_addr = inet_addr("192.168.2.104"); // IP ����� �������
    
    
    struct in_addr inaddr;      // ��������� ��� �������������� � ������ ������ ������� � ���� ���������� ���������� strAdd2
    inaddr.S_un.S_addr = myCom.sin_addr.S_un.S_addr;
    char *strAdd2 = inet_ntoa(inaddr);
 
    rc = bind(s,(struct sockaddr*)&myCom,sizeof(myCom));  // �������� ����� � ���������� �������
                                                          // s - ���������� ���������� ������,  sockaddr* - ��������� �� ���������
	  													  // � ������� ��������������� ���������� -- myCom, sizeof(peer) - ����� ��������� � ������
    
    
    if (rc<0) printf ("Error: server %s", strAdd2," is not ready...");    //���� rc<0, �� ������ �� ����� ��������� � ���������� ������� � �����
       else { 
	   printf ("Server %s", strAdd2);
	   printf (" is ready...\n");
       }
       
	
    listen(s, 5);   // ���������� ������ s � ������ �������� ����������, 5 - ������������ ����� ������������� ���������� 
 
    //�������� ����������, ����� ������
    SOCKET s1; 
    
    if ((s1 = accept(s, (struct sockaddr*)&myCom, NULL)) ==1) printf ("Error: socket is not ready...");  // �������� ������� �� ������������ ����������
    
 
    while (true)
    {
    f = fopen("filerec.txt","ab+");   // ������� ���� �� ������
    char buf[100];     // ����� �� 100 ����
    int r = recv(s1, buf, sizeof(buf), 0);  //����� ����� �� �������. ������� ���������� ����� ��������� ������ 
    if (r <= 0) //���� ��� ������

    {
      cout<<"0 bytes"<<endl;
      cout<<"Error: "<<WSAGetLastError()<<endl; //����� ���� ������
      return false; //����� �� �����
    }
    
    //printf (buf);
    
    fwrite(buf,1,r,f);   // �������� ���������� ������ �� ������ � ����, r - ����� ��������� ������
    cout<<"Received bytes: "<<r<<", part: "<<i<<endl;
    send(s1,"ready ro receive",6*sizeof(char),0);  // �������� ���������� ������� � ���������� ������
  
    i++;
    fclose (f);   // �������� �����
     //  cout <<"Ready to receive..."<<endl;
    }
    
	//���������� ����������
    shutdown(s1, 2);
    WSACleanup();  // ������� ������ ����� �������� ������
    return 0;
}
