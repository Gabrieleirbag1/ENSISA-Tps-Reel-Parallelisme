#include "summit.hpp"
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <utility>
#include <vector>

using namespace std;

const int MAX_SUMMITS = 11;
const int INF = numeric_limits<int>::max();

int generateRandomNumber(int min, int max) {
    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

vector<Summit> generateRandomSummits(int count) {
    vector<Summit> summits;
    for (int i = 0; i < count; ++i) {
        Summit summit;
        summit.setNumber(i + 1);
        summits.push_back(summit);
    }
    return summits;
}

vector<vector<pair<int, int>>> generateRandomGraph(int summitCount) {
    vector<vector<pair<int, int>>> graph(summitCount);

    for (int source = 0; source < summitCount; ++source) {
        for (int destination = 0; destination < summitCount; ++destination) {
            if (source != destination && generateRandomNumber(0, 1) == 1) {
                int weight = generateRandomNumber(1, 20);
                graph[source].push_back({destination, weight});
            }
        }
    }

    return graph;
}

void graphToMatrix(const vector<vector<pair<int, int>>>& graph,
                   int matrix[MAX_SUMMITS][MAX_SUMMITS]) {
    for (int source = 0; source < MAX_SUMMITS; ++source) {
        for (int destination = 0; destination < MAX_SUMMITS; ++destination) {
            matrix[source][destination] = source == destination ? 0 : INF;
        }
    }

    for (int source = 0; source < static_cast<int>(graph.size()); ++source) {
        for (const auto& edge : graph[source]) {
            matrix[source][edge.first] = edge.second;
        }
    }
}

void royFloydWarshall(int matrix[MAX_SUMMITS][MAX_SUMMITS], int summitCount) {
    for (int middle = 0; middle < summitCount; ++middle) {
        for (int source = 0; source < summitCount; ++source) {
            for (int destination = 0; destination < summitCount; ++destination) {
                if (matrix[source][middle] != INF && matrix[middle][destination] != INF) {
                    int newDistance = matrix[source][middle] + matrix[middle][destination];
                    if (newDistance < matrix[source][destination]) {
                        matrix[source][destination] = newDistance;
                    }
                }
            }
        }
    }
}

void displayGraph(const vector<Summit>& summits,
                  const vector<vector<pair<int, int>>>& graph) {
    cout << "Graphe oriente pondere :" << endl;
    for (int source = 0; source < static_cast<int>(summits.size()); ++source) {
        cout << "Sommet " << summits[source].getNumber() << " -> ";
        for (const auto& edge : graph[source]) {
            cout << "(" << edge.first + 1 << ", " << edge.second << ") ";
        }
        cout << endl;
    }
}

void displayMatrix(int matrix[MAX_SUMMITS][MAX_SUMMITS], int summitCount) {
    for (int source = 0; source < summitCount; ++source) {
        for (int destination = 0; destination < summitCount; ++destination) {
            if (matrix[source][destination] == INF) {
                cout << setw(4) << "-";
            } else {
                cout << setw(4) << matrix[source][destination];
            }
        }
        cout << endl;
    }
}


int main() {
    int summitCount = generateRandomNumber(5, MAX_SUMMITS);
    vector<Summit> summits = generateRandomSummits(summitCount);
    vector<vector<pair<int, int>>> graph = generateRandomGraph(summitCount);
    static int matrix[MAX_SUMMITS][MAX_SUMMITS];

    graphToMatrix(graph, matrix);
    displayGraph(summits, graph);

    cout << "\nMatrice d'adjacence :" << endl;
    displayMatrix(matrix, summitCount);

    royFloydWarshall(matrix, summitCount);

    cout << "\nDistances minimales :" << endl;
    displayMatrix(matrix, summitCount);

    return 0;
}