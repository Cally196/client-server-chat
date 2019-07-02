/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Server.h
 * Author: mmkme
 *
 * Created on Pondelok, 2019, januára 7, 15:58
 */

#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "Socket.h"
#include <thread>
#include <map>
#include <string>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <algorithm>

using namespace std;

class Server {
public:
    Server();
    int run();
    void threadFunc(int *sock);
    int parseMsg(string msg);
    string deleteIdent(string msg);
    bool findLogReg(string name);
    string loginFunc(string msg, int *sock);
    void handleMsg(string msg, int *sock, string name);
    void registerFunc(string msg, int *sock);
    void sendMsg(string name, int* sock);
    string logoutFunc(int* sock);
    string deleteAcc(string name, int *sock);
    void addFriend(int *sock, string name, string msg);
    void showFrndReq(int *sock, string name);
    void confirmReq(int *sock, string name, string msg);
    void printFriendlist(int *sock, string name);
    void deleteFriend(int *sock, string name, string msg);
    virtual ~Server();
private:
    mutex mtx;
    condition_variable cv;
    bool access = true;
    int sockfd, newsockfd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    char buffer[256];
    int i = 0;
    Socket *socket;
    thread threads[500];
    int sockets[500];
    map<string, string> users;
    map<string, vector<string>> friendlist;
    multimap<string, string>messages;
    multimap<string, string> friendReq;
    
};

#endif /* SERVER_H */

