/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Server.cpp
 * Author: mmkme
 * 
 * Created on Pondelok, 2019, januára 7, 15:58
 */

#include <deque>
#include <mutex>
#include <vector>

#include "Server.h"
#include "Socket.h"

typedef struct data {
    int *sock;
} DATA;


using namespace std;

Server::Server() {
    socket = new Socket;
}

int Server::run() {

    sockfd = socket->getSock();

    listen(sockfd, 5);
    cli_len = sizeof (cli_addr);


    int j = 0;

    while (i == 0) {



        newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &cli_len);
        cout << newsockfd << endl;
        sockets[j] = newsockfd;
        thread newClient(&Server::threadFunc, this, &sockets[j]);
        threads[j] = move(newClient);

        j++;

    }

    for (int i = 0; i < sizeof (threads); i++) {
        threads[i].join();
    }



    close(newsockfd);
    close(sockfd);
    return 0;
}

int Server::parseMsg(string msg) {
    string ident = "#";
    string token = msg.substr(0, msg.find(ident));
    return atoi(token.c_str());

}

string Server::deleteIdent(string msg) {
    string ident = "#", str = msg;
    size_t pos = msg.find(ident);
    str.erase(0, pos + ident.length());
    return str;
}

void Server::threadFunc(int *sock) {

    string name = "";

    bool flag = true;

    try {
        while (flag) {

            bzero(buffer, 256);

            n = read(*sock, buffer, 255);
            string mssg(buffer);

            int token = Server::parseMsg(mssg);
            mssg = Server::deleteIdent(mssg);



            unique_lock<mutex> lk(mtx);
            while (!access)
                cv.wait(lk);
            access = false;


            if (token == 1) {
                handleMsg(mssg, sock, name);
            } else if (token == 2) {
                registerFunc(mssg, sock);
            } else if (token == 3) {
                name = loginFunc(mssg, sock);
            } else if (token == 4) {
                sendMsg(name, sock);
                //                for (int i = 0; i < 200000; i++) {
                //                    cout << *sock << endl;
                //                }
            } else if (token == 5) {
                name = logoutFunc(sock);
            } else if (token == 6) {
                name = deleteAcc(name, sock);
            } else if (token == 7) {
                addFriend(sock, name, mssg);
            } else if (token == 8) {
                showFrndReq(sock, name);
            } else if (token == 9) {
                confirmReq(sock, name, mssg);
            } else if (token == 10) {
                printFriendlist(sock, name);
            } else if (token == 11) {
                deleteFriend(sock, name, mssg);
            } else if (token == 12) {
                cout << "Klient odhlaseny." << endl;
                flag = false;
            }




            access = true;
            lk.unlock();
            cv.notify_all();
        }
    } catch (exception) {
        cout << "EROOOOOOOOOOOOR";
    }

}

void Server::deleteFriend(int* sock, string name, string msg) {
    string frnd, ident = ";";
    size_t pos = 0;

    frnd = msg.substr(0, msg.find(ident));

    vector<string> vec = friendlist.find(name)->second;
    //auto it = vec.begin();

    bool flag = false;


    for (auto s : vec) {
        if (s == name) {
            flag = true;
            break;
        }

    }

    if (find(vec.begin(), vec.end(), frnd) != vec.end()) {
        vector<string> vctr = friendlist.find(name)->second;
        auto pos = find(vctr.begin(), vctr.end(), frnd);
        vctr.erase(pos);

        map <string, vector < string>>::iterator position;
        position = friendlist.find(name);
        friendlist.erase(position);
        friendlist[name] = vctr;


        vctr = friendlist.find(frnd)->second;
        pos = find(vctr.begin(), vctr.end(), name);
        vctr.erase(pos);

        //map <string, vector<string>>::iterator position;
        position = friendlist.find(frnd);
        friendlist.erase(position);
        friendlist[frnd] = vctr;



        const char* mssg = "Odstranenie priatela prebehlo uspesne.";
        n = write(*sock, mssg, strlen(mssg) + 1);
    } else {
        const char* mssg = "Zadaneho priatela nemate vo friendliste.";
        n = write(*sock, mssg, strlen(mssg) + 1);
    }


}

