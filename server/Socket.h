/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Socket.h
 * Author: mmkme
 *
 * Created on Pondelok, 2019, januára 7, 16:05
 */

#ifndef SOCKET_H
#define SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>

class Socket {
public:
    Socket();
    int getSock();
    virtual ~Socket();
private:
    int sockNmb;
    struct sockaddr_in serv_addr;
};

#endif /* SOCKET_H */

