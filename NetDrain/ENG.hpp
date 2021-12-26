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

using namespace std;


class TH{
    public:
     static mutex mtx, mtxQ;
     static condition_variable cv;
     static queue<void*> siteq;
     static string* _website;

     static void runSite (int thid) {
      while(true){
          unique_lock<mutex> lck(mtx);
          while(siteq.empty()){
              cout<<thid<<" waiting"<<endl;
              cv.wait(lck);
          }
          cout << "gotcha" <<endl;
          mtxQ.lock();
          string* z = (string*)siteq.front();
          siteq.pop();
          mtxQ.unlock();


          cout << " >>>DOING string " << *z <<" "<< thid <<endl;
          lck.unlock();
          exec("wget "+*z);
          cout << " >>>DONE string " << *z <<" "<< thid <<endl;

          }
      }
    
    static string exec(string command) {
       char buffer[128];
       string result = "";

       // Open pipe to file
       FILE* pipe = popen(command.c_str(), "r");
       if (!pipe) {
          return "popen failed!";
       }

       // read till end of process:
       while (!feof(pipe)) {

          // use buffer to read and add to result
          if (fgets(buffer, 128, pipe) != NULL)
             result += buffer;
       }

       pclose(pipe);
       return result;
    }
};
condition_variable TH::cv;
mutex TH::mtx;
mutex TH::mtxQ;
queue<void*> TH::siteq;

class ENG{
public:


    //Insert
    void inq(void* site){
        TH::mtxQ.lock();
        TH::siteq.push(site);
        TH::cv.notify_all();
        TH::mtxQ.unlock();
    }

    ENG(){
        int MT=10;

        //std::vector<thread> threads(MT);
        std:thread t1(TH::runSite,1),t2(TH::runSite,2),t3(TH::runSite,3);
        thread t4(TH::runSite,4),t5(TH::runSite,5),t6(TH::runSite,6);

//        // spawn n threads:
//        for (int i = 0; i < MT; i++) {
//            threads[i] =  thread(TH::runSite, i+1);
//        }
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();

        cout << "spwaned" <<endl;

    }

};



//int main() {
//   cout << exec("wget www.apple.com");
//   cout << exec("wget www.google.com");
//   cout << exec("wget www.dell.com");
//   cout <<  exec("wget www.nokia.com");
//}

#endif
