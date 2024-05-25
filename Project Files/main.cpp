/*
File 5: main.cpp
Written By:
1- Edward Assaf (Lines 1-36)
2- Jawa Abd-Al-Hadi (Lines 37-49)
3- Batoul Khaleel (Lines 50-58)
4- Modar Abdullah (Lines 59-67)
*/

#include <iostream>
#include <iomanip>
#include <chrono>
#include "LinkedList.h"
#include "DatasetReader.h"
#include "CustomDatasets.h"
#include "GraphFunctions.h"
using namespace std::chrono;

int main() {
	ll n;
	LinkedList<ll>* adjList;
	for (ll i = 0; i < SNAP::datasets.size(); i++) {
		std::cout << "Reading file " << SNAP::datasets[i] << "...\n";
		if (!readFile(SNAP::directory + SNAP::datasets[i], adjList, n)) {
			std::cout << "Failed to read file: Incorrect path.\n";
			continue;
		}
		std::cout << "Testing Strong Connectivity of Directed Graph...\n";
		auto start = high_resolution_clock::now();
		ll* components = gabow(adjList, n);
		if (!isStronglyConnected(components, n)) {
			std::cout << "-> Maximum SCC is being considered...\n";
			auto largestSCC = extractLargestSCC(adjList, n, components);
			adjList = largestSCC.first; n = largestSCC.second;
		}
		auto end = high_resolution_clock::now();
		std::cout << "Testing Two-Connectivity of Underlying Graph...\n";
		auto start2 = high_resolution_clock::now();
		makeUndirected(adjList, n);
		if (!isBiconnected(adjList, n)) {
			auto end2 = high_resolution_clock::now();
			duration<double, std::milli> d1 = end - start;
			duration<double, std::milli> d2 = end2 - start2;
			std::cout << "Result: Graph is NOT 2-vertex strongly biconnected!\n";
			std::cout << "Gabow Time: " << d1.count() << "ms\n";
			std::cout << "Jens Time: " << d2.count() << "ms\n\n";
			continue;
		}
		auto end2 = high_resolution_clock::now();
		std::cout << "Bruteforcing... (2-vertex strong biconnectivity candidate)\n";
		bool success = true;
		for (ll j = 0; j < n; j++) {
			LinkedList<ll>* test = adjList;
			ll vertices = n;
			removeVertex(test, vertices, j);
			if (!isBiconnected(test, vertices)) { success = false; break; }
			if (j % 1000 == 0) std::cout << std::setprecision(2) << (double(j) * 100) / n << "% Complete\n";
		}
		if (success) std::cout << "100% Complete: Graph is 2-vertex strongly biconnected!\n";
		else std::cout << "100% Complete: Graph is NOT 2-vertex strongly biconnected!\n";
		duration<double, std::milli> d1 = end - start;
		duration<double, std::milli> d2 = end2 - start2;
		std::cout << "Gabow Time: " << d1.count() << "ms\n";
		std::cout << "Jens Time: " << d2.count() << "ms\n\n";
	}
	return 0;
}