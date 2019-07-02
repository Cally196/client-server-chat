#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "Klient.h"
#include <iostream>


using namespace std;

Klient::Klient() {

}

Klient::~Klient() {
    sockfd = 0;
    n = 0;
    delete &serv_addr;
    free(server);
    x = 0;
    entry = 0;
    name = "";
}

int Klient::start() {


    server = gethostbyname("localhost");

    bzero((char*) &serv_addr, sizeof (serv_addr));

    serv_addr.sin_family = AF_INET;
    bcopy(
            (char*) server->h_addr,
            (char*) &serv_addr.sin_addr.s_addr,
            server->h_length
            );


    serv_addr.sin_port = htons(7753);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);


    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof (serv_addr)) < 0) {
        perror("CHYBA!!!");
    }

    printMenu();

    while (x == 0) {
        cin >> entry;

        if (!cin) {
            cout << "Enter a valid name fool.\n" << "Try again:  " << endl;
            cin.clear();
            cin.ignore(100, '\n');
            continue;
        }


        switch (entry) {
            default:
                printMenu();
                break;

            case 1:
                sendMessage();
                break;

            case 2:
                login();
                break;

            case 3:
                reg();
                break;

            case 4:
                checkMsg();
                break;

            case 5:
                logout();
                break;

            case 6:
                deleteAcc();
                break;

            case 7:
                addFriend();
                break;

            case 8:
                showFrndReq();
                break;

            case 9:
                confirmReq();
                break;

            case 10:
                printFriendlist();
                break;

            case 11:
                deleteFriend();
                break;
                
            case 12:
                logout();
                exitKlient();
                x = 1;
                break;
        }



    }

    close(sockfd);

    return 0;
}

void Klient::exitKlient() {
    stringstream stream;
    stream << 12 << "#";
    string str = stream.str();
    char buffer[256];
    strcpy(buffer, str.c_str());
    n = write(sockfd, buffer, strlen(buffer));
    
   
}

void Klient::deleteFriend() {
    if (Klient::name != "") {
        cout << "Zadajte meno priatela na vymazanie:\n";
        string frnd;
        cin >> frnd;


        stringstream stream;
        stream << 11 << "#" << frnd << ";";
        string str = stream.str();
        char buffer[256];
        strcpy(buffer, str.c_str());
        n = write(sockfd, buffer, strlen(buffer));

        bzero(buffer, 256); 
        n = read(sockfd, buffer, 255); 

        cout << buffer << endl;

    } else {
        cout << "Najpr musite byt prihlaseny." << endl;
    }
}

void Klient::printFriendlist() {
    stringstream stream;
    stream << 10 << "#";
    string str = stream.str();
    char buffer[256];
    strcpy(buffer, str.c_str());
    n = write(sockfd, buffer, strlen(buffer));

    bzero(buffer, 256); 
    n = read(sockfd, buffer, 255); 

    string msg(buffer);

    if (strcmp(buffer, "0") == 0) {
        cout << "Nemate ziadnych priatelov." << endl;
    } else {
        cout << "Priatelia:\n" << msg << endl;
    }
}

void Klient::confirmReq() {
    string frnd;

    cout << "Zadaj meno:\n";
    cin >> frnd;

    stringstream stream;
    stream << 9 << "#" << frnd << ";";

    string str = stream.str();

    char buffer[256];
    //bzero(buffer, 256);

    strcpy(buffer, str.c_str());

    n = write(sockfd, buffer, strlen(buffer)); 

    bzero(buffer, 256); 
    n = read(sockfd, buffer, 255); 

    printf("%s\n", buffer);
}

void Klient::showFrndReq() {
    stringstream stream;
    stream << 8 << "#";
    string str = stream.str();
    char buffer[256];
    strcpy(buffer, str.c_str());
    n = write(sockfd, buffer, strlen(buffer));

    bzero(buffer, 256); 
    n = read(sockfd, buffer, 255); 
    string msg(buffer);

    if (strcmp(buffer, "0") == 0) {
        cout << "Nemate ziadne ziadosti o priatelstvo." << endl;
    } else {
        cout << "Ziadosti o priatelstvo od:\n" << msg << endl;
    }



}