void Server::printFriendlist(int* sock, string name) {
    stringstream stream;

    vector<string> frnds = friendlist.find(name)->second;

    if (!frnds.empty()) {

        int iter;
        for (vector<string>::const_iterator iter = frnds.begin(); iter != frnds.end(); ++iter) {
            stream << *iter << "\n";
        }

        string str = stream.str();
        cout << str << endl;
        char msg[256];
        strcpy(msg, str.c_str());

        n = write(*sock, msg, strlen(msg) + 1);
    } else {
        const char* mssg = "0";
        n = write(*sock, mssg, strlen(mssg) + 1);
    }
}

void Server::confirmReq(int* sock, string name, string msg) {
    string frnd, ident = ";";
    size_t pos = 0;

    frnd = msg.substr(0, msg.find(ident));
    bool flag = false;

    auto it = friendReq.begin();
    while (it != friendReq.end()) {
        if (it->second == frnd) {
            flag = true;
            break;
        }
    }

    if (flag) {
        map <string, string>::iterator position;

        friendReq.erase(it);

        friendlist.find(name)->second.push_back(frnd);
        friendlist.find(frnd)->second.push_back(name);

        const char* mssg = "Ziadost o priatelstvo bola potvrdena.";
        n = write(*sock, mssg, strlen(mssg) + 1);
    } else {
        const char* mssg = "Od zadaneho cloveka nemate ziadost o priatelstvo.";
        n = write(*sock, mssg, strlen(mssg) + 1);
    }
}

void Server::showFrndReq(int *sock, string name) {
    if (friendReq.find(name) != friendReq.end()) {

        stringstream stream;


        auto range = friendReq.equal_range(name);

        for (auto i = range.first; i != range.second; i++) {

            stream << i->second << "\n";

        }

        string str = stream.str();
        cout << str << endl;
        char msg[256];
        strcpy(msg, str.c_str());

        n = write(*sock, msg, strlen(msg) + 1);

    } else {
        stringstream stream;
        stream << 0;
        string str = stream.str();

        char msg[256];
        strcpy(msg, str.c_str());

        n = write(*sock, msg, strlen(msg) + 1);
    }
}

void Server::addFriend(int* sock, string name, string msg) {
    string frnd, ident = ";";
    size_t pos = 0;

    frnd = msg.substr(0, msg.find(ident));

    if (Server::findLogReg(frnd)) {
        friendReq.insert(make_pair(frnd, name));\

        const char* msg = "Ziadost o priatelstvo odoslana. Caka sa na potvrdenie.";
        n = write(*sock, msg, strlen(msg) + 1);
    } else {
        const char* msg = "Uzivatel s danym menom neexistuje.";
        n = write(*sock, msg, strlen(msg) + 1);
    }
}

string Server::deleteAcc(string name, int* sock) {
    if (Server::findLogReg(name)) {
        map <string, string>::iterator position;
        position = users.find(name);
        users.erase(position);

        const char* msg = "Vas ucet bol odstraneny.";
        n = write(*sock, msg, strlen(msg) + 1);
        return "";
    } else {
        const char* msg = "Vas ucet sa nenachadza v databaze.";
        n = write(*sock, msg, strlen(msg) + 1);
        return name;
    }
}

string Server::logoutFunc(int* sock) {
    const char* msg = "Boli ste odhlaseny.";
    n = write(*sock, msg, strlen(msg) + 1);
    return "";
}

