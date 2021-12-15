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
using namespace std;


class TH{
    public:
     static mutex mtx, mtxQ, stop;
     static condition_variable cv;
     static queue<void*> siteq;

     static void runSite (int thid) {
      while(true){
          unique_lock<mutex> lck(mtx);
          while(siteq.empty()){
              cout<<thid<<" waiting"<<endl;
              cv.wait(lck);
          }
          mtxQ.lock();
          int z = *((int*)siteq.front());
          siteq.pop();
          mtxQ.unlock();


          cout << ">>>DOING" << z <<".." << thid<< endl;
          lck.unlock();
          usleep(400000);
          cout << "<<<DONE"<< z <<".." << thid<< endl;

          }
      }
};
condition_variable TH::cv;
mutex TH::mtx;
mutex TH::mtxQ;
mutex TH::stop;
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

        std::vector<thread> threads(MT);
        // spawn n threads:
        for (int i = 0; i < MT; i++) {
            threads[i] =  thread(TH::runSite, i+1);
        }
        unique_lock<mutex> lck(TH::stop);
        usleep(400000);
        int i = 1;
        inq((void*)&i);
        usleep(400000);
        int ii = 2;
        inq((void*)&ii);

        TH::stop.lock();

    }

};
#endif
