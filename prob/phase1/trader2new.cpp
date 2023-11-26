#include "receiver.h"
#include<vector>
#include<string>
using namespace std;
int totalprofit=0;
class custom_map {
    private:
    struct pairval{
        string str="";int val;
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
        // cout<<temp.data.size()<<" "<<temp1.data.size()<<endl;
        // if(temp.data.size()<temp1.data.size()) {cout<<"here\n";custom_map temp2=temp;temp=temp1;temp1=temp2;}
        for(auto c:temp.data){ //this parsing might not be allowed as data is declared as a private variable
            if(c.str=="") continue;
            if(temp1.data[temp1.find(c.str)].str==c.str){
                if(temp1.data[temp1.find(c.str)].val==c.val)
                continue;
                else return 0;
            }
            else return 0;
        }
        for(auto c:temp1.data){ //this parsing might not be allowed as data is declared as a private variable
            if(c.str=="") continue;
            if(temp.data[temp.find(c.str)].str==c.str){
                // cout<<"in here";
                if(temp.data[temp.find(c.str)].val==c.val)
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
        for(auto c:temp1.data){ //this parsing might not be allowed as data is declared as a private variable
            if(c.str=="") continue;
            if(temp.data[temp.find(c.str)].str==c.str){
                if(temp.data[temp.find(c.str)].val==c.val*-1)
                continue;
                else return 0;
            }
            else return 0;
        }
        return 1;
    }
};
class mymap {
    private:
    vector< pair< pair<custom_map, int>, bool> >orderbooknew;
    vector< pair<custom_map, int >> orderbook;
    vector<int> arbcycle;
    void printarbcycle(){
        for(auto k:arbcycle){
            cout<<k<<" ";
        }
        cout<<endl;
    }
    void checkcancellations(){
        int price=orderbook[orderbook.size()-1].second;
        custom_map temp=orderbook[orderbook.size()-1].first;
        for(int i=0;i<arbcycle.size()-1;i++){
            custom_map temp1=orderbook[arbcycle[i]].first;
            int price1=orderbook[arbcycle[i]].second;
            bool flag=temp.compare_map(temp,temp1); //we can avoid passing temp here (if time remove)
            if(flag==1){
                // cout<<"flag was 1 ";

                if(price<price1) {arbcycle.pop_back();break;} //ig this erase is wrong this begin() is wrong
                else if(price>price1) {arbcycle.erase(arbcycle.begin()+i);}
                // printarbcycle();
            }
            else{
                bool flag1=temp.opposite(temp,temp1);
                if(flag1==1 && price+price1==0){
                arbcycle.erase(arbcycle.begin()+i);arbcycle.pop_back();
            }
            }
            // cout<<"inside for loop for checkcancellation ";printarbcycle();
        }
    }
    void generate(vector<int>arbcycle,vector<int> &current, vector<vector <int>> &subsets, int j,int index){
        if (current.size() == j) {
        subsets.push_back(current);
        // cout<<"{";
        // for(auto c: current){
        //     cout<<c<<",";
        // }
        // cout<<"}\n";
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
        for(auto k:total.data){
            if(k.str!="" && k.val!=0) return 0; 
        }
        return profit;
    }
    void getarbitrage(){
        vector<int> newboy=arbcycle;
        newboy.pop_back();
        vector<vector<int>>subsets;
        for(int j=1;j<=arbcycle.size()-1;j++){
            makesubset(newboy,subsets,j); //check if the subsets are forming correctly
        }
        for(auto& k:subsets){
            k.push_back(arbcycle[arbcycle.size()-1]);
        }
        int maxprofit=0;
        int profit=0;
        vector<int> maxsubset; 
        // cout<<"inside getarbitrage checking subsets"<<endl;
        for(auto c:subsets){
            // for(auto k:c){
            //     cout<<k<<" ";
            // }
            // cout<<endl;
            profit=checkarbitrage(c);
            if(profit>maxprofit) {
                maxprofit=profit;
                maxsubset=c;
            }
        }
        // cout<<"maxprofit "<<maxprofit<<" ";
        // for(auto k:maxsubset){
        //         cout<<k<<" ";
        //     }
        //     cout<<endl;
        for(auto l : maxsubset){
            auto it=find(arbcycle.begin(),arbcycle.end(),l);
            if(it!=arbcycle.end()){
                arbcycle.erase(it);
            }
        }
        if(maxprofit>0){
            while(!maxsubset.empty()){
                custom_map temp=orderbook[maxsubset[maxsubset.size()-1]].first;
                int pricetag=orderbook[maxsubset[maxsubset.size()-1]].second;
                bool type=orderbooknew[maxsubset[maxsubset.size()-1]].second;
                for(auto z : temp.data){
                    if(z.str=="") continue;
                    if(type==1) cout<<z.str<<" "<<z.val*-1<<" ";
                    else
                    cout<<z.str<<" "<<z.val<<" "; 
                }
                if(type==1) cout<<pricetag*-1<<" ";
                else
                cout<<pricetag<<" ";
                cout<<(type==0?'s':'b')<<'#'<<endl;
                maxsubset.pop_back();
            }
            // cout<<maxprofit<<endl;
            totalprofit+=maxprofit;
        }
        else{
            cout<<"No Trade\n";
        }
    }
    public:
    mymap(){}
    void insert(vector< pair<string,int> > orders, int price, bool type){
        custom_map temp;
        for(auto c:orders){
            temp.insert(c.first,c.second);
        }
        orderbooknew.push_back({{temp,price},type});
        orderbook.push_back({temp,price});
        arbcycle.push_back(orderbook.size()-1);
        // printarbcycle();
        checkcancellations();
        // printarbcycle();
        getarbitrage();
        // printarbcycle();
    }
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
        // cout<<"nextline"<<endl;
        market.insert(orders,price,type);
    }
    cout<<totalprofit<<endl;
    return 0;
}
