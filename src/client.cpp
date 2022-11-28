#include <iostream>
#include <stdio.h>
#include <string>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>

#define BUF_SIZE 4096
void send_msg(int);
void recv_msg(int);

using namespace std;
string msg;
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

    // char buf[4096];
    // string userInput;
    // do{
    //     cout << ">";
    //     cin >> userInput;
    //     if(userInput.size()>0){
    //         int sendResult = send(rsocket,userInput.c_str(),userInput.size() + 1,0);
    //         if (sendResult != SOCKET_ERROR){
    //             memset(buf,0,4096);
    //             int rbyte = recv(rsocket,buf,4096,0);
    //             cout << "SERVER " << string(buf,0,rbyte) << endl;
    //         }
    //     }

    // }while(userInput.size() > 0);
    thread snd(send_msg, rsocket);
    thread rcv(recv_msg, rsocket);
    
    snd.join();
    rcv.join();

    closesocket(rsocket);
    WSACleanup();
}

void send_msg(int rsocket){
    while(1){
        getline(cin, msg);
        if (msg == "Quit"|| msg == "quit"){
            closesocket(rsocket);
            exit(0);
        }
        string name_msg = rsocket + " " + msg;
        send(rsocket, name_msg.c_str(), name_msg.length() + 1, 0);
    }
}

void recv_msg(int rsocket){
    char name_msg[BUF_SIZE + sizeof(rsocket) + 1];
    while (1){
        int str_len = recv(rsocket, name_msg, BUF_SIZE + sizeof(rsocket) + 1, 0);
        if (str_len == -1){
            exit(-1);
        }
        cout<<string(name_msg)<<endl;
    }
}
