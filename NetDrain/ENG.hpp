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
#include <arpa/inet.h>
#include <sys/time.h>

using namespace std;


class TH{
public:
    static mutex mtx, mtxQ;
    static mutex prt;
    static condition_variable cv;
    static queue<string*> siteq;
    
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
            string* z = siteq.front();
            siteq.pop();
            mtxQ.unlock();
            
            
            cout << " >>>DOING string " << *z <<" "<< thid <<endl;
            lck.unlock();
            if (exec(*z).compare("OK")==0){
                prt.lock();
                cout << *z << " " << thid << " OK" << endl;
                prt.unlock();
            }else{
                prt.lock();
                cout << *z << " " << thid << " NOK" << endl;
                prt.unlock();
            }
            prt.unlock();
            
            delete z;
        }
    }
    
    static string exec(string command){
        int valread, sock=0;
        struct sockaddr_in serv_addr;
        string Mess = "GET / HTTP/1.1\nHost: "+command+"\n\n";
        
        char buffer[1024] = {0};

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(80);
        
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            cout << "Socket creation error" << endl;
        }
        
        // Convert IPv4 and IPv6 addresses from text to binary form
        if(inet_pton(AF_INET, command.c_str(), &serv_addr.sin_addr)<=0)
        {
            cout <<" Invalid address/ Address not supported \n";
            return "NOK";
        }
        
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            cout << "Connection Failed \n";
            return "NOK";
        }
        send(sock , Mess.c_str() , Mess.size() , 0 );
        //printf("Hello message sent\n");
        valread = read(sock , buffer, 1024);
        //printf("%s\n",buffer );
        return "OK";
    }
    static string exec2(string command) {
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
        
        n = write(sockfd, char_array, strlen(char_array));
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
queue<string*> TH::siteq;

class ENG{
public:
    
    
    //Insert
    void inq(string* site){
        //cout << "insert "<<*site<<endl;
        TH::mtxQ.lock();
        TH::siteq.push(site);
        TH::cv.notify_all();
        TH::mtxQ.unlock();
    }
    
    //    string expand(string _inputString){
    //        size_t _sz = _inputString.find("*");
    //        if (_sz==std::string::npos){
    //            return _inputString;
    //        }
    //        else{
    //            for (int i = 1; i<256 ; i++){
    //                string* line2 = new string(_inputString.substr(0,_sz)+std::to_string(i)+_inputString.substr(_sz+1));
    //                size_t __sz= line2->find("*");
    //        }
    //
    //                if (__sz!=std::string::npos){
    //                    for (int ii = 1 ; ii<256 ; ii++){
    //                        string* line3 = new string(line2->substr(0,__sz)+std::to_string(ii)+line2->substr(__sz+1));
    //                        ee->inq(line3);
    //    }
    
    ENG(){
        
        //std::vector<thread> threads(MT);
#ifdef __APPLE__
        int MT = 4;
#else
        int MT = 50;
#endif
    std:thread t[MT];
        
        
        
        // spawn n threads:
        for (int i = 0; i < MT; i++) {
            t[i] =  thread(TH::runSite, i+1);
        }
        for (int i = 0; i < MT; i++) {
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
    uint64_t GetTimeStamp() {
        struct timeval tv;
        gettimeofday(&tv,NULL);
        return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
    }
};


//int main() {
//   cout << exec("wget www.apple.com");
//   cout << exec("wget www.google.com");
//   cout << exec("wget www.dell.com");
//   cout <<  exec("wget www.nokia.com");
//}

#endif
