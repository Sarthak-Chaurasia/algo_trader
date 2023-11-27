#include "receiver.h"
#include<vector>
#include<string>
#include<sstream>
#include<iostream>
using namespace std;
class mymap {
    private:
    struct pairval{
        string str;
        int val[5]={-1,-1,-1,-1,-1};
    };
    vector<pairval> data;
    long int hash(string str){
        long int hashval=10;
        for(auto c:str){
            hashval = (30*hashval) + static_cast<size_t>(c);
        }
        return hashval; 
    }
    public:
    mymap(size_t initval=100) : data(initval, pairval{"",}){}
    void insert(string key, int price, bool type){
        long int godplz = hash(key);
        bool hashfinally=0;
        while(!hashfinally){
            long int hashval= godplz%data.size();
            bool flag=0;
            long int initial=hashval;
            while(!flag){
                if(data[hashval].str=="" || data[hashval].str==key) {hashfinally=1;godplz=hashval;break;}
                hashval = (hashval+1)%data.size();
                if(initial==hashval) flag=1;
            }
            if(flag==1){
                data.resize(2*data.size()+1,pairval{"",});
            }
        }
                if(data[godplz].val[0]==-1) {
            data[godplz].val[0]=price;
            cout<<key<<" "<<to_string(price)<<" "<<(type==1?"b":"s")<<endl; //terneray here is opposite
        }
        else if(type==0){
            if(data[godplz].val[1]==-1 || data[godplz].val[1]<price){ //ig if there is an active order then either it will remain or new will you can change that
                if(data[godplz].val[1]!=price){
                    data[godplz].val[1]=price;
                    data[godplz].val[2]=1;
                }
                else{
                    data[godplz].val[2]++;
                }
                if(data[godplz].val[3]==-1 || data[godplz].val[3]!=price){
                    if(data[godplz].val[0]<price){
                        cout<<key<<" "<<to_string(price)<<" s"<<endl;
                        data[godplz].val[0]=price;
                        if(data[godplz].val[2]==1){
                            data[godplz].val[1]=-1;
                            data[godplz].val[2]=-1;
                        }
                        else{
                            data[godplz].val[2]--;
                        }
                    }
                    else{
                        cout<<"No Trade"<<endl;
                    }
                }
                else{
                    cout<<"No Trade"<<endl; //trade happening between some two but not us so curr price remains same
                    if(data[godplz].val[4]>1) {data[godplz].val[4]--;}
                    else{
                        data[godplz].val[3]=-1;data[godplz].val[4]=-1;
                    }
                    if(data[godplz].val[2]>1){data[godplz].val[2]--;}
                    else{
                        data[godplz].val[1]=-1;data[godplz].val[2]=-1;
                    }
                }
            }
            else{
                cout<<"No Trade"<<endl;
            }
        }
        else if(type==1){
            if(data[godplz].val[3]==-1 || data[godplz].val[3]>price){ //ig if there is an active order then either it will remain or new will you can change that
                if(data[godplz].val[3]!=price){
                    data[godplz].val[3]=price;
                    data[godplz].val[4]=1;
                }
                else{
                    data[godplz].val[4]++;
                }
                if(data[godplz].val[1]==-1 || data[godplz].val[1]!=price){
                    if(data[godplz].val[0]>price){
                        cout<<key<<" "<<to_string(price)<<" b"<<endl;
                        data[godplz].val[0]=price;
                        if(data[godplz].val[4]==1){
                            data[godplz].val[3]=-1;
                            data[godplz].val[4]=-1;
                        }
                        else{
                            data[godplz].val[4]--;
                        }
                    }
                    else{
                        cout<<"No Trade"<<endl;
                    }
                }
                else{
                    cout<<"No Trade"<<endl; //trade happening between some two but not us so curr price remains same
                    if(data[godplz].val[2]>1) {data[godplz].val[2]--;}
                    else{
                        data[godplz].val[1]=-1;data[godplz].val[2]=-1;
                    }
                    if(data[godplz].val[4]>1){data[godplz].val[4]--;}
                    else{
                        data[godplz].val[3]=-1;data[godplz].val[4]=-1;
                    }
                }
            }
            else{
                cout<<"No Trade"<<endl;
            }
        }

    }

};


int main() {
    bool foundDollar = false;
    std::vector<std::string> words;   
    std::string loadit;
    Receiver rcv; 

    while (!foundDollar) {
    std::string message = rcv.readIML();
    loadit.append(message);
    if (message.find('$') != std::string::npos) {
        foundDollar = true;
    }
    }

    for (int i=0; i<loadit.length(); i++) {
    if (loadit[i] == '\r' || loadit[i] == '\n') {
        loadit[i] = '*';
    }
    }        
    std::istringstream iss(loadit);
    std::string word;
    while (std::getline(iss,word,'*')) {
    words.push_back(word);
    }
    cout<<words.size()<<endl;
    for (int i=0; i<words.size(); i++) {
    std::cout<<words[i]<<"\r\n";
    }
    rcv.terminate();
    mymap orderbook;
    for(auto c:words){
        cout<<c<<endl;
        // if(c=="$") continue;
        istringstream ss(c);
        vector<string> tokens;
        for(string token;getline(ss,token,' ');){
            tokens.push_back(token);
        }
        orderbook.insert(tokens[0],stoi(tokens[1]),(tokens[2][0]=='b'?0:1));
    }
    return 0;
}
