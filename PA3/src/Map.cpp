#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>

Map::Map() {
    // TODO: Your code here
    // Initialize all distances to a value representing no direct connection
    // Initialize all provinces as unvisited

    distanceMatrix = new int*[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++) {
        distanceMatrix[i] = new int[MAX_SIZE];
    }

    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            distanceMatrix[i][j] = -1; 
        }
    }

    visited = new bool[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++) {
        visited[i] = false;
    }


}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    // TODO: Your code here
    // Read each line in the CSV file
    // Read each cell separated by a comma
    // Convert cell to an integer and store in distanceMatrix

    std::ifstream file(filename);
    std::string line;
    int row = 0;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        int col = 0;

        while (std::getline(ss, cell, ',')) {
            distanceMatrix[row][col] = std::stoi(cell);  
            col++;
        }
        row++;
    }
    file.close();
}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    // TODO: Your code here
    if(provinceB == -1 || provinceA ==-1){
        return false;
    }else {
        int distance = distanceMatrix[provinceA][provinceB];
        return (distance != -1 && distance <= maxDistance);
    }

}

// Marks a province as visited
void Map::markAsVisited(int province) {
    // TODO: Your code here
    visited[province] = true;
    //std::cout << "buraya geldiiiikkkkk " << province << std::endl;


}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    // TODO: Your code here

    return visited[province];
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    // TODO: Your code here

    for (int i = 0; i < MAX_SIZE; i++) {
        visited[i] = false;
    }

}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    // TODO: Your code here
    int count = 0;
    for (int i = 0; i < MAX_SIZE; i++) {
        if (visited[i]) {
            count++;
        }
    }
    return count;

}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    // TODO: Your code here

    return distanceMatrix[provinceA][provinceB];

}


void Map::reMarkAsVisited(int province) {
    // TODO: Your code here
    visited[province] = false;


}




Map::~Map() {
    for (int i = 0; i < MAX_SIZE; i++) {
        delete[] distanceMatrix[i];  
    }
    delete[] distanceMatrix;  

  
    delete[] visited;
}