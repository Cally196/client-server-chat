/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Klient.h
 * Author: mmkme
 *
 * Created on Pondelok, 2019, januára 7, 14:57
 */

using namespace std;

#ifndef KLIENT_H
#define KLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>    
#include <cstring>
#include <iostream>
#include <sstream>



class Klient {
public:
    Klient();
    Klient(const Klient& orig);
    int start();
    virtual ~Klient();
    void printMenu();
    void login();
    void reg();
    void sendMessage();
    void checkMsg();
    void logout();
    void deleteAcc();
    void addFriend();
    void showFrndReq();
    void confirmReq();
    void printFriendlist();
    void deleteFriend();
    void exitKlient();
private:
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;    
    //char *buffer[256];
    int x=0, entry;
    string name;
    
};

#endif /* KLIENT_H */

