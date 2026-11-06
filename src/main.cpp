#include "summit.hpp"
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <utility>
#include <vector>
#include <queue>
#include <chrono>
#include <thread>
#include <algorithm>
#include <omp.h>

using namespace std;

vector<int> N = {100, 200, 300, 500};
const int INF = numeric_limits<int>::max();
using Matrix = vector<vector<int>>;

// pour générer un nombre aléatoire entre min et max pour générer un graph random
int generateRandomNumber(int min, int max)
{
    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

vector<Summit> generateRandomSummits(int count)
{
    vector<Summit> summits;
    for (int i = 0; i < count; ++i)
    {
        Summit summit;
        summit.setNumber(i + 1);
        summits.push_back(summit);
    }
    return summits;
}

vector<vector<pair<int, int>>> generateRandomGraph(int summitCount)
{
    vector<vector<pair<int, int>>> graph(summitCount);

    for (int source = 0; source < summitCount; ++source)
    {
        for (int destination = 0; destination < summitCount; ++destination)
        {
            if (source != destination && generateRandomNumber(0, 1) == 1)
            {
                int weight = generateRandomNumber(1, 20);
                graph[source].push_back({destination, weight});
            }
        }
    }

    return graph;
}

// conversion du graph en matrix
void graphToMatrix(const vector<vector<pair<int, int>>> &graph, Matrix &matrix)
{
    const int summitCount = static_cast<int>(graph.size());
    matrix.assign(summitCount, vector<int>(summitCount, INF));

    for (int source = 0; source < summitCount; ++source)
    {
        matrix[source][source] = 0;
    }

    for (int source = 0; source < static_cast<int>(graph.size()); ++source)
    {
        for (const auto &edge : graph[source])
        {
            matrix[source][edge.first] = edge.second;
        }
    }
}

// implementation de la methode royfloyd
void royFloydWarshallCore(Matrix &matrix, int summitCount, int middle, int firstRow, int lastRow)
{
    for (int source = firstRow; source < lastRow; ++source)
        {
            for (int destination = 0; destination < summitCount; ++destination)
            {
                if (matrix[source][middle] != INF && matrix[middle][destination] != INF)
                {
                    int newDistance = matrix[source][middle] + matrix[middle][destination];
                    if (newDistance < matrix[source][destination])
                    {
                        matrix[source][destination] = newDistance;
                    }
                }
            }
        }
}

// appel classique
void royFloydWarshall(Matrix &matrix, int summitCount)
{
    for (int middle = 0; middle < summitCount; ++middle)
    {
        royFloydWarshallCore(matrix, summitCount, middle, 0, summitCount);
    }
}

// version thread 
void royFloydWarshallThread(Matrix &matrix, int summitCount, int threadCount)
{
    threadCount = min(threadCount, summitCount);

    for (int middle = 0; middle < summitCount; ++middle) {
        vector<thread> threads;

        for (int threadId = 0; threadId < threadCount; ++threadId) {
            int firstRow = threadId * summitCount / threadCount;
            int lastRow = (threadId + 1) * summitCount / threadCount;

            threads.emplace_back([&, firstRow, lastRow, middle]() {
                royFloydWarshallCore(matrix, summitCount, middle, firstRow, lastRow);
            });
        }

        for (thread& currentThread : threads) {
            currentThread.join();
        }
    }
}

// version openMP
void royFloydWarshallOpenMP(Matrix &matrix, int summitCount, int threadCount)
{
    omp_set_num_threads(threadCount);

    for (int middle = 0; middle < summitCount; ++middle) {
        #pragma omp parallel
        {
            int currentThread = omp_get_thread_num();
            int firstRow = currentThread * summitCount / threadCount;
            int lastRow = (currentThread + 1) * summitCount / threadCount;

            royFloydWarshallCore(matrix, summitCount, middle, firstRow, lastRow);

        }
    }
}

// bfs implémenté pour l'étape 5
vector<int> breadthFirstSearch(const vector<vector<pair<int, int>>> &graph, int source)
{
    const int summitCount = static_cast<int>(graph.size());
    vector<int> distances(summitCount, INF);
    queue<int> pendingSummits;

    distances[source] = 0;
    pendingSummits.push(source);

    while (!pendingSummits.empty())
    {
        int current = pendingSummits.front();
        pendingSummits.pop();

        for (const auto &[destination, weight] : graph[current])
        {
            (void)weight;
            if (distances[destination] == INF)
            {
                distances[destination] = distances[current] + 1;
                pendingSummits.push(destination);
            }
        }
    }

    return distances;
}

void displayGraph(const vector<Summit> &summits, const vector<vector<pair<int, int>>> &graph)
{
    cout << "Graphe oriente pondere :" << endl;
    for (int source = 0; source < static_cast<int>(summits.size()); ++source)
    {
        cout << "Sommet " << summits[source].getNumber() << " -> ";
        for (const auto &edge : graph[source])
        {
            cout << "(" << edge.first + 1 << ", " << edge.second << ") ";
        }
        cout << endl;
    }
}

void displayMatrix(const Matrix &matrix, int summitCount)
{
    for (int source = 0; source < summitCount; ++source)
    {
        for (int destination = 0; destination < summitCount; ++destination)
        {
            if (matrix[source][destination] == INF)
            {
                cout << setw(4) << "-";
            }
            else
            {
                cout << setw(4) << matrix[source][destination];
            }
        }
        cout << endl;
    }
}

int main()
{
    const int threadCount = 4;

    // creation N fois de graph random et calcul de royfloyd et bfs
    for (int summitCount : N)
    {
        // on génère un graph random avec un nombre de sommet aléatoire
        vector<Summit> summits = generateRandomSummits(summitCount);
        vector<vector<pair<int, int>>> graph = generateRandomGraph(summitCount);
        Matrix initialMatrix;
        // on convertit le graph en matrix pour l'algorithme de royfloyd
        graphToMatrix(graph, initialMatrix);

        Matrix matrixSequential = initialMatrix;
        Matrix matrixThread = initialMatrix;
        Matrix matrixOpenMP = initialMatrix;

        // mesure du temps d'exec pour chaque algo
        auto startBfs = chrono::high_resolution_clock::now();
        vector<int> bfsDistances = breadthFirstSearch(graph, 0);
        auto endBfs = chrono::high_resolution_clock::now();

        auto startSequential = chrono::high_resolution_clock::now();
        royFloydWarshall(matrixSequential, summitCount);
        auto endSequential = chrono::high_resolution_clock::now();

        auto startThread = chrono::high_resolution_clock::now();
        std::thread t{royFloydWarshallThread, ref(matrixThread), summitCount, threadCount};
        t.join();
        auto endThread = chrono::high_resolution_clock::now();

        auto startOpenMP = chrono::high_resolution_clock::now();
        royFloydWarshallOpenMP(matrixOpenMP, summitCount, threadCount);
        auto endOpenMP = chrono::high_resolution_clock::now();

        // calcul speedup et tps d'exec
        chrono::duration<double> sequentialTime = endSequential - startSequential;
        chrono::duration<double> threadTime = endThread - startThread;
        chrono::duration<double> openMPTime = endOpenMP - startOpenMP;
        chrono::duration<double> bfsTime = endBfs - startBfs;

        double speedupThread = sequentialTime.count() / threadTime.count();
        double speedupOpenMP = sequentialTime.count() / openMPTime.count();
        int reachableSummits = 0;
        for (int distance : bfsDistances)
        {
            if (distance != INF)
            {
                ++reachableSummits;
            }
        }

        cout << "N = " << summitCount << endl;
        cout << "BFS depuis le sommet 1 :" << endl;
        cout << "  Temps : " << bfsTime.count() << " secondes" << endl;
        cout << "  Sommets atteignables : " << reachableSummits << "/" << summitCount << endl;
        cout << "Temps sequentiel : " << sequentialTime.count() << " secondes" << endl;
        cout << "Temps std::thread : " << threadTime.count() << " secondes" << endl;
        cout << "Temps openMp : " << openMPTime.count() << " secondes" << endl;
        cout << "Speedup std::thread : " << speedupThread << endl;
        cout << "Speedup openMp : " << speedupOpenMP << endl << endl;
    }

    return 0;
}