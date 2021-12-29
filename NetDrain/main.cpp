//============================================================================
// Name        : NetDrain.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include "ENG.hpp"
using namespace std;

ENG* ee;

void th2(){
    ee = new ENG();
}

int main() {
    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

    std:thread tENG (th2);

    string line;
    ifstream myfile ("sites.txt");
    if (myfile.is_open())
    {
      while ( getline (myfile,line) )
      {
          size_t _sz = line.find("*");
          if (_sz!=std::string::npos){
              for (int i = 2; i<256 ; i++){
                  string line2 = line.substr(0,_sz)+std::to_string(i)+line.substr(_sz+1);
                  cout <<"expand"<<line2<<endl;
                  ee->inq(line2);
              }
          }else {
              ee->inq(line);
          }
      }
      myfile.close();
    }

    
    
//
    static mutex stop;
    unique_lock<mutex> lck(stop);
    stop.lock();
    stop.lock();
    cout << "dying" << endl;
    
    /*
     
     GET / HTTP/1.1
     Host: www.example.com
     User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)

     */


    return 0;
}
