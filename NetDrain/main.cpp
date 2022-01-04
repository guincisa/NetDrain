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
#ifdef __APPLE__
    ifstream myfile ("/Users/gug/sites.txt");
#else
    ifstream myfile ("sites.txt");
#endif
    if (myfile.is_open())
    {
      while ( getline (myfile,line) )
      {
          size_t _sz = line.find("*");
          if (_sz!=std::string::npos){
              for (int i = 1; i<256 ; i++){
                  string* line2 = new string(line.substr(0,_sz)+std::to_string(i)+line.substr(_sz+1));
                  size_t __sz= line2->find("*");
                  if (__sz!=std::string::npos){
                      for (int ii = 1 ; ii<256 ; ii++){
                          string* line3 = new string(line2->substr(0,__sz)+std::to_string(ii)+line2->substr(__sz+1));
                          ee->inq(line3);
                          //cout <<"expand "<<*line3<<endl;
                      }
                      delete line2;
                  }
                  else{
                      ee->inq(line2);
                      //cout <<"expand "<<*line2<<endl;
                  }
              }
          }else {
              string* line2 = new string(line);
              ee->inq(line2);
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