void Klient::addFriend() {
    string frnd;

    cout << "Zadaj meno:\n";
    cin >> frnd;

    stringstream stream;
    stream << 7 << "#" << frnd << ";";

    string str = stream.str();

    char buffer[256];


    strcpy(buffer, str.c_str());

    n = write(sockfd, buffer, strlen(buffer));

    bzero(buffer, 256); 
    n = read(sockfd, buffer, 255); 

    printf("%s\n", buffer);
}

void Klient::deleteAcc() {
    if (Klient::name != "") {
        stringstream stream;
        stream << 6 << "#";
        string str = stream.str();
        char buffer[256];
        strcpy(buffer, str.c_str());
        n = write(sockfd, buffer, strlen(buffer));

        bzero(buffer, 256); 
        n = read(sockfd, buffer, 255); 

        if (strcmp(buffer, "Vas ucet bol odstraneny.") == 0) {
            Klient::name = "";
            cout << buffer << endl;
        }
    } else {
        cout << "Najpr musite byt prihlaseny." << endl;
    }
}

void Klient::logout() {
    if (Klient::name != "") {
        stringstream stream;
        stream << 5 << "#";
        string str = stream.str();
        char buffer[256];
        strcpy(buffer, str.c_str());
        n = write(sockfd, buffer, strlen(buffer));

        bzero(buffer, 256); 
        n = read(sockfd, buffer, 255); 

        if (strcmp(buffer, "Boli ste odhlaseny.") == 0) {
            Klient::name = "";
            cout << buffer << endl;
        }

    } else {
        cout << "Najpr musite byt prihlaseny." << endl;
    }


}

void Klient::checkMsg() {
    char buffer[256];
    const char* mssg = "4#";

    n = write(sockfd, mssg, strlen(mssg)); 


    bzero(buffer, 256); 
    n = read(sockfd, buffer, 255); 

    cout << "\nPrijate spravy:\n" << buffer << endl;
}

void Klient::printMenu() {
    cout << endl << endl << "##################################\n";

    if (this->name == "") {
        cout << "Neprihlaseny uzivatel\n";
        cout << "2- Prihlas sa\n";
        cout << "3- Zaregistruj sa\n";
    } else {
        cout << "Prihlaseny ako:" << Klient::name << endl;
        cout << "1- Posli spravu\n";
        cout << "4- Pozri spravy\n";
        cout << "5- Odhlas sa\n";
        cout << "6- Zrus ucet\n";
        cout << "7- Pridaj priatela\n";
        cout << "8- Ukaz ziadosti o priatelstvo\n";
        cout << "9- Potvrd ziadost o priatelstvo\n";
        cout << "10- Zobraz priatelov\n";
        cout << "11- Vymaz priatela\n";
        
    }
    cout << "12- Ukonci\n";
}

void Klient::reg() {
    string name, pass;
    cout << "Zadaj meno:\n";
    cin >> name;
    cout << "Zadaj heslo:\n";
    cin >> pass;

    stringstream stream;
    stream << 2 << "#" << name << ";" << pass << ";";

    string str = stream.str();

    char buffer[256];
    strcpy(buffer, str.c_str());

    n = write(sockfd, buffer, strlen(buffer)); 

    bzero(buffer, 256); 
    n = read(sockfd, buffer, 255); 

    printf("%s\n", buffer);
}

void Klient::login() {
    string name, pass;
    cout << "Zadaj meno:\n";
    cin >> name;
    cout << "Zadaj heslo:\n";
    cin >> pass;

    stringstream stream;
    stream << 3 << "#" << name << ";" << pass << ";";

    string str = stream.str();

    char buffer[256];
    strcpy(buffer, str.c_str());


    n = write(sockfd, buffer, strlen(buffer)); 

    bzero(buffer, 256); 
    n = read(sockfd, buffer, 255); 

    if (strcmp(buffer, "Boli ste uspesne prihlaseny.") == 0) {
        Klient::name = name;
        cout << buffer << endl;
    } else {
        cout << buffer << endl;
    }

}

void Klient::sendMessage() {

    string message, rec;

    cout << "Zadaj prijemcu:\n";
    cin >> rec;
    cout << "Zadaj spravu:\n";
    cin >> message;

    stringstream stream;
    stream << 1 << "#" << rec << ";" << name << ";" << message << ";";

    string str = stream.str();


    char buffer[256];
    //bzero(buffer, 256);

    strcpy(buffer, str.c_str());

    n = write(sockfd, buffer, strlen(buffer)); 

    bzero(buffer, 256); 
    n = read(sockfd, buffer, 255); 

    printf("%s\n", buffer);
}


