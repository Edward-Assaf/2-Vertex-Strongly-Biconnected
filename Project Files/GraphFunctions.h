/*
File 4: GraphFunctions.h
Written By:
1- Batoul Khaleel (Lines 1-42 & 188-237) ('makeUndirected', 'validSCC' functions)
2- Ali Ghassoun (Lines 43-74) ('dfs' function)
3- Jawa Abd-Al-Hadi (Lines 75-138) ('gabow_dfs', 'gabow' functions)
4- Armenak Jabbour (Lines 139-167) ('SCCReport' function)
5- Joudy Shahhoud (Lines 168-187) ('swap' function)
6- Helal Salloum (Lines 238-253) ('isStronglyConnected' function)
7- Edward Assaf (Lines 254-494) (validSCC,extractLargestSCC,classifyingDFS,connectedClassification,redirect,chainDFS,isBiconnected,removeVertex functions)
*/

#pragma once
#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <set>
#include "LinkedList.h"

/*
WRITTEN BY BATOUL KHALEEL
JOB:
- converts a directed graph to an undirected graph
- junk edges (duplicates and self-loops) will be discarded
PARAMETERS:
- adjList: the graph represented by an adjacency list
- vertices: the number of vertices in the graph
*/
void makeUndirected(LinkedList<ll>*& adjList, const ll& vertices) {
	for (ll i = 0; i < vertices; i++) {
		ListIterator<ll> itr(adjList[i].get_clone());
		if (!itr.reference()) continue;
		while (itr.hasNext()) {
			adjList[itr.value()].push_front(i);
			itr.next();
		}
		adjList[itr.value()].push_front(i);
	}
	tune(adjList, vertices);
}

/*
WRITTEN BY ALI GHASSOUN
JOB:
- returns a linked list containing the vertices discovered while depth-first-searching 'graph' at 'vertex'
- vertices are in descending order of discovery
PARAMETERS:
- graph: the graph represented by an adjacency list
- vertex: the vertex to start the search from
- visited: a boolean array that trackes discovered vertices (visited[i] = true -> vertex i is discovered)
*/
LinkedList<ll> dfs(LinkedList<ll>*& graph, const ll& vertex, bool*& visited) {
	LinkedList<ll> result; //stores the dfs tree nodes
	std::stack<ll> search;
	search.push(vertex);
	visited[vertex] = true;
	while (!search.empty()) {
		ll current = search.top();
		search.pop();
		result.push_front(current);
		LinkedList<ll> outdegrees = graph[current];
		ListIterator<ll> itr(outdegrees.get_clone()); //linked list iterator
		for (ll i = 0; i < outdegrees.get_len(); i++) {
			if (!visited[itr.value()]) {
				search.push(itr.value());
				visited[itr.value()] = true;
			}
			itr.next();
		}
	}
	return result;
}

/*
WRITTEN BY JAWA ABD-AL-HADI :)
JOB:
- runs depth-first-search on a continuous segment of the graph and discovers local strongly connected components
PARAMETERS:
- graph: the graph represented by an adjacency list
- vertex: the vertex to run depth-first-search from
- n: the number of vertices in the graph
- visited: a boolean array to track discovered vertices (visited[i] = true  -> vertex i is discovered)
- inOstack: a boolean array to track vertices in 'ostack' in O(1) time (inOstack[i] = true -> vertex i is in ostack)
- dfs_counter: a counter that will index vertices in ascending order of discovery
- dfs_numbers: an array that will store the index of each vertex provided by the dfs_counter
- ostack: a stack that will store 'open vertices' according to the 'Cheriyan-Mehlhorn-Gabow' algorithm
- rstack: a stack that will store potential 'representative vertices' according to the 'Cheriyan-Mehlhorn-Gabow' algorithm
- components: an array that will store each vertex's component (components[i] = x -> vertex i is a part of component x)
*/
void gabow_dfs(LinkedList<ll>*& graph, const ll& vertex, const ll& n, bool*& visited, bool*& inOstack, 
	ll& dfs_counter, ll*& dfs_numbers, std::stack<ll>& ostack, std::stack<ll>& rstack, ll*& components) {
	visited[vertex] = true;
	dfs_numbers[vertex] = dfs_counter++;
	ostack.push(vertex); rstack.push(vertex);
	inOstack[vertex] = true;
	LinkedList<ll> outdegrees = graph[vertex];
	ListIterator<ll> itr(outdegrees.get_clone());
	for (ll i = 0; i < outdegrees.get_len(); i++) {
		if (!visited[itr.value()])
			gabow_dfs(graph, itr.value(), n, visited, inOstack, dfs_counter, dfs_numbers, ostack, rstack, components);
		else
			if (inOstack[itr.value()]) while (dfs_numbers[itr.value()] < dfs_numbers[rstack.top()]) rstack.pop();
		itr.next();
	}
	if (vertex == rstack.top()) {
		rstack.pop();
		while (!ostack.empty()) {
			ll w = ostack.top(); ostack.pop();
			inOstack[w] = false;
			components[w] = vertex;
			if (w == vertex) break;
		}
	}
}

