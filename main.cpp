/******************************************************************************


*******************************************************************************/
#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <bits/stdc++.h>
#include <string> 
#include <chrono>
#include <ctime>
#include <fstream>
using namespace std;

struct Node {
    public:
        int x, y, p; //x ,y and p are x-coordinates,y-coordinates and strength of kids
        
};
class Graph{ 
    private:
        int V; //num of kids
    public:
        
        Graph(int); //constructor
        bool visited[1000]; //keeping visited nodes
        int parent[1000]; // save parents of nodes for dfs
        vector<list<int>>graph; //adcany list
        vector<vector<int>> adj_matrix; //adjancy matrix
        string str_dfs=""; //saving path for dfs
        list<int> dic; //saving path for dfs before and after source
        bool dfs(int, int,int); //recursively DFS algorithm and it return whether cycle is or not
        void addEdge(int,int); //creating graph
        string BFS(int,int); //returning BFS path and BFS algoritm
        string find_cycle(int ); //save last path DFS after recursively searching
        void print_adjMatrix(); //for print adj_matrix
        void print_vector_list(vector<list<int>>); //for print adj lists
};
//find distannce of two nodes
int distance(Node a, Node b) {
    return pow(a.x - b.x, 2) + pow(a.y - b.y, 2);
}
void file_adj(vector<vector<int>>);
void file_bfs(string );
void file_dfs(string);

int main(int argc, char* argv[])

{
    system("clear");
    //auto start = chrono::steady_clock::now();
    vector<vector<int>>matrix; 
    int source; //source kid
    int target; //taget kid
    int n; //kids number
    ifstream file("./graphs/case2/input_2.txt");//argv[1] "./graphs/case9/input_9.txt"
    // Read the file line by line
    string line;
    file>>n>>source>>target ;
    for(int i = 0; i< n; ++i){
        int k,l,m;
        vector<int>row;
        file>>k>>l>>m;
        row.insert(row.begin(),k);
        row.insert(row.begin()+1,l);
        row.insert(row.begin()+2,m);
        matrix.push_back(row);
        
    }    
    file.close();
    Graph g(n);//create Graph g
    vector<Node>not_real=vector<Node>(n);//temporary vector saving kids nodes
    for (int i = 0; i < n; i++) {
        not_real[i].x=matrix[i][0];
        not_real[i].y=matrix[i][1];
        not_real[i].p=matrix[i][2];
    }
    
    //adding nodes and connect them in graph
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j){
                continue;
            }
            
            if (distance(not_real[i],not_real[j]) <= not_real[i].p && distance(not_real[i],not_real[j]) <= not_real[j].p){
                        g.addEdge(i, j);
            }
            
        }
    }
   
    file_adj(g.adj_matrix); //create adjancy matrix on txt file
    string str=g.BFS(source,target);
    cout<<str<<endl;
    file_bfs(str); //create BFS txt
    string s=g.find_cycle(source); 
    //auto finish = chrono::steady_clock::now(); //end of time to calculate runtime
    file_dfs(s); //create DFS file
    cout<<s<<endl;
     //double elapsed_seconds =chrono::duration_cast<chrono::duration<double> >(finish - start).count();  
    //cout<<elapsed_seconds<<endl;
    return 0;
}
Graph::Graph(int b)
{
    this->V = b;
    adj_matrix.resize(b, vector<int>(b));
    for (unsigned int i = 0; i < adj_matrix.size(); i++)
    {
        for (unsigned int j = 0; j < adj_matrix[i].size(); j++)
        {
            adj_matrix[i][j]=0;
        }    
        
    }
    graph.resize(b+1);
    
}
//if conditions are okey ,adding nodes
void Graph::addEdge(int u, int v)
{
    graph[u].push_back(v);
    adj_matrix[u][v]=1;
}

string Graph::BFS(int source, int target) {
    
    int distance[V+ 5]; //to calculte steps
    int nparent[V + 5]; //saving parent of nodes
    bool visit[V + 5]; //keep visit info
    queue<int> q; //BFS path
    q.push(source); // push source
    visit[source] = true;
    distance[source] = 0;
    while (!q.empty()) { //until q is empty
        int u = q.front(); //pop first element of queue
        q.pop();
        for (auto adj : graph[u]) { //looking for u.list of Graph
            
            if (!visit[adj]) { //if not visited
                visit[adj] = true; // visit is true
                distance[adj] = distance[u] + 1; //update current distance from u
                nparent[adj] = u; // record parent of v as u
                q.push(adj); //push current for path
            }
        }
        if (u == target) { //if u is target break
            break;
        }
    }
    //savig path as a string
    string h="";
    h=h+to_string(distance[target])+" ";
    vector<int> path; 
    int current = target;
    while (current != source) {
        path.push_back(current);
        current = nparent[current];
    }
    path.push_back(source);
    for (int i = path.size() - 1; i > 0; i--) {
        h=h+to_string(path[i])+"->";
    }
    h=h+to_string(path[0]);
    return h;
}

string Graph::find_cycle(int source) {
    visited[source] = true; //source is visited and its parent is nothing
    parent[source] = -1;
    auto i=graph[source].front(); //take first node of source. list
        
    if (dfs(i, source,source)) { //for first time control bool recursive DFS for source
            str_dfs=to_string(dic.size()+1)+" "+to_string(source)+"->";//if there is arrange path
            for(auto a: dic)
                str_dfs=str_dfs+to_string(a)+"->";
            str_dfs=str_dfs+to_string(source);    
    }
    else{//if there is no cycle , print -1
            str_dfs="-1";
    }
    
    return str_dfs;
}

bool Graph::dfs(int node, int source,int p) {
    visited[node] = true; 
    parent[node] = p; //saving p as parent of node
    dic.push_back(node); //push node 
    //if current is neighbour with source and distance is bigger than 2, return true
    if(dic.size()>1&&adj_matrix[node][source]==1){
                return true;
    }
    //looking for node neighbour list
    for (auto i : graph[node]) {
        if (!visited[i]) { //if not visted
            if (dfs(i, source,node)) { //and if there is DFS with current and node, return true
                return true;
                
            }
        } 
        
        if (parent[i] == source&& dic.size()>1) { //if parent of current is source and distance bigger than 2,return true
                return true;
        }
    }
    dic.pop_back(); //adding node is not suitable , pop_back
    return false;//return false 
}
void file_adj(vector<vector<int>>mat){
    ofstream adj("./graph.txt");
    adj<<"Graph:"<<endl;
    if(adj.is_open()){
        for(auto vec:mat){
            for(auto elem:vec){
                adj<<elem<<" ";
            }
            adj<<endl;
        }
        adj.close();
    }
    
    return;
}
void file_bfs(string s){
    ofstream jbfs("./bfs.txt");
    if(jbfs.is_open()){
        jbfs<<"BFS:"<<endl;
        jbfs<<s;
        
    }
    return;
}
void file_dfs(string s){
    ofstream jdfs("./dfs.txt");
    if(jdfs.is_open()){
        jdfs<<"DFS:"<<endl;
       
        jdfs<<s;
        
    }
    return;
}
    
    
 //for print adj list
void Graph::print_vector_list(vector<list<int>> gr){
    list<int>::iterator it;
    for (auto adjacent : gr) {
        adjacent.sort();
        for (it=adjacent.begin(); it!=adjacent.end(); ++it){
            cout << ' ' << *it;
            
        }
        cout << endl;
    }
}
//for print adjancy matrix
void Graph::print_adjMatrix(){
    int n=adj_matrix.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << adj_matrix[i][j] << " ";
        }
        cout << endl;
    }    
    
}   
    
