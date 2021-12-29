//============================================================================
// Name        : NetDrain.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "ENG.hpp"
using namespace std;

ENG* ee;

void th2(){
    ee = new ENG();
}

int main() {
    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

    std:thread tENG (th2);
    string* s1 = new string("www.apple.com");
    string* s2 = new string("www.nokia.com");
    string* s3 = new string("www.microsoft.com");
    string* s4 = new string("192.168.1.1");
    string* s5 = new string("www.ecuavisa.com");
    
    cout << s1 << " " << *s1<<endl;
    cout << s2 << " " << *s2<<endl;
    cout << s3 << " " << *s3<<endl;
    cout << s4 << " " << *s4<<endl;
    cout << s5 << " " << *s5<<endl;

    ee->inq((void*)s1);
    ee->inq((void*)s2);
    ee->inq((void*)s3);
    ee->inq((void*)s4);
    ee->inq((void*)s5);
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
