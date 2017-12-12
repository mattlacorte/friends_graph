
#include "graph.h"
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

int main()
{
	clock_t t;
	t = clock();
	
	ifstream ifs("facebook_combined.txt");
    Graph g(true, ifs);
    cout << "Number of people: " << g.getNumVertices() << endl;
    cout << "Number of connections: " << g.getNumEdges() << endl << endl;
	
	g.findBesties();
	g.findFriend();
	g.printFriendInfo();
	
	t = clock() - t;
	
	cout << "Time to run: " << ((float)t)/CLOCKS_PER_SEC << "s" << endl;
	
    return 0;
}
