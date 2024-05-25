/*
File 3: DatasetReader.h
Written By:
1- Amer Oniza (Lines 1-29) ('isNumeric' function)
2- Modar Abdullah (Lines 30-50) ('extract' function)
3- Yazan Al-Jendi (Lines 51-80) ('makeUnique' function)
4- Batoul Khaleel (Lines 81-95) ('tune' function)
5- Edward Assaf (Lines 96-108) ('readFile' function)
*/

#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include "LinkedList.h"

/*
WRITTEN BY AMER ONIZA
JOB:
- returns true if a character represents a number (0-9) and false otherwise
PARAMETERS:
- c: the character
*/
bool isNumeric(const char& c) {
	return (c >= 48 && c <= 57);
}

/*
WRITTEN BY MODAR ABDULLAH
JOB:
- converts a line of some .txt file into an integer and returns that integer
- it works with lines of format 'x y' such as '2039 9283' and '3203 181728'
- converts one segment at a time, must be passed the starting index of the first segment (0)
- automatically stores the starting index of the second segment in the same variable after the first call
PARAMETERS:
- line: the std::string line to be converted
- index: the index referencing the beginning of segments (pass 0)
*/
ll extract(const std::string& line, ll& index) {
	std::string ans = "";
	ll len = line.length();
	while (index < len && isNumeric(line[index])) ans += line[index++];
	if (index != len) {
		while (!isNumeric(line[index])) index++;
	}
	return std::stoll(ans);
}

/*
WRITTEN BY YAZAN ALJENDI
JOB:
- removes duplicate elements in a linked list (specifically LinkedList<ll>)
PARAMETERS:
- list: the linked list
*/
void makeUnique(LinkedList<ll>& list) {
	if (list.get_len() == 0) return;
	std::set<ll> unique;
	LinkedList<ll> clone;
	ListIterator<ll> itr(list.get_clone());
	while (itr.hasNext()) {
		if (unique.find(itr.value()) == unique.end()) {
			unique.insert(itr.value());
			clone.push_front(itr.value());
		}
		itr.next();
	}
	if (unique.find(itr.value()) == unique.end()) {
		unique.insert(itr.value());
		clone.push_front(itr.value());
	}
	itr = clone.get_clone();
	list = LinkedList<ll>();
	while (clone.get_len()) {
		list.push_front(clone.pop_front());
	}
}

/*
WRITTEN BY BATOUL KHALEEL
JOB:
- removes junk edges from graph (duplicates and self-loops)
PARAMETERS:
- adjList: the graph represented by an adjacency list
- vertices: the number of vertices in the graph
*/
void tune(LinkedList<ll>*& adjList, const ll& vertices) {
	for (ll i = 0; i < vertices; i++) {
		adjList[i].removeValue(i);
		makeUnique(adjList[i]);
	}
}

/*
WRITTEN BY EDWARD ASSAF
JOB:
- reads a SNAP .txt dataset file and extracts the graph and the number of vertices from it
- junk edges such as duplicate edges and self-loops will be discarded
- returns true if file is read successfully and false otherwise
- vertices' numbering will differ in the result graph to avoid cases of unordered vertices
PARAMETERS:
- file_path: the path of the .txt dataset
- adjList: a null or uninitialized pointer to 'LinkedList<ll>' which will be initialized with the adjacency list
- size: a null or uninitialized integer that will be storing the number of vertices in the graph
*/
bool readFile(const std::string& file_path, LinkedList<ll>*& adjList, ll& size) {
	std::fstream datasetReader;
	std::map<ll, ll>indexer; //gives vertices an index (useful for unordered vertices)
	ll index = 1; //index used to number vertices (starts from 1 to allow 0 to be the default 'unindexed' state)
	LinkedList<std::pair<ll, ll>>tracker; //tracks the edges of the graph while reading and indexing vertices
	datasetReader.open(file_path, std::ios::in);
	if (datasetReader.is_open()) {
		std::string line;
		for (ll i = 0; i < 4; i++) std::getline(datasetReader, line); //get rid of junk lines
		while (std::getline(datasetReader, line)) {
			ll i = 0;
			ll fromVertex = extract(line, i);
			ll toVertex = extract(line, i);
			if (!indexer[fromVertex]) indexer[fromVertex] = index++;
			if (!indexer[toVertex]) indexer[toVertex] = index++;
			tracker.push_front({ indexer[fromVertex] - 1, indexer[toVertex] - 1 }); //-1 to let numbering start from 0
		}
		datasetReader.close();
	}
	else return false;
	size = index - 1;
	adjList = new LinkedList<ll>[size];
	while (tracker.get_len()) {
		std::pair<ll, ll>edge = tracker.pop_front();
		adjList[edge.first].push_front(edge.second);
	}
	tune(adjList, size);
	return true;
}