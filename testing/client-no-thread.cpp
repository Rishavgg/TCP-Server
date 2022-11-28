
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