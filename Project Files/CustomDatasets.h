/*
File 1: CustomDatasets.h
Written By:
1- Amer Oniza (Lines 1-19) (all of it)
*/

#pragma once
#include <iostream>
#include <string>
#include <vector>
typedef long long int ll;

//stores manual data about SNAP datasets
namespace SNAP {
	std::vector<std::string> datasets = {"Email-EuAll.txt", "Cit-HepTh.txt", "Cit-HepPh.txt", "Wiki-Vote.txt", 
    "p2p-Gnutella06.txt", "p2p-Gnutella04.txt"}; //list of SNAP .txt files

	std::string directory = "SNAP Datasets\\"; //when files are in same directory as project, set directory = ""
}