/*
WRITTEN BY JAWA ABD-AL-HADI :)
JOB:
- discovers strongly connected components in a directed graph using 'Cheriyan-Mehlhorn-Gabow' algorithm
- returns an array where arr[i] = x -> vertex i is a part of component x
PARAMETERS:
- adjList: the graph represented by an adjacency list
- vertices: the number of vertices in the graph
*/
ll* gabow(LinkedList<ll>*& adjList, const ll& vertices) {
	ll dfs_counter = 0;
	bool* visited = new bool[vertices] {}, *inOstack = new bool[vertices] {};
	ll* dfs_numbers = new ll[vertices];
	ll* components = new ll[vertices];
	std::stack<ll> ostack, rstack;
	for (ll i = 0; i < vertices; i++) {
		if (visited[i]) continue;
		gabow_dfs(adjList, i, vertices, visited, inOstack, dfs_counter, dfs_numbers, ostack, rstack, components);
	}
	return components;
}

/*
WRITTEN BY ARMENAK JABBOUR
JOB:
- outputs to console a report about strongly connected components
- useful for 'gabow' function
PARAMETERS:
- components: an array that represents vertices' components (components[i] = x -> vertex i is a part of component x)
- vertices: the number of vertices in the graph
*/
void SCCReport(ll*& components, const ll& vertices) {
	std::map<ll, ll> tracker; //tracks the number of vertices in each component
	for (ll i = 0; i < vertices; i++) {
		tracker[components[i]]++;
	}
	std::cout << "Number of strongly connected components: " << tracker.size() << '\n';
	ll min = vertices + 1, max = 0;
	ll min_i = -1, max_i = -1;
	for (auto& i : tracker) {
		if (i.second < min) {
			min = i.second; min_i = i.first;
		}
		if (i.second > max) {
			max = i.second; max_i = i.first;
		}
	}
	std::cout << "Largest strongly connected component size: " << max << " (Representative vertex: " << max_i << ")\n";
	std::cout << "Smallest strongly connected component size: " << min << " (Representative vertex: " << min_i << ")\n";
}

/*
WRITTEN BY JOUDY SHAHHOUD
JOB:
- swaps every pair of edges (x,y) to (y,x)
PARAMETERS:
- adjList: the graph represented by an adjacency list
- vertices: the number of vertices in the graph
*/
void swap(LinkedList<ll>*& adjList, const ll& vertices) {
	LinkedList<ll>* swapped = new LinkedList<ll>[vertices];
	for (ll i = 0; i < vertices; i++) {
		ListIterator<ll> itr(adjList[i].get_clone());
		for (ll j = 0; j < adjList[i].get_len(); j++) {
			swapped[itr.value()].push_front(i);
			itr.next();
		}
	}
	adjList = swapped;
}

/*
WRITTEN BY BATOUL KHALEEL
JOB:
- checks the correctness of a components array (matching it to strongly connnected components)
- returns true if matching was successful, and false otherwise
PARAMETERS:
- components: an array that represents vertices' components (components[i] = x -> vertex i is a part of component x)
- adjList: the graph represented by an adjacency list
- vertices: the number of vertices in the graph
*/
bool validSCC(ll*& components, LinkedList<ll>*& adjList, const ll& vertices) {
	std::cout << "-> mapping components...\n";
	std::map<ll, ll> tracker; //tracks each component's size
	for (ll i = 0; i < vertices; i++) {
		tracker[components[i]]++;
	}
	ll* indexes = new ll[vertices]{};
	std::cout << "-> encoding vertices component-wise...\n";
	ll* encoder = new ll[vertices]; //gives vertices new indexes
	for (ll i = 0; i < vertices; i++) {
		encoder[i] = indexes[components[i]]++;
	}
	std::cout << "-> checking each component's connectivity...\n";
	bool valid = true;
	for (auto i : tracker) {
		LinkedList<ll>* adj = new LinkedList<ll>[i.second];
		for (ll j = 0; j < vertices; j++) {
			if (components[j] != i.first) continue;
			ll vertex_index = encoder[j];
			ListIterator<ll> itr(adjList[j].get_clone());
			for (ll k = 0; k < adjList[j].get_len(); k++) {
				if (components[itr.value()] == i.first) adj[vertex_index].push_front(encoder[itr.value()]);
				itr.next();
			}
		}
		bool* visited = new bool[i.second] {};
		LinkedList<ll> res1 = dfs(adj, 0, visited);
		swap(adj, i.second);
		visited = new bool[i.second] {};
		LinkedList<ll> res2 = dfs(adj, 0, visited);
		if (res1.get_len() != res2.get_len() || res1.get_len() != i.second) {
			std::cout << "mismatch for component " << i.first << " (DFS1: " << res1.get_len() << ", DFS2: " << res2.get_len() << ", Size: " << i.second << ")\n";
			valid = false;
		}
	}
	if (valid) std::cout << "VERDICT: Successful\n";
	else std::cout << "VERDICT: Failed\n";
	return valid;
}

