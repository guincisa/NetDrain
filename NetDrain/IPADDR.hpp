//
//  IPADDR.h
//  NetDrain
//
//  Created by Guglielmo Incisa di Camerana on 09/01/2022.
//

#ifndef IPADDR_h
#define IPADDR_h

class IPADD{
    friend class IPADDEXP;
private:
    string ipaddress;
    string p[4];
    bool splittd = false, valid = false;
public:
    IPADD(string _s){
        ipaddress = _s;
    }
    IPADD(string _s0, string _s1, string _s2, string _s3){
        ipaddress = _s0+"."+_s1+"."+_s2+"."+_s3;
        p[0] = _s0;
        p[1] = _s1;
        p[2] = _s2;
        p[3] = _s3;
        splittd = true;
    }
    string& get(void){
        return ipaddress;
    }
    string getSub(int _i){
        if (!splittd){
            split();
        }
        if (_i<0 ||_i>3)
            _i = 0;
        return p[_i];
    }
    
private:
    void split(void){
        splittd = true;
        
        char * cstr = new char [ipaddress.length()+1];
        std::strcpy (cstr, ipaddress.c_str());
        
        // cstr now contains a c-string copy of str
        char *saveptr1;
        p[0] = strtok_r (cstr,".",&saveptr1);
        p[1] = strtok_r (NULL,".",&saveptr1);
        p[2] = strtok_r (NULL,".",&saveptr1);
        p[3] = strtok_r (NULL,".",&saveptr1);
//        d[0] = std::stoi(p[0]);
//        d[1] = std::stoi(p[1]);
//        d[2] = std::stoi(p[2]);
//        d[3] = std::stoi(p[3]);
        delete[] cstr;
        
    }
    
};
class IPADDEXP : public IPADD{
private:
    //1.1.1.* or 1.1.*.* or *.1.*.* or 1.*.2-9.* ...
    vector<IPADD*> ipaddrs;
    bool expanded = false;
public:
    
    vector<IPADD*> getVector(void){
        return ipaddrs;
    };

    IPADDEXP(string _s) : IPADD(_s){}
    
    void expand(void){
        
        if (!splittd){
            split();
        }
        
        int start[4] = {0,0,0,0};
        int end[4] = {0,0,0,0};
        bool expa[4] = {false,false,false,false};
        //check 'N-M'
        // N <= M expand N,N+1,...M
    
        //check '*'
        // expand 1,...255
        //p[0]
        for (int i = 0 ; i <4 ; i ++){
            if (p[i].compare("*") == 0){
                start[i] = 0;
                end[i] = 255;
                expa[i] = true;
            }
        }
        for (int i = 0 ; i < 4 ; i++){
            size_t _sz = p[i].find("-");
            if (_sz!=std::string::npos){
                cout << "h2" << endl;
                char * cstr = new char [p[i].length()+1];
                std::strcpy (cstr, p[i].c_str());
                char *saveptr1;
                string _start  = strtok_r (cstr,"-",&saveptr1);
                string _end = strtok_r (NULL,".",&saveptr1);
                start[i] = stoi(_start);
                end[i] = stoi(_end);
                expa[i] = true;
                delete[] cstr;

            }

        }
        for (int i  = 0 ; i < 4 ; i ++){
            cout << i << " " << start[i] << " " << end[i]<< endl;
            if (!expa[i]){
                start[i] = std::stoi(p[i]);
                end[i] = std::stoi(p[i]);
            }
        }
            
        for (int n0 = start[0] ; n0 <= end[0] ; n0++ )
        {
            for (int n1 = start[1] ; n1 <= end[1] ; n1++)
            {
                for (int n2 = start[2] ; n2 <= end[2] ; n2++)
                {
                    for (int n3 = start[3] ; n3 <= end[3] ; n3++ )
                    {
                        string addr ="";
                        addr =  to_string(n0)+"."+to_string(n1)+"."+to_string(n2)+"."+to_string(n3);
                        //cout << "final " << addr<< endl;
                        IPADD* s = new IPADD(to_string(n0),to_string(n1),to_string(n2),to_string(n3));
                        ipaddrs.push_back(s);
                    }
                }
            }
                
        }
        expanded = true;
    }
};


#endif /* IPADDR_h */
