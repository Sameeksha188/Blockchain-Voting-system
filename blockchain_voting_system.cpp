#include<iostream>
#include <string>
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
    long voting_deadline;
    vector<Block>chain;
    vector<string>registered_voters = {"VOTER101","VOTER102","VOTER103"};
    vector<string>candidates = {"Alice","Bob","Charlie"};
    Blockchain(){
        voting_deadline = time(0)+300;
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
    bool validate_vote(string voter_id,string candidate){
        if (check(registered_voters,voter_id)==false){
            cout<<"[ERROR] Voter ID not registered"<<endl;
            return false;
        }
        if (check(candidates,candidate)==false){
            cout<<"[ERROR] candidate does not exist"<<endl;
            return false;
        }
        if (has_Voted(voter_id)==true){
            cout<<"[ERROR] Voter has already voted"<<endl;
            return false;
        }
        long currentTime = (long)time(0);
        if (currentTime > voting_deadline) {
            cout << "Rejected: Voting period has ended! (Deadline reached)" << endl;
            return false;
        }
        return true;
    }
    void add_vote(string voter_id,string candidate){
        if (validate_vote(voter_id,candidate)==false){
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
                cout<<"[ALERT]TAMPERING DETECTED"<<endl;
                cout<<"previous hash mismatched"<<endl;
                return false;
            }
            if (chain[i].current_hash != chain[i].makehash()){
                cout<<"[ALERT] TAMPERING DETECTED"<<endl;
                cout<<"Hash doesn't match"<<endl;
                return false;
            }
        }
        cout<<"It is a valid chain"<<endl;
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
    void declare_winner() {
    map<string,int> count_votes;

    for (string c : candidates) {
        count_votes[c] = 0;
    }

    for (int i = 1; i < chain.size(); i++) {
        count_votes[chain[i].candidate]++;
    }

    int winner_votes = 0;

    // Find maximum votes
    for (string c : candidates) {
        if (count_votes[c] > winner_votes) {
            winner_votes = count_votes[c];
        }
    }

    // Find all candidates with maximum votes
    vector<string> winners;

    for (string c : candidates) {
        if (count_votes[c] == winner_votes) {
            winners.push_back(c);
        }
    }

    if (winners.size() == 1) {
        cout << "WINNER : " << winners[0] << endl;
    }
    else {
        cout << "ELECTION TIED BETWEEN: ";
        for (string c : winners) {
            cout << c << " ";
        }
        cout << endl;
    }
}
};
int main(){
    Blockchain myvote;
    myvote.add_vote("VOTER101","Alice");
    myvote.add_vote("VOTER102","Charlie");
    myvote.add_vote("VOTER101","Alice");
    myvote.add_vote("VOTER111","bob");
    myvote.add_vote("VOTER103","Lexi");
    cout<<"---Before tampering---"<<endl;
    myvote.count_votes();
    myvote.display_chain();
    myvote.declare_winner();
    myvote.chain[1].candidate = "Bob";
    cout<<"---After tampering---"<<endl;
    myvote.is_valid();
   
     
    return 0;
}