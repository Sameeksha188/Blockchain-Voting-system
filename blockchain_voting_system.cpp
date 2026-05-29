#include<iostream>
#include <String>
#include<ctime>
#include<map>
#include<vector>
#include<functional>
using namespace std;
class Block{
    public:
    int index;
    string voter_id;
    string candidate;
    string previous_hash;
    string current_hash;
    string timestamp;
    Block(int i,string voter,string candidate_chosen,string prev_hash){
         index = i;
         voter_id = voter;
         candidate = candidate_chosen;
         previous_hash = prev_hash;
         timestamp = get_timestamp();
         current_hash = makehash();
         
    }
    string get_timestamp() {
        time_t now = time(0);
        string ts = ctime(&now);
        if (!ts.empty()&&ts.back()=='\n'){
            ts.pop_back();
        }
        return ts;
    }
    string makehash(){
        string data = to_string(index)+voter_id+candidate+previous_hash+timestamp;
        hash<string>to_hash;
        return to_string(to_hash(data));
    }
};
class Blockchain{
    public:
    vector<Block>chain;
    vector<string>registered_voters = {"VOTER101","VOTER102","VOTER103"};
    vector<string>candidates = {"Alice","Bob","Charlie"};
    Blockchain(){
        create_genesis_block();
    }
    void create_genesis_block(){
        Block genesis(0,"GENESIS","GENESIS","0");
        chain.push_back(genesis);
    }
    bool check(const vector<string>&list,const string &item){
        for (string item_in_list : list){
            if (item_in_list == item){
                return true;
            }
        }
        return false;
    }
    bool has_Voted(string voter_id_given){
        for (int i =1;i<chain.size();i++){
            if (chain[i].voter_id == voter_id_given){
                return true;
            }
        }
        return false;
    }
    void add_vote(string voter_id,string candidate){
        if (check(registered_voters,voter_id)==false){
            cout<<"Voter ID not registered"<<endl;
            return;
        }
        if (check(candidates,candidate)==false){
            cout<<"candidate doesn't exist"<<endl;
            return;
        }
        if (has_Voted(voter_id)==true){
            cout<<"Voter has already voted"<<endl;
            return;
        }
        string previous_hash = chain.back().current_hash;
        Block current_block(chain.size(),voter_id,candidate,previous_hash);
        chain.push_back(current_block);
        cout<<"Vote added successfully"<<endl;
    }
    bool is_valid(){
        for (int i =1;i<chain.size();i++){
            if (chain[i].previous_hash != chain[i-1].current_hash){
                cout<<"previous hash mismatched"<<endl;
                return false;
            }
            if (chain[i].current_hash != chain[i].makehash()){
                cout<<"Hash doesn't match"<<endl;
                return false;
            }
            
        }
        return true;
    }
    void count_votes(){
        map<string,int>count_votes;
        for (string c : candidates){
            count_votes[c]=0;
        }
        for (int i=1;i<chain.size();i++){
            count_votes[chain[i].candidate]++;
        }
        cout<< "Candidate : votes"<<endl;
        for (auto data : count_votes){
            cout<<data.first <<" : "<<data.second<<endl;
        }
    }
    void display_chain(){
        cout<<"--------------------------------------"<<endl;
        cout<<"Displaying the complete chain"<<endl;
        cout<<"--------------------------------------"<<endl;
        for (int i=1;i<chain.size();i++){
            cout<<"Block No: "<< chain[i].index<<endl;
            cout<<"Voter ID: "<<chain[i].voter_id<<endl;
            cout<<"Candidate choosen: "<<chain[i].candidate<<endl;
            cout<<"Timestamp: "<<chain[i].timestamp<<endl;
            cout<<"Previous hash: "<<chain[i].previous_hash<<endl;
            cout<<"Hash: "<<chain[i].current_hash<<endl;


            cout<<"--------------------------------------"<<endl; 
        }
        cout<<"=========== End of the chain==========="<<endl;

    }
    void declare_winner(){
        int winner_votes =0;
        string winner;
        map<string,int>count_votes;
        for (string c : candidates){
            count_votes[c]=0;
        }
        for (int i=1;i<chain.size();i++){
            count_votes[chain[i].candidate]++;
        }
        for (string c : candidates){
            if (count_votes[c]>winner_votes){
                winner_votes = count_votes[c];
                winner = c;
            }
        }
        cout<<"WINNER : "<<winner<<endl;
    }
};
int main(){
    Blockchain myvote;
    myvote.add_vote("VOTER101","Alice");
    myvote.add_vote("VOTER102","Charlie");
    myvote.add_vote("VOTER103","Alice");
    cout<<"Before tampering"<<endl;
    myvote.count_votes();
    myvote.display_chain();
    myvote.declare_winner();
    myvote.chain[1].candidate = "Bob";
    cout<<"After tampering"<<endl;
    myvote.count_votes();
    myvote.is_valid(); 
    return 0;
}