void Server::sendMsg(string name, int *sock) {
    string adr, message, msg, ident = ";";
    size_t pos = 0;

    if (messages.find(name) != messages.end()) {
        stringstream stream;

        auto range = messages.equal_range(name);

        for (auto i = range.first; i != range.second; i++) {
            msg = i->second;

            adr = msg.substr(0, msg.find(ident));
            pos = msg.find(ident);
            msg.erase(0, pos + ident.length());
            message = msg.substr(0, msg.find(ident));


            stream << adr << ": " << message << "\n";

        }

        string str = stream.str();
        cout << str << endl;
        char msg[256];
        strcpy(msg, str.c_str());

        n = write(*sock, msg, strlen(msg) + 1);

    } else {
        const char* mssg = "Ziadne spravy nenajdene";
        n = write(*sock, mssg, strlen(mssg) + 1);
    }
}

void Server::handleMsg(string msg, int* sock, string name) {

    string rec, sendMsg, ident = ";";
    size_t pos = 0;

    rec = msg.substr(0, msg.find(ident));
    pos = msg.find(ident);
    msg.erase(0, pos + ident.length());
    sendMsg = msg;

    vector<string> vctr = friendlist.find(name)->second;

    if (find(vctr.begin(), vctr.end(), rec) != vctr.end()) {
        messages.insert(make_pair(rec, sendMsg));

        const char* mssg = "Správa bola prijatá.";
        n = write(*sock, mssg, strlen(mssg) + 1);
    } else {
        const char* mssg = "Daneho adresata nemate v priateloch.";
        n = write(*sock, mssg, strlen(mssg) + 1);
    }
}

string Server::loginFunc(string msg, int *sock) {
    string ident = ";";
    size_t pos = 0;
    string name, pass;

    name = msg.substr(0, msg.find(ident));
    pos = msg.find(ident);
    msg.erase(0, pos + ident.length());
    pass = msg.substr(0, msg.find(ident));

    if (Server::findLogReg(name)) {
        map <string, string>::iterator position;
        position = users.find(name);
        string passDB = position->second;

        if (passDB == pass) {
            const char* msg = "Boli ste uspesne prihlaseny.";
            n = write(*sock, msg, strlen(msg) + 1);
            return name;
        } else {
            const char* msg = "Zadali ste zle prihlasovacie udaje.";
            n = write(*sock, msg, strlen(msg) + 1);
            return "";
        }


    } else {
        const char* msg = "Najprv sa musite zaregistrovat.";
        n = write(*sock, msg, strlen(msg) + 1);
    }
    return "";
}

void Server::registerFunc(string msg, int *sock) {
    string ident = ";";
    size_t pos = 0;
    string name, pass;

    name = msg.substr(0, msg.find(ident));
    pos = msg.find(ident);
    msg.erase(0, pos + ident.length());
    pass = msg.substr(0, msg.find(ident));

    if (Server::findLogReg(name)) {
        const char* msg = "Uzivatel s danym menom uz existuje.";
        n = write(*sock, msg, strlen(msg) + 1);
    } else {
        users[name] = pass;
        vector<string> vec;
        friendlist[name] = vec;
        const char* msg = "Boli ste uspesne zaregistrovany.";
        n = write(*sock, msg, strlen(msg) + 1);
    }

}

bool Server::findLogReg(string name) {
    if (users.find(name) == users.end()) {
        return false;
    } else {
        return true;
    }

}

Server::~Server() {
    delete &mtx;
    sockfd = 0;
    newsockfd = 0;
    delete &cli_len;
    delete &serv_addr;
    delete &cli_addr;
    n = 0;
    free(buffer);
    i = 0;
    free(socket);
    for (i = 1; i < 500; i++) {
        delete &threads[i];
    }
    for (i = 1; i < 500; i++) {
        sockets[i] = 0;
    }
    for (map<string, string>::iterator it = users.begin(); it != users.end(); ++it)
        delete &it->second;
    delete &users;
    
    for (map<string, vector<string>>::iterator it = friendlist.begin(); it != friendlist.end(); ++it)
        delete &it->second;
    delete &friendlist;
    
    for (map<string, string>::iterator it = messages.begin(); it != messages.end(); ++it)
        delete &it->second;
    delete &messages;
    
    for (map<string, string>::iterator it = friendReq.begin(); it != friendReq.end(); ++it)
        delete &it->second;
    delete &friendReq;

}

