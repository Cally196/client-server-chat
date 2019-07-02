/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Socket.cpp
 * Author: mmkme
 * 
 * Created on Pondelok, 2019, januára 7, 16:05
 */

#include "Socket.h"

Socket::Socket() {
}

int Socket::getSock(){
    //vyčistenie (vynulovanie) a inicializácia sieťovej adresy

	bzero((char*)&serv_addr, sizeof(serv_addr));

//nastavenie pre adresovanie v doméne INET

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(7753);

//tvorba samotného socketu "sockfd"

	sockNmb = socket(AF_INET, SOCK_STREAM, 0);

//priradenie adresy socketu (aj s overením)

	if (bind(sockNmb, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		 perror("CHYBA! - nepodarilo sa priradiť adresu");
	} else {
            return sockNmb;
        }

}

Socket::~Socket() {
    sockNmb = 0;
    delete &serv_addr;
}