/*
WRITTEN BY HELAL SALLOUM
JOB:
- checks the strong-connectivity of a graph using the 'Cheriyan-Mehlhorn-Gabow' algorithm (number of components)
- returns true if the graph is strongly connected, and false otherwise
PARAMETERS:
- components: an array that represents vertices' components (components[i] = x -> vertex i is a part of component x)
- vertices: the number of vertices in the graph
*/
bool isStronglyConnected(ll*& components, const ll& vertices) {
	for (ll i = 1; i < vertices; i++) { //strongly connected -> all vertices in one component
		if (components[0] != components[i]) return false;
	}
	return true;
}

/*
WRITTEN BY EDWARD ASSAF
JOB:
- extracts the largest strongly connected component as an individual subgraph
- returns a pair <adjList, vertices> that represents the subgraph and its number of vertices
PARAMETERS:
- adjList: the graph represented by an adjacency list
- vertices: the number of vertices in the graph
- components: an array that represents vertices' components (components[i] = x -> vertex i is a part of component x)
*/
std::pair<LinkedList<ll>*, ll> extractLargestSCC(LinkedList<ll>*& adjList, const ll& vertices, ll*& components) {
	std::map<ll, ll> tracker, encoder;
	for (ll i = 0; i < vertices; i++) {
		tracker[components[i]]++;
	}
	ll max = 0, largest_component = -1;
	for (auto& i : tracker) {
		if (i.second > max) {
			max = i.second;
			largest_component = i.first;
		}
	}
	ll index = 0;
	for (ll i = 0; i < vertices; i++) {
		if (components[i] == largest_component) encoder[i] = index++;
	}
	LinkedList<ll>* scc = new LinkedList<ll>[index];
	for (ll i = 0; i < vertices; i++) {
		if (components[i] != largest_component) continue;
		ll vertex = encoder[i];
		ListIterator<ll> itr(adjList[i].get_clone());
		for (ll j = 0; j < adjList[i].get_len(); j++) {
			if (components[itr.value()] == largest_component) scc[vertex].push_front(encoder[itr.value()]);
			itr.next();
		}
	}
	return std::pair<LinkedList<ll>*, ll> { scc, index };
}

/*
WRITTEN BY EDWARD ASSAF
JOB:
- runs depth-first-search on an undirected graph while classifying encountered edges (Tree Edge or Backward Edge)
PARAMETERS:
- adjList: the graph represented by an adjacency list
- vertex: the vertex to run depth-first-search from
- TE: a dynamically-allocated set that will store all tree edges found in graph
- visited: a boolean array that trackes discovered vertices (visited[i] = true -> vertex i is discovered)
- dfs_counter: a counter that will index vertices in ascending order of discovery
- dfs_numbers: an array that will store the index of each vertex provided by the dfs_counter
- hasBE: an array where hasBE[i] stores the number of backward edges for vertex i
*/
void classifyingDFS(LinkedList<ll>*& adjList, const ll& vertex, std::set<std::pair<ll, ll>>*& TE, 
	 bool*& visited, ll& dfs_counter, ll*& dfs_numbers, ll*& hasBE) {
	visited[vertex] = true;
	dfs_numbers[vertex] = dfs_counter++;
	LinkedList<ll>outdegrees = adjList[vertex];
	ListIterator<ll>itr(outdegrees.get_clone());
	for (ll i = 0; i < outdegrees.get_len(); i++) {
		if (!visited[itr.value()]) {
			TE->insert({ vertex,itr.value() });
			classifyingDFS(adjList, itr.value(), TE, visited, dfs_counter, dfs_numbers, hasBE);
		}
		else if (TE->find({ itr.value(),vertex }) == TE->end() && !hasBE[itr.value()]) { hasBE[vertex]++; }
		itr.next();
	}
}

