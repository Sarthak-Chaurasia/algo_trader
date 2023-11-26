#include "receiver.h"
#include<vector>
#include<string>
using namespace std;
class custom_map {
    private:
    struct pairval{
        string str;int val;
    };
    long int hash(string str){
        long int hashval=10;
        for(auto c:str){
            hashval = (30*hashval) + static_cast<size_t>(c);
        }
        return hashval; 
    }
    long int index(string key){ //if you pass a key into this will return the postion of the key otherwise will give new index for the key
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
            return godplz;
    }
    public:
    vector<pairval> data;
    custom_map(size_t initval=100) : data(initval, pairval{"",}){}
    void insert(string str1, int val1){
        long int indexval=index(str1);
        data[indexval].str=str1;
        data[indexval].val=val1;
    }
    long int find(string key){
        return index(key);
    }
    bool compare_map(custom_map temp, custom_map temp1){
        for(auto c:temp.data){ //this parsing might not be allowed as data is declared as a private variable
            if(c.str=="") continue;
            if(temp1.data[temp1.find(c.str)].str==c.str){
                if(temp1.data[temp1.find(c.str)].val==c.val)
                continue;
                else return 0;
            }
            else return 0;
        }
        return 1;
    }
    bool opposite(custom_map temp, custom_map temp1){
        for(auto c:temp.data){ //this parsing might not be allowed as data is declared as a private variable
            if(c.str=="") continue;
            if(temp1.data[temp1.find(c.str)].str==c.str){
                if(temp1.data[temp1.find(c.str)].val==c.val*-1)
                continue;
                else return 0;
            }
            else return 0;
        }
        return 1;
    }
};
//key=curr price,active buy order price ,buy order count, active sell order price, sell order count
class mymap {
    private:
    // struct pairval{
    //     string str;
    //     int bP=-1,sP=-1,bC=-1,sC=-1;
    //     vector<int> buy={},sell={};
    // };
    // vector<pairval> stocklist= vector<pairval>(100);
    vector< pair<custom_map, int >> orderbook;
    // vector<tuple <vector < pair<string, int> >, int, bool> > orderbook;
    vector<int> arbcycle;
    
    void checkcancellations(){
        int price=orderbook[orderbook.size()-1].second;
        custom_map temp=orderbook[orderbook.size()-1].first;
        for(int i=0;i<arbcycle.size()-1;i++){
            custom_map temp1=orderbook[arbcycle[i]].first;
            int price1=orderbook[arbcycle[i]].second;
            bool flag=temp.compare_map(temp,temp1); //we can avoid passing temp here (if time remove)
            if(flag==1){
                if(price<price1) {arbcycle.pop_back();break;} //ig this erase is wrong this begin() is wrong
                else if(price>price1) {arbcycle.erase(arbcycle.begin()+i);}
            }
            else{
                bool flag1=temp.opposite(temp,temp1);
                if(flag1==1){
                arbcycle.erase(arbcycle.begin()+i);arbcycle.pop_back();
            }
            }
        }
    }
    void generate(vector<int>arbcycle,vector<int> &current, vector<vector <int>> &subsets, int j,int index){
        if (current.size() == j) {
        subsets.push_back(current);
        cout<<"{";
        for(auto c: current){
            cout<<c<<",";
        }
        cout<<"}\n";
        return;
    }
    for (int i = index; i < arbcycle.size(); ++i) {
        current.push_back(arbcycle[i]);
        generate(arbcycle, current, subsets, j, i+1);
        current.pop_back();
    }
    }
    void makesubset(vector<int>arbcycle, vector<vector <int>> &subsets, int j ){
        vector<int> current;
        generate(arbcycle,current,subsets,j,0);
    }

