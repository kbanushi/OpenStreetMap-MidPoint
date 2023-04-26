#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <string>
#include <fstream>

#include "graph.h"

using namespace std;

int main(){
    set<int> t;
    vector<int> v;
    graph<int, int> test;
    cout <<test.addVertex(1);
    cout <<test.addVertex(4);
    cout << test.addVertex(6);
    cout << test.addVertex(6);
    cout << endl << endl;
    cout <<test.addVertex(2);
    cout << test.NumVertices();
    test.addEdge(1,4, 10);
    cout << test.NumEdges() << endl;

    t = test.neighbors(1);
    v = test.getVertices();

    for (auto n : t){
        cout << n << " ";
    }
    cout << endl;

    for (auto vert : v){
        cout << vert << " ";
    }

    test.dump(cout);

    return 0;
}