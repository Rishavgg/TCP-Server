#include <iostream>
#include <stdio.h>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

int main(){
    string ipaddress = "127.0.0.1";
    int port = 50000;

    WSADATA data;
    WORD ver = MAKEWORD(2,2);

    int wsResult = WSAStartup(ver,&data);

    if(wsResult != 0){
        cerr << "-----can't start quitting-----" << wsResult << endl;
        return 0;
    }

    SOCKET rsocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(rsocket == INVALID_SOCKET){
        cerr <<"----- can't create socket quitting-----"<< WSAGetLastError <<endl;
        return 0;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET,ipaddress.c_str(),&hint.sin_addr);

    int connresult = connect(rsocket,(sockaddr*)&hint,sizeof(hint));
    if(connresult == SOCKET_ERROR){
        cerr << "cant connect to server" << WSAGetLastError() << endl;
        closesocket(rsocket);
        WSACleanup();
        return 0;
    }

    char buf[4096];
    string userInput;
    do{
        cout << ">";
        cin >> userInput;
        if(userInput.size()>0){
            int sendResult = send(rsocket,userInput.c_str(),userInput.size() + 1,0);
            if (sendResult != SOCKET_ERROR){
                memset(buf,0,4096);
                int rbyte = recv(rsocket,buf,4096,0);
                cout << "SERVER " << string(buf,0,rbyte) << endl;
            }
        }

    }while(userInput.size() > 0);

    closesocket(rsocket);
    WSACleanup();
}