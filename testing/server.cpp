#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

int main(){

    WSADATA Winsockdata;
    WORD ver = MAKEWORD(2,2);

    int wsOK = WSAStartup(ver,&Winsockdata);

    if(wsOK != 0){
        cerr << "---can't start quitting---" << endl;
        return 0;
    }

    SOCKET listening = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(listening == INVALID_SOCKET){
        cerr <<"--- can't create socket quitting---"<<endl;
        return 0;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(50000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listening,(sockaddr*)&hint,sizeof(hint));
    listen(listening,SOMAXCONN);

    sockaddr_in client;
    int clientsize = sizeof(client);
    SOCKET clientsocket = accept(listening,(sockaddr*)&client,&clientsize);

    char hostname[NI_MAXHOST];
    char service[NI_MAXSERV];

    memset(hostname,0,NI_MAXHOST);
    memset(service,0,NI_MAXSERV);

    if(getnameinfo((sockaddr*)&client,sizeof(client),hostname,NI_MAXHOST,service,NI_MAXSERV,0) == 0)
    {
        cout << "connected on port: " << service << endl;
    }
    else{
        inet_ntop(AF_INET,&client.sin_addr,hostname,NI_MAXHOST);
        cout << hostname << "connected" << ntohs(client.sin_port) << endl;
    }
     
     closesocket(listening);

     char buf[4096];
     while (true){
        memset(buf,0,4096);
        int datar = recv(clientsocket,buf,4096,0);
        if (datar == SOCKET_ERROR){
            cerr << "error in recv() function: " << endl;
            break;
        }
        if(datar == 0){
            cout << "disconnected" << endl;
            break;
        }
        cout << string(buf,0,datar) << endl;

        send(clientsocket,buf,datar + 1,0);
     }
     closesocket(clientsocket);
     WSACleanup();
}