    int checkarbitrage(vector<int> c){
        int profit=0;
        custom_map total;
        for(auto s:c){
            custom_map t=orderbook[s].first;
            for(int i=0;i<t.data.size();i++){
                if(t.data[i].str=="") continue;
                if(total.data[total.find(t.data[i].str)].str=="") {total.insert(t.data[i].str,t.data[i].val);}
                else{
                    total.data[total.find(t.data[i].str)].val+=t.data[i].val;
                }
            }
            profit+=orderbook[s].second;
        }
        return profit;
    }
    void getarbitrage(){
        vector<int> newboy=arbcycle;
        newboy.pop_back();
        vector<vector<int>>subsets;
        for(int j=2;j<=arbcycle.size()-1;j++){
            makesubset(newboy,subsets,j); //check if the subsets are forming correctly
        }
        for(auto k:subsets){
            k.push_back(arbcycle[arbcycle.size()-1]);
        }
        int maxprofit=0;
        int profit=0;
        vector<int> maxsubset; 
        for(auto c:subsets){
            profit=checkarbitrage(c);
            if(profit>maxprofit) {
                maxprofit=profit;
                maxsubset=c;
            }
        }
        if(maxprofit>0){
            
        }
        else{
            cout<<"No Trade\n";
        }
    }
    public:
    mymap(){}
    void insert(vector< pair<string,int> > orders, int price){
        custom_map temp;
        for(auto c:orders){
            temp.insert(c.first,c.second);
        }
        orderbook.push_back({temp,price});
        arbcycle.push_back(orderbook.size()-1);
        checkcancellations();
        getarbitrage();
    }
        
        
    // cout<<key<<" "<<price<<" "<<type<<" and "<<data[godplz].str<<" "<<data[godplz].val[0]<<" "<<data[godplz].val[1]<<" "<<data[godplz].val[2]<<" "<<data[godplz].val[3]<<" "<<data[godplz].val[4]<<"  ";
    //     if(data[godplz].val[0]==-1) {
    //         data[godplz].val[0]=price;
    //         cout<<key<<" "<<to_string(price)<<" "<<(type==1?"b":"s")<<endl; //terneray here is opposite
    //     }
    //     else if(type==0){
    //         if(data[godplz].val[1]==-1 || data[godplz].val[1]<=price){ //ig if there is an active order then either it will remain or new will you can change that
    //             if(data[godplz].val[1]!=price){
    //                 data[godplz].val[1]=price;
    //                 data[godplz].val[2]=1;
    //             }
    //             else{
    //                 data[godplz].val[2]++;
    //             }
    //             if(data[godplz].val[3]==-1 || data[godplz].val[3]!=price){
    //                 if(data[godplz].val[0]<price){
    //                     cout<<key<<" "<<to_string(price)<<" s"<<endl;
    //                     data[godplz].val[0]=price;
    //                     if(data[godplz].val[2]==1){
    //                         data[godplz].val[1]=-1;
    //                         data[godplz].val[2]=-1;
    //                     }
    //                     else{
    //                         data[godplz].val[2]--;
    //                     }
    //                 }
    //                 else{
    //                     cout<<"No Trade\n";
    //                 }
    //             }
    //             else{
    //                 cout<<"No Trade\n"; //trade happening between some two but not us so curr price remains same
    //                 if(data[godplz].val[4]>1) {data[godplz].val[4]--;}
    //                 else{
    //                     data[godplz].val[3]=-1;data[godplz].val[4]=-1;
    //                 }
    //             }
    //         }
    //         else{
    //             cout<<"No Trade\n";
    //         }
    //     }
    //     else if(type==1){
    //         if(data[godplz].val[3]==-1 || data[godplz].val[3]>=price){ //ig if there is an active order then either it will remain or new will you can change that
    //             if(data[godplz].val[3]!=price){
    //                 data[godplz].val[3]=price;
    //                 data[godplz].val[4]=1;
    //             }
    //             else{
    //                 data[godplz].val[4]++;
    //             }
    //             if(data[godplz].val[1]==-1 || data[godplz].val[1]!=price){
    //                 if(data[godplz].val[0]>price){
    //                     cout<<key<<" "<<to_string(price)<<" b"<<endl;
    //                     data[godplz].val[0]=price;
    //                     if(data[godplz].val[4]==1){
    //                         data[godplz].val[3]=-1;
    //                         data[godplz].val[4]=-1;
    //                     }
    //                     else{
    //                         data[godplz].val[4]--;
    //                     }
    //                 }
    //                 else{
    //                     cout<<"No Trade\n";
    //                 }
    //             }
    //             else{
    //                 cout<<"No Trade\n"; //trade happening between some two but not us so curr price remains same
    //                 if(data[godplz].val[2]>1) {data[godplz].val[2]--;}
    //                 else{
    //                     data[godplz].val[1]=-1;data[godplz].val[2]=-1;
    //                 }
    //             }
    //         }
    //         else{
    //             cout<<"No Trade\n";
    //         }
    //     }

    // }

};


int main() {

    bool foundDollar = false;
    std::vector<std::string> words;   
    std::string loadit;
    Receiver rcv; 
    //A 1 B -1 C 4 100 b#
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
    //std::vector<std::string> words;
    while (std::getline(iss,word,'*')) {
    words.push_back(word);
    }
    // cout<<words.size()<<endl;
    // for (int i=0; i<words.size(); i++) {
    // std::cout<<words[i]<<"\r\n";
    // }
    rcv.terminate();

    mymap market;
    // char delimeter=' ';
    for(auto c:words){
        if(c=="$") continue;

        istringstream ss(c);
        vector<string> tokens;
        for(string token;getline(ss,token,' ');){
            tokens.push_back(token);
        }
        vector< pair<string,int> > orders;
        int price; bool type;
        type=(tokens[tokens.size()-1][0]=='b'?0:1);
        if(type==0)price=stoi(tokens[tokens.size()-2]);
        else price=stoi(tokens[tokens.size()-2])*-1;
        for(int i=0;i<((tokens.size()-2)/2);i++){
            if(type==0)orders.push_back({tokens[2*i],stoi(tokens[2*i+1]) });
            else orders.push_back({tokens[2*i],stoi(tokens[2*i+1])*-1 });
        }
        cout<<type<<endl;
        market.insert(orders,price);
    }
    return 0;
}
