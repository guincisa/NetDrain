//**********************************************************************************
//**********************************************************************************
//**********************************************************************************
// NetDrain
// Copyright (C) 2021 Guglielmo Incisa di Camerana
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//**********************************************************************************
//**********************************************************************************
//**********************************************************************************
//
//  ENG.h
//  ECMS
//

#ifndef ENG_h
#define ENG_h
#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <queue>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
using namespace std;


class TH{
    public:
     static mutex mtx, mtxQ;
     static mutex prt;
     static condition_variable cv;
     static queue<string> siteq;
     //static string _website;

     static void runSite (int thid) {
      while(true){
          unique_lock<mutex> lck(mtx);
          while(siteq.empty()){
              //cout<<thid<<" waiting"<<endl;
              cv.wait(lck);
          }
          //cout << "gotcha" <<endl;
          mtxQ.lock();
          string z = (string)siteq.front();
          siteq.pop();
          mtxQ.unlock();


          //cout << " >>>DOING string " << z <<" "<< thid <<endl;
          lck.unlock();
          string rr = exec(z);
          if (rr.compare("OK")==0){
            prt.lock();
            cout << z << " " << thid << " OK" << endl;
            prt.unlock();

          }
//         if (rr.compare("ERR")!=0&&rr.compare("NOHOST")!=0&&rr.compare("CONERR")!=0&&rr.compare("WRIERR")!=0&&rr.compare("REAERR")!=0){
//              int cr = rr.find("\n");
//              prt.lock();
//              cout << z << " " << thid << " " << rr.substr(0,cr) << endl;
//              prt.unlock();
//              //cout << " >>>DONE string " << z <<" "<< thid <<endl;
//
//          }
//          else{
//              prt.lock();
//              cout << z << " " << thid << " " << rr << endl;
//              prt.unlock();
//          }
      }

    }
    
    static string exec(string command) {
        int sockfd, portno=80, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;

        char buffer[256];

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            return "ERR";
        server = gethostbyname(command.c_str());
        if (server == NULL) {
            return "NOHOST";
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr,
             (char *)&serv_addr.sin_addr.s_addr,
             server->h_length);
        serv_addr.sin_port = htons(portno);
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
            return "CONERR";
        
        //string Mess = "GET /hello.htm HTTP/1.1\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\n Host: "+command+"\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive";
        string Mess = "GET / HTTP/1.1\nHost: "+command+"\n\n";

        //cout << "["<<Mess<<"]"<<endl;
        char char_array[Mess.length() + 1];
        strcpy(char_array, Mess.c_str());

        n = write(sockfd,char_array,strlen(char_array));
        if (n < 0)
            return "WRIERR";
        bzero(buffer,256);
        n = read(sockfd,buffer,255);
        if (n < 0)
            return "REAERR";
        //printf("%s\n",buffer);
        close(sockfd);
        //return string(buffer);
        return "OK";
    }

};
condition_variable TH::cv;
mutex TH::mtx;
mutex TH::mtxQ;
mutex TH::prt;
queue<string> TH::siteq;

class ENG{
public:


    //Insert
    void inq(string site){
        //cout << "insert"<<site<<endl;
        TH::mtxQ.lock();
        TH::siteq.push(site);
        TH::cv.notify_all();
        TH::mtxQ.unlock();
    }

    ENG(){

        //std::vector<thread> threads(MT);
        std:thread t[50];
        

        // spawn n threads:
        for (int i = 0; i < 50; i++) {
            t[i] =  thread(TH::runSite, i+1);
        }
        for (int i = 0; i < 50; i++) {
            t[i].join();
        }

        cout << "spwaned" <<endl;

    }

};

class UTIL{
public:
    int active  = 0;
    void DEB(string s){
        if (active == 1){
            cout << "[" << s << "]";
        }
    }
};


//int main() {
//   cout << exec("wget www.apple.com");
//   cout << exec("wget www.google.com");
//   cout << exec("wget www.dell.com");
//   cout <<  exec("wget www.nokia.com");
//}

#endif
