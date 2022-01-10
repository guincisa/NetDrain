//============================================================================
// Name        : NetDrain.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

// https://www.nirsoft.net/countryip/it.html
#include <iostream>
#include <fstream>
#include <string>
#include "ENG.hpp"
#include "IPADDR.hpp"

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
            IPADDEXP* ipad = new IPADDEXP(line);
            ipad->expand();
            vector<IPADD*> _a = ipad->getVector();
            for (IPADD* _s : _a) {
                std::cout << _s->get() << endl;
                ee->inq(&_s->get());
                //find the way to delete allocated
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

