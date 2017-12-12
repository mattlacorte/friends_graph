#include "graph.h"
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

//unordered_map<int, vector<int>> list_of_friends;
//unordered_map<int, int> besties;
//unordered_map<int, int> friend_rec;
//int num_edges;


//v-source & v-target --- if source and target do not exist in hashtable, create them
void Graph::addEdge(int source, int target, const bool reverse){

// check if person exists
	vector<int> newVec;
	pair <int, vector<int>> newPair (source, newVec);
			
	vector<int> newerVec;
	pair <int, vector<int>> newerPair (target, newerVec);
			
	bool isVS = false;
	bool isVT = false;
	bool isThere = false;		//if source(vs) or target(vt) exists, or if edge already exists
			
			
	if (list_of_friends.find(newPair.first) != list_of_friends.end()) { isVS = true; }
	if (list_of_friends.find(newerPair.first) != list_of_friends.end()) { isVT = true; }
			
	// if source vertex does not exist, add it
	if (!(isVS)) {
		newPair.second.push_back(target);
		list_of_friends.insert(newPair);
		num_edges++;
	}
	// if target vertex does not exist, add it
	if (!(isVT)) {
		if (reverse) {
			newerPair.second.push_back(source);
			num_edges++;
		}
		list_of_friends.insert(newerPair);
	}
	
	//if target is not in source, add it
	if (isVS) {
		auto findS = list_of_friends.find(source);

		for (auto it : findS->second) {
			if (it == target) {	isThere = true; }
		}
		
		if (!(isThere)) {
			findS->second.push_back(target);
			num_edges++;
		}
	}
	
	//if target exists, and reverse true, add source to target
	if (isVT && reverse) {
		isThere = false;
		auto findS = list_of_friends.find(source);
		auto findT = list_of_friends.find(target);
		
		for (auto it : findT->second) {
			if (it == target) {	isThere = true; }
		}
		
		if (!(isThere)) {
			findT->second.push_back(source);
			num_edges++;
		}
	}
}

// Reads the graph from a text file of edges one per line
Graph::Graph(bool insertReverseEdge, ifstream& ifs){
	
	string line, vs, vt, breaker = " ";
	int source, target;
	num_edges = 0;
	
	if (ifs.is_open()) {
		//	ADD ALL VERTICES
		while (getline(ifs, line)) {
			vs = line.substr(0, line.find(breaker));
			source = stoi(vs);
			vt = line.substr(line.find(breaker)+1, line.size());
			target = stoi(vt);
		
			addEdge(source, target, insertReverseEdge);
		}	
	}
	else {
		cout << "ERROR opening file" << endl;
	}
	
	return;
}
Graph::~Graph() {
	
	return;
}

//order list_of_friends vectors
void Graph::sortFriends() {
	for (auto it : list_of_friends) {
		sort(it.second.begin(), it.second.end());
	}
	
	return;
}
void Graph::findBesties() {
		
	//cout << "size of friends list : " << list_of_friends.size() << endl;
	
	//iterate through all people
	for (auto person : list_of_friends) {
		
			int tempBestie;				//change to bestie if they have more mutuals than the previous
			int friendCount = 0;		//stores num of mutuals of bestie
			int tempFriendCount = 0;	//temp storage for tempBestie
			int theTempFriend;			//friend to cpmpare friends against (person.vector_of_friends
			
		for (auto friends : person.second) {
			
			theTempFriend = friends;
			
			//assign pointers to start and end of friend lists
			auto tempFriend = list_of_friends.find(friends)->second.begin();
			auto endTempFriend = list_of_friends.find(friends)->second.end();
			auto friendsList = person.second.begin();
			auto endFriends = person.second.end();
			
			tempFriendCount = 0;
			
			do {
				//if they have a mutual friend
				if (*tempFriend == *friendsList) {
					tempFriendCount++;
					tempFriend++;
					friendsList++;
				}
				//if one num is higher than the other, increase other
				else if (*tempFriend > *friendsList) {
					friendsList++;
				}
				//same as before
				else if (*tempFriend < *friendsList) {
					tempFriend++;
				}
				else {
					cout << "shit..." << endl;
				}
			//do while not at the end of both friend lists
			} while ((tempFriend != endTempFriend) && (friendsList != endFriends));
			
			//if the new friend has more mutuals than the previous, switch em
			if (tempFriendCount >= friendCount) {
			friendCount = tempFriendCount;
			tempBestie = theTempFriend;
			}
		}
		
		//assign the new pair of besties to the list
		pair<int, int> newFriends (person.first, tempBestie);
		besties.insert(newFriends);
	}
	
	//cout << "size of new friends : " << besties.size() << endl;
	
	return;			
}

void Graph::findFriend() {
	bool badRec = false;		//if they are already friends --> true
	
	//go through all groups of besties
	for (auto person : besties) {
		int best = person.second;	//bestie
		int toFriend = besties.find(best)->second;	//bestie's bestie
		badRec = false;
		
		
		
		pair<int, int> newPair (person.first, toFriend);
		
		//if they are each other's best friends
		if (person.first == besties.find(person.second)->second) {
			toFriend = findPopular(person.first);
		}
		else if (badRec) {
			toFriend = betterFriendRec(person.first);
		}
		
		newPair.second = toFriend;
		friend_rec.insert(newPair);
	}
	
	return;
}

int Graph::betterFriendRec(int person) {
	
	int friendToReturn = 0;
	bool badRec = false;		//if already friends --> true
	

	auto best = besties.find(person)->second;
	auto toFriend = besties.find(best)->second;	//bestie's bestie
		
	for (auto it : list_of_friends.find(person)->second) {
		if (toFriend == it) {
			badRec = true;
			break;
		}
	}
	
	if (badRec) {
		friendToReturn = betterFriendRec(list_of_friends.find(toFriend)->first);
	}
	
		
	return friendToReturn;
}

int Graph::findPopular(int person) {
	
	int toRec;					//friend to return
	int max = 0;				//num of popular's friends
	bool alreadyThere = false;	//if already friends --> true
	
	vector<int> person_friends = list_of_friends.find(person)->second;	//person's list of friends
	
	//go through all people
	for (auto pop : list_of_friends) {
		//check if already friends
		for (auto fri : person_friends) {
			if (pop.first == fri) {
					alreadyThere = true;
			}
		}
		//check for num of friends
		if ((pop.second.size() > max) && !alreadyThere) {
			toRec = pop.first;
		}
	}
	
	return toRec;
}

void Graph::printFriends() {
	
	for (int i = 0; i < list_of_friends.size(); i++) {
		cout << i << " : ";
		for (auto itt : list_of_friends.find(i)->second) {
			cout << itt << " ";
		}
		cout << endl;
	}
	
	return;
}

void Graph::printFriendInfo() {
	int top = list_of_friends.size();
	
	cout << "Person    ||    Bestie    ||    Recommended" << endl;
	for (int i = 0; i < top; i++) {
		cout << i << "\t\t\t\t" << besties.find(i)->second << "\t\t\t\t" << friend_rec.find(i)->second << endl;
	}
	
	
	return;
}
