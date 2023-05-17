
//BUSRA CALISKAN 504211507

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>
#include <math.h>
using namespace std;
//it is used for dates and times of str 
struct Interval {
    string start;
    string end;
};
//it is used for salons which has capacity and list of end and begin times
struct Salon {
    vector<Interval> times;
    int capacity;
};
//it is used for places which has salons and list of end and begin dates 
struct Place {
    vector<Interval> dates;
    map<string, Salon> salons;
};
//it is used for kruskal and WIS. I created vector has Weighted str type
struct Weighted {
    string starto; //start date or time
    string endo; //end date or time
    int capacito; // capacity as rules
    string name; //name
};
//it is used just for assets.txt
struct Asset {
    string name;
    int price;
    double value;
};
//it keeps Place name and its Revenue.
struct Rules {
    std::map<string, int> values;

    int getValue(const string& name) {
        if (values.count(name) > 0) {
            return values[name];
        } else {
            
            return -1;
        }
    }
};
Rules rules;

map<string, Place> places;//it keeps all schedules which has all places and its salons
//it make string time to int time which i used compare as their times
int timeToMinutes(string& times) {
    stringstream ss(times);
  int hours, minutes;
  char separator;
  ss >> hours >> separator >> minutes;
  int total_minutes = hours * 60 + minutes;
    return total_minutes;
}
//it make string dates to int dates which i used compare as their dates
int dateToDays(string& dates) {
    stringstream ss(dates);
  int months, days;
  char separator;
  ss >> days >> separator >> months;
  int total;
  if(months==4||months==6||months==9||months==11){
    total= months * 30 + days;
  }
  if(months==1||months==3||months==5||months==7||months==8||months==10){
    total= months * 31 + days;
  }
  if(months==2){
    total= months * 28 + days;
  }
    return total;
}
//compare end times for best for eachplace
bool myfunction1(Weighted& s1, Weighted& s2)
{
    return (timeToMinutes(s1.endo) < timeToMinutes(s2.endo));
}
//compare end dates for best tour
bool myfunction2(Weighted& s1, Weighted& s2)
{
    return(dateToDays(s1.endo)< dateToDays(s2.endo));
}
//return string of months
string getMonthName(int month) {
    string monthNames[] = {"January", "February", "March", "April", "May", "June",
                           "July", "August", "September", "October", "November", "December"};
    return monthNames[month - 1];
}
//change month as desired view
string monthtrans(string& a){
    stringstream ss(a);
    int months, days;
    char separator;
    ss >> days >> separator >> months;
    string s=to_string(days)+" "+getMonthName(months);
    return s;
}
//for availability_intervals.txt ,here we create places and their dates and keep as map
void parse_availability_intervals(string filename) {
    ifstream input(filename);
    string line;
    getline(input, line); // skip first line
    while (getline(input, line)) {
        stringstream ss(line);
        string place_name, start, end;
        ss >> place_name >> start >> end;
        Interval interval = { start, end };
        Place& place = places[place_name];
        place.dates.push_back(interval);
    }
    input.close();
}
//for daily_schedule.txt ,here we create salons and their times and keep as map and put belonging places already created
void parse_daily_schedule(string filename) {
    ifstream input(filename);
    string line;
    getline(input, line); // skip first line
    while (getline(input, line)) {
        stringstream ss(line);
        string place_name,salon_name, start, end;
        ss >>place_name>> salon_name >> start >> end;
        Interval interval = {start, end};
        Salon& salon = places[place_name].salons[salon_name];
        salon.times.push_back(interval);
    }
    input.close();
}
//for capacity.txt, here we put capacity of salons
void parse_capacity(string filename) {
    ifstream input(filename);
    string line;
    getline(input, line); // skip first line
    while (getline(input, line)) {
        stringstream ss(line);
        string place_name,salon_name, cap;
        ss >> place_name>>salon_name >> cap;
        Salon& salon = places[place_name].salons[salon_name];
        salon.capacity = stoi(cap);
    }
    input.close();
}
//for assets.txt, here we create vector assest 
vector<Asset> readAssetsFromFile(string filename) {
    vector<Asset> assets;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        getline(file, line); // Skip the header line

        while (getline(file, line)) {
            istringstream iss(line);
            Asset asset;
            iss >> asset.name >> asset.price >> asset.value;
            assets.push_back(asset);
        }

        file.close();
    } else {
        cout << "Failed to open the file: " << filename << std::endl;
    }

    return assets;
}
//printing map<string, Place> places keeps all schedules which has all places and its salons
void printing(map<string, Place>& pl){
    for(auto a:pl){
        cout<<a.first<<endl;
        for(auto b:a.second.salons){
            cout<<"\t"<<b.first<<" ";
            cout<<b.second.capacity<<endl;
            for(auto c:b.second.times){
                cout<<"\t\t"<<c.start<< " "<<c.end<<endl;
            }
        }
    } 
    for(auto a:pl){
        cout<<a.first<<endl;
        for(auto b:a.second.dates){
            cout<<"\t"<<b.start<<" "<<b.end;
        }
        cout<<endl;
    }
}
//for best each place , we create WIS
void WIS1(map<string, Salon>& sal,vector<Weighted>& result,int& t){
    
    vector<Weighted> jobs; //create vector for using WIS and put and keep weighted str which has start end capacity and name of salons
    for (auto s : sal) {
        for (auto a:s.second.times) {
            Weighted i;
            i.starto=a.start;
            i.endo=a.end;
            i.name=s.first;
            i.capacito=s.second.capacity;
            jobs.push_back(i);
        }
    }
    sort(jobs.begin(),jobs.end(),myfunction1); //sort vector as their end times
    int n = jobs.size();
    if (n == 0) {
        return;
    }
    vector<int> tasks[n];//n-1.vector of task keeps desired WIS path
    int maxProfit[n];//it keeps all max profit
    for (int i = 0; i < n; i++)
    {
        maxProfit[i] = 0; //first profit is 0
        for (int j = 0; j < i; j++)
        {
            // update i.salon if the j. salon is non-conflicting and leading to max profit
            if (timeToMinutes(jobs[j].endo) <= timeToMinutes(jobs[i].starto) && maxProfit[i] < maxProfit[j])
            {
                tasks[i] = tasks[j];
                maxProfit[i] = maxProfit[j];
            }
        }
        // current task with i.salon
        tasks[i].push_back(i);
        maxProfit[i] += jobs[i].capacito;
    }
    // find an index with the maximum profit
    int index = 0;
    for (int i = 1; i < n; i++)
    {
        if (maxProfit[i] > maxProfit[index]) {
            index = i;
        }
    }
    //put infos and return result
    for (int i: tasks[index])
    {
        t=t+jobs[i].capacito; //calculate Revenue
        Weighted n;
        n.name=jobs[i].name;
        n.starto=jobs[i].starto;
        n.endo= jobs[i].endo;
        n.capacito= jobs[i].capacito;
        result.push_back(n);
    }
}
//knapsack algo
pair<float, vector<string>> knapsack(vector<Asset>& assets, int totalValue) {
    int n = assets.size();
    vector<vector<float>> dp(n + 1, std::vector<float>(totalValue + 1, 0.0));

    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j <= totalValue; ++j) {
            if (assets[i - 1].price <= j) {
                dp[i][j] = max(dp[i - 1][j], static_cast<float>(assets[i - 1].value + dp[i - 1][j - assets[i - 1].price]));
            } else {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }

    vector<string> selectedAssets;
    int j = totalValue;
    float maxValue = dp[n][totalValue];
    for (int i = n; i > 0 && j > 0; --i) {
        if (dp[i][j] != dp[i - 1][j]) {
            selectedAssets.push_back(assets[i - 1].name);
            maxValue =maxValue- assets[i - 1].value;
            j =j- assets[i - 1].price;
        }
    }

    return {dp[n][totalValue], selectedAssets};
}
//WIS FOR BEST TOUR
vector<int> y;
//FIND NONOVERLAP ELEMENT
int nonover2(int j,vector<Weighted>&b){
    for(int i=j-1;i>=0;i--){
        if(dateToDays(b[i].endo)<=dateToDays(b[j].starto)){
            return i;
        }
    }
    return -1;
}
//COMPUTE OPTİIMAL
int Compute_Opt2(int j,vector<Weighted>&b){
    if(j<0){
        return 0;
    }
    else if(y[j]==0){
       
        y[j]=max(b[j].capacito+Compute_Opt2(nonover2(j,b),b),Compute_Opt2(j-1,b));
    }
    return y[j];
}
//FIND PATH
int Find_Solution2(int j,vector<Weighted>&b,vector<Weighted>&results){
    if(j<0){
        return 0;
    }
    else if(b[j].capacito+Compute_Opt2(nonover2(j,b),b)>Compute_Opt2(j-1,b)){
        results.push_back(b[j]);
        return Find_Solution2(nonover2(j,b),b,results);
        
    }
    return Find_Solution2(j-1,b,results);
    
}
pair<int, vector<Weighted>> WIS2(map<string, Place>& sal){
    vector<Weighted> jobs; 
    
    for (auto s : sal) {
        for (auto a:s.second.dates) {
            Weighted i;
            i.starto=a.start;
            i.endo=a.end;
            i.name=s.first;
            int diff=dateToDays(a.end)-dateToDays(a.start);//calculate diff of days
            i.capacito=diff*rules.getValue(s.first); //calculate each revenue
            jobs.emplace_back(i);
            y.push_back(0);
        }
    }
    y.push_back(0);
    sort(jobs.begin(),jobs.end(),myfunction2);
    
    int n = jobs.size();
    
    vector<Weighted> result;
    
    Compute_Opt2(n- 1, jobs);
    Find_Solution2(n-1,jobs,result);

    reverse(result.begin(), result.end());

    return { y[n - 1], result };
}
void creating(string s, string n) {
    fstream filename;
    filename.open(s, ios::out);
    filename << n;
    filename.close();
}
int main(int argc, char* argv[]) {
    string caseNo = "1";
    if(argc == 2){
        caseNo = argv[1];
    }
    string input_path = "/inputs/" + caseNo + "/";
    string e=input_path + "daily_schedule.txt";
    string b=input_path + "capacity.txt";
    string c=input_path + "availability_intervals.txt";
    string d=input_path + "assets.txt";
    parse_availability_intervals(c);
    parse_daily_schedule(e);
    parse_capacity(b);
    vector<Asset> assets = readAssetsFromFile(b);
    //printing(places);
    string s1="";
    for(auto a:places){
        s1=s1+a.first+"-->";
        int t=0;
        vector<Weighted> s; 
        WIS1(a.second.salons,s,t);
        s1+=to_string(t)+"\n";
        rules.values[a.first]=t;
        for(auto b:s){
            s1+=a.first+"\t\t"+b.name+"\t" + b.starto + "\t" + b.endo + "\n";
        }
        s1+="\n";
    }
    
    auto [optValue, s] = WIS2(places);
    string s2="";
    s2+= "Total Revenue −−> " + to_string(optValue) + "\n";
    for(auto b:s){
        s2+=b.name+"\t" + monthtrans(b.starto) + "\t" + monthtrans(b.endo) + "\n";
    }
    string s3="";
    auto [ch, selectedAssets] = knapsack(assets, optValue);
    s3+= "Total Value --> " + to_string(ch) + "\n";
    for (const std::string& asset : selectedAssets) {
        s3+= asset + "\n";
    }
    string bestForEachPlace = "/outputs/" + caseNo+"/"+"best_for_eachplace.txt";
    string bestTour = "/outputs/" + caseNo+"/"+"best_tour.txt";
    string upgradeList ="/outputs/" + caseNo+"/"+"upgrade_list.txt";
    creating(bestForEachPlace,s1);
    creating(bestTour,s2);
    creating(upgradeList,s3);
    
    return 0;
}