/*
WRITTEN BY EDWARD ASSAF
JOB:
- classifies edges of a CONNECTED undirected graph because ONLY ONE depth-first-search call will be used
- returns true if the classification is valid (graph is connected) and false otherwise
PARAMETERS:
- adjList: the graph represented by an adjacency list
- vertices: the number of vertices in the graph
- hasBE: an array that will store the number of backward edges for each vertex i in hasBE[i]
- TE: an uninitialized dynamically-allocated set that will store all tree edges found in graph
- dfs_counter: a counter that will index vertices in ascending order of discovery
- dfs_numbers: an array that will store the index of each vertex provided by the dfs_counter
*/
bool connectedClassification(LinkedList<ll>*& adjList, const ll& vertices, ll*& hasBE, 
	 std::set<std::pair<ll, ll>>*& TE, ll& dfs_counter, ll*& dfs_numbers) {
	TE = new std::set<std::pair<ll, ll>>;
	bool* visited = new bool[vertices] {};
	classifyingDFS(adjList, 0, TE, visited, dfs_counter, dfs_numbers, hasBE);
	for (ll i = 0; i < vertices; i++) {
		if (!visited[i]) return false;
	}
	return true;
}

/*
WRITTEN BY EDWARD ASSAF
JOB:
- converts undirected graph to directed graph by directing tree edges to dfs-root and backward edges away from it
PARAMETERS:
- adjList: the graph represented by an adjacency list
- vertices: the number of vertices in the graph
- TE: the set of tree edges in the graph found from some root
- dfs_numbers: an array that stores the index of each vertex in ascending order of discovery
- hasBE: an array that stores the number of backward edges for each vertex i in hasBE[i]
*/
void redirect(LinkedList<ll>*& adjList, const ll& vertices, std::set<std::pair<ll, ll>>*& TE, ll*& dfs_numbers, ll*& hasBE) {
	for (ll i = 0; i < vertices; i++) {
		ListIterator<ll>itr(adjList[i].get_clone());
		ListIterator<ll>prev;
		while(itr.hasNext()) {
			if (TE->find({ i,itr.value() }) != TE->end()) {
				adjList[i].drop(prev);
				if (!prev.reference()) { itr = adjList[i].get_clone(); }
				else { itr = prev; itr.next(); }
			}
			else if (TE->find({ itr.value(), i }) != TE->end()) { prev = itr; itr.next(); }
			else if (dfs_numbers[i] > dfs_numbers[itr.value()]) {
				if (hasBE[i]) { hasBE[i]--; hasBE[itr.value()]++; }
				adjList[i].drop(prev);
				if (!prev.reference()) { itr = adjList[i].get_clone(); }
				else { itr = prev; itr.next(); }
			}
			else {
				if (adjList[itr.value()].find(i) != -1) {
					if (hasBE[itr.value()]) { hasBE[i]++; hasBE[itr.value()]--; }
					auto res = adjList[itr.value()].locate(i);
					if (res.second) adjList[itr.value()].drop(res.first);
				}
				prev = itr; itr.next();
			}
		}
		if (adjList[i].get_len() == 0) continue;
		if (TE->find({ i,itr.value() }) != TE->end()) {
			adjList[i].drop(prev);
			if (!prev.reference()) { itr = adjList[i].get_clone(); }
			else { itr = prev; itr.next(); }
		}
		else if (TE->find({ itr.value(), i }) != TE->end()) { prev = itr; itr.next(); }
		else if (dfs_numbers[i] > dfs_numbers[itr.value()]) {
			if (hasBE[i]) { hasBE[i]--; hasBE[itr.value()]++; }
			adjList[i].drop(prev);
			if (!prev.reference()) { itr = adjList[i].get_clone(); }
			else { itr = prev; itr.next(); }
		}
		else {
			if (adjList[itr.value()].find(i) != -1) {
				if (hasBE[itr.value()]) { hasBE[i]++; hasBE[itr.value()]--; }
				auto res = adjList[itr.value()].locate(i);
				if (res.second) adjList[itr.value()].drop(res.first);
			}
			prev = itr; itr.next();
		}
	}
}

