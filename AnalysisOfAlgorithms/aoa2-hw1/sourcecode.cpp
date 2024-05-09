/*Ahmet Göktug Sevinc
    150140120
*/
#include <iostream>
#include <string.h>
#include <deque>
#include <list>
#include <string>
#include <ctime>

using namespace std;

class object{

public:
    string name;
    object(string n){
        name = n;
    }
};

class node{
public:
    deque <node> Parent;
    int key;
    deque <object> ar;
    node(int i, object o1, object o2, object o3, object o4,  object o5, object o6, object o7, object o8, object o9);
};

node::node(int i, object o1, object o2, object o3, object o4, object o5, object o6, object o7, object o8, object o9){
        key = i;
        ar.push_back(o1);  ar.push_back(o2);  ar.push_back(o3);  ar.push_back(o4);  ar.push_back(o5);  ar.push_back(o6);  ar.push_back(o7); ar.push_back(o8); ar.push_back(o9);
}

class Graph{
    list <node> *adj;
    deque<node>Trace;
    list<node>*Parent;
    bool *visited;
    int Max;
    int nvisited;
public:
    Graph();
    void edge(int x, node y);
    void printResult();
    void printDA();
    void findpath(node& x, char* mode);
    void clearFlags();
};

Graph::Graph(){
    adj = new list<node>[16];
    Parent = new list <node>[17];
    visited = new bool[16];
}

void Graph::edge(int x, node y){
    adj[x].push_back(y);
}

void Graph::findpath(node& x, char* mode){
    deque <node> solve;
    bool check ;
    solve.push_front(x);  //x is the node that we want to find the path to final node
    Max = 0;
    nvisited = 0;
    while(!solve.empty()){
        if(solve.size()>Max){
            Max = solve.size();
        }
        node temp = solve.front();  //take front node from solve
        nvisited++;
        check = true;
        int k = temp.key;
        if(k==16){  //16 is the key of final node
           Trace.push_back(temp);  //Push final node to Trace
           break;
        }
        solve.pop_front();
        if(!visited[k]){
            visited[k] = true;  //mark visited[k] as true; for example if we are at 5th node, we will mark this node as visited to avoid coming back
            list<node>::iterator it =adj[k].begin();  //we will check the adjacency list of kth node

            while(it!=adj[k].end()){
                if(!visited[(*it).key]){   //if we have not visited the node that has edge with kth node
                    check = false;
                    Parent[(*it).key].push_back(temp);  // temp is the kth node; so this means Parent of the node that has edge with kth node is: the kth node;
                    if(strcmp(mode, "BFS")==0 || strcmp(mode, "bfs")==0){   //if the mode is BFS push_back the node that has connection with kth node
                        solve.push_back(*it);
                    }else if(strcmp(mode, "DFS")==0 || strcmp(mode, "dfs")==0){   //if the mode is DFS push_front the node that has connection with kth node
                        solve.push_front(*it);
                    }else{
                        cout << "Please enter a valid mode!" << endl;
                    }
                }
                it++; //increase iterator
            }
        }
        if(!check){   //check controls if we find a connection with another node or not; if we found;
            Trace.push_back(temp);  //push the kth node to Trace.
            //For example first node has connection with 2nd 3rd 4th and 5th nodes, so check will be false; push first node to Trace
        }
    }
}

