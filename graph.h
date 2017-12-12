#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <stack>

using namespace std;

class Graph {
	private:
		unordered_map<int, vector<int>> list_of_friends;
		unordered_map<int, int> besties;
		unordered_map<int, int> friend_rec;
		int num_edges = 0;
		void addEdge(int source, int target, const bool reverse);		// v-source & v-target --- if source and target do not exist in hashtable, create them
	public:
		Graph():num_edges(0) {}; 
		Graph(bool insertReverseEdge, ifstream& ifs); // Reads the graph from a text file of edges one per line.
		~Graph();
		int getNumVertices() { return list_of_friends.size();}
		int getNumEdges() { return num_edges;}
		
		void sortFriends();	//iterates through all nodes and sorts friends list
		void findBesties();	//iterates through all nodes to find best friends (based on common friends)
		void findFriend();	//iterates through all nodes to recommend a  friend that should be added (based on common friends)
		int betterFriendRec(int person);	//if the rec is already in friends list, do something else
		
		int findPopular(int person);
		
		void printFriends();		//prints all friends
		void printFriendInfo();		//prints person, bestie, and friend rec
};

#endif