/*
WRITTEN BY EDWARD ASSAF
JOB:
- calculates the chains of the 'Jens Schmidt' algorithm
PARAMETERS:
- adjList: the graph represented by an adjacency list
- vertex: the vertex k where (root,k) is a backward edge
- visited: a boolean array that trackes discovered vertices (visited[i] = true -> vertex i is discovered)
- chain: a vector that will store the path or cycle formed by Jens Schmidt chain
- pause: a boolean value that will halt the depth-first-search when chain ends (pass false)
*/
void chainDFS(LinkedList<ll>*& adjList, const ll& vertex, bool*& visited, std::vector<ll>& chain, bool& pause) {
	if (pause) return;
	if (visited[vertex]) { chain.push_back(vertex); pause = true; return; }
	visited[vertex] = true;
	chain.push_back(vertex);
	ListIterator<ll>itr(adjList[vertex].get_clone());
	for (ll i = 0; i < adjList[vertex].get_len(); i++) {
		if (pause) return;
		if (!visited[itr.value()]) chainDFS(adjList, itr.value(), visited, chain, pause);
		else { chain.push_back(itr.value()); pause = true; return; }
		itr.next();
	}
}

/*
WRITTEN BY EDWARD ASSAF
JOB:
- uses the 'Jens Schmidt' algorithm to determine if an undirected graph is biconnected or not
- returns true if the graph is biconnected, and false otherwise
PARAMETERS:
- adjList: the graph represented by an adjacency list
- vertices: the number of vertices in the graph
*/
bool isBiconnected(LinkedList<ll>* adjList, const ll& vertices) {
	std::set<std::pair<ll, ll>>* TE;
	ll* dfs_numbers = new ll[vertices]{};
	ll dfs_counter = 0;
	ll* hasBE = new ll[vertices]{};
	if (!connectedClassification(adjList, vertices, hasBE, TE, dfs_counter, dfs_numbers)) return false;
	if (vertices <= 2) return true;
	LinkedList<ll>* clone = new LinkedList<ll>[vertices];
	for (ll i = 0; i < vertices; i++) {
		ListIterator<ll> itr(adjList[i].get_clone());
		for (ll j = 0; j < adjList[i].get_len(); j++) {
			clone[i].push_front(itr.value());
			itr.next();
		}
	}
	redirect(clone, vertices, TE, dfs_numbers, hasBE);
	std::map<ll, ll>orderedDFSNumbers;
	for (ll i = 0; i < vertices; i++) {
		if (hasBE[i]) orderedDFSNumbers[dfs_numbers[i]] = i;
	}
	bool* visited = new bool[vertices] {};
	bool first_iteration = true;
	std::set<std::pair<ll, ll>>covered_edges;
	for (auto& i : orderedDFSNumbers) {
		ListIterator<ll>itr(clone[i.second].get_clone());
		visited[i.second] = true;
		for (ll j = 0; j < clone[i.second].get_len(); j++) {
			if (TE->find({ itr.value(), i.second }) != TE->end()) { itr.next(); continue; }
			bool pause = false;
			std::vector<ll> chain; chain.push_back(i.second);
			chainDFS(clone, itr.value(), visited, chain, pause);
			ll len = chain.size();
			if (chain[len - 1] == chain[0] && !first_iteration) return false;
			for (ll k = 0; k < len - 1; k++) covered_edges.insert({ chain[k],chain[k + 1] });
			itr.next();
		}
		first_iteration = false;
	}
	ll total_edges = 0;
	for (ll i = 0; i < vertices; i++) total_edges += clone[i].get_len();
	return (total_edges == covered_edges.size());
}

/*
WRITTEN BY EDWARD ASSAF
JOB:
- deletes a specific vertex from the graph
- all edges linked to deleted vertex will be deleted as well
PARAMETERS:
- adjList: the graph represented by an adjacency list
- vertices: the number of vertices in the graph
- vertex: the vertex to delete
*/
void removeVertex(LinkedList<ll>*& adjList, ll& vertices, const ll& vertex) {
	LinkedList<ll>* newGraph = new LinkedList<ll>[vertices - 1];
	for (ll i = 0; i < vertices; i++) {
		if (i == vertex) { continue; }
		ListIterator<ll>itr(adjList[i].get_clone());
		for (ll j = 0; j < adjList[i].get_len(); j++) {
			if (itr.value() == vertex) { itr.next(); continue; }
			newGraph[i - (i > vertex)].push_front(itr.value() - (itr.value() > vertex));
			itr.next();
		}
	}
	adjList = newGraph; vertices--;
}