void Graph::printResult(){
    cout << "Number of visited node: " << nvisited << endl;
    cout << "Max elements kept in memory: " << Max << endl<< endl;
    deque<node>Order;  //deque for Trace back
    node last = Trace.back(); //last is final node in Trace, which will be 16th node
    node first = Trace.front();  //first is the first node in Trace, which will be 1st node if we are searching from the beginning
    while(last.key!=1){  //while last.key is not 1   (last node will show Parents of the nodes, so if we reach the 1st node that means we are done)
        Order.push_front(last);  //push fron the last node
        node parent = Parent[last.key].front();  //find parent of last node
        last = parent;  //make last  = parent
    }
    Order.push_front(first);  //push first node
    int end = 0;
    /*Every node includes information about who is departures, who is arrivals and who is going or coming back
    for example first node is ("Farmer", "Rabbit", "Fox", "Carrot", "Init", "Dest", "Farmer", Rabbit", "Boat")
    that means initially Farmer, Rabbit, Fox, Carrot are departures and there are no arrivals, and Farmer and Rabbit will be on the boat
    */
    while(!Order.empty()){
        node temp = Order.front();  //first node in order
        Order.pop_front();
        int start = 0;
        while(temp.ar.front().name.compare("init")!=0){  //while front object's name is not "init"
            cout << temp.ar.front().name << " ";   //print the names of objects
            temp.ar.pop_front();  //pop object
            start++;   //start counts number of departures,   end counts number of arrivals
        }
        temp.ar.pop_front(); //pop "init"
        cout << " || ";
        while(temp.ar.front().name.compare("dest")!=0){  //while front object's name is not "dest"
            cout << temp.ar.front().name << " ";  //print the names of objects
            temp.ar.pop_front();
        }
        temp.ar.pop_front(); //pop dest
        cout << endl;
        if(start>end && temp.key!=16){  //if departure number is greater that arrival number that means there will be arrivals
            cout << "( ";
               while(temp.ar.front().name.compare("boat")!=0){   //while front object's name is not "boat"
                cout << temp.ar.front().name << " ";  //print the names of objects
                temp.ar.pop_front();
            }
            temp.ar.pop_front(); //pop boat
            cout << " >) " << endl;
        }
        else if(end>start && temp.key!=16){  //if arrival number is greater than departure number that means there will be some one coming back
            cout << "( ";
            while(temp.ar.front().name.compare("back")!=0){  //while front object's name is not "back"
                cout << temp.ar.front().name << " ";  //print the names of objects
                temp.ar.pop_front();
            }
            cout << " < ) " << endl;
            temp.ar.pop_front(); //pop back
        }
        end = start;
    }
}

int main(int argc, char* argv[])
{
    object o1("Farmer");    object o2("Rabbit");   object o3("Carrot");   object o4("Fox");   object o5("init");   object o6("boat");   object o7("dest");  object o8("back");   object o9("empty");
    node n1(1, o1, o2, o3, o4, o5, o7, o1, o2, o6);   node n2(2, o2, o3, o4, o5, o1, o7, o9, o9, o9);    node n3(3, o2, o4, o5, o1, o3, o7, o9, o9, o9);
    node n4(4, o3, o4, o5, o1, o2, o7, o1, o8, o9);   node n5(5, o2, o3, o5, o1, o4, o6, o9, o9, o9);    node n6(6, o1, o4, o3, o5, o2, o7, o1, o4, o6);
    node n7(7, o1, o4, o3, o5, o2, o7, o1, o3, o6);   node n8(8, o3, o5, o1, o2, o4, o7, o1, o2, o8);    node n9(9, o4, o5, o1, o2, o3, o7, o1, o2, o8);
    node n10(10, o1, o4, o5, o2, o3, o7, o9, o9, o9);        node n11(11, o1, o2, o3, o5, o4, o7, o1, o3, o6);   node n12(12, o1, o2, o4, o5, o3, o7, o1, o4, o6);
    node n13(13, o1, o3, o5, o2, o4, o7, o9, o9, o9);       node n14(14, o2, o5, o1, o3, o4, o7, o1, o8, o9);   node n15(15, o1,o2, o5, o3, o4, o7, o1, o2, o6) ;
    node n16(16, o5, o1, o2, o3, o4, o7, o9, o9, o9);
    Graph g;
    g.edge(1, n2);     g.edge(1, n3);   g.edge(1, n4);      g.edge(1, n5);
    g.edge(2, n1);
    g.edge(4, n1);      g.edge(4, n6);      g.edge(4, n7);
    g.edge(6, n4);      g.edge(6, n8);
    g.edge(7, n4);      g.edge(7, n9);
    g.edge(8, n6);      g.edge(8, n10);     g.edge(8, n11);
    g.edge(9, n7);      g.edge(9, n12);      g.edge(9, n13);
    g.edge(11, n8);    g.edge(11, n14);
    g.edge(12, n9);    g.edge(12, n14) ;
    g.edge(14, n15);
    g.edge(15, n14);    g.edge(15, n16);
    clock_t begin = clock();
    g.findpath(n1, argv[1]);
    clock_t end = clock();
    double elapsed_secs = double(end-begin)/CLOCKS_PER_SEC;
    cout << "Algorithm: " << argv[1] << endl;
    cout << "Elapsed time: "<< elapsed_secs<< " secs"<< endl;
    g.printResult();

    return 0;
}
