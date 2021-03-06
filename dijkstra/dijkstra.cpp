#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <queue>
#include <set>
#include <iomanip>

using namespace std;

typedef struct Node{
    int nodeId;
    double priority;
} node;

typedef struct Solution {
    int initialVertex;
    map<int, double> totalCost;
    map<int, int> prevNode;
} solution;

bool operator<(const node& a, const node& b) {
  return a.priority > b.priority;
}

void printHelp() {
    cout << "E necessario passar um arquivo de entrada para este programa, caso contrario ele nao sera executado." << endl;
    cout << "E necessario passar um vertice inicial para este programa, caso contrario ele nao sera executado." << endl;
    cout << "Caso nao seja passado um arquivo de saida, sera criado um arquivo de saida de nome 'dijkstra-saida.txt'." << endl << endl;
    cout << "Lista de comandos:" << endl << endl;
    cout << "-h : mostra as informacoes sobre a execucao do programa." << endl;
    cout << "   Uma vez passado esse comando na execucao, o programa apenas exibira as informacoes e nao sera executado." << endl << endl;
    cout << "-o <arquivo> : redireciona a saida para o 'arquivo'." << endl;
    cout << "   Exemplo: ./dijkstra -o arquivo-saida.txt" << endl << endl;
    cout << "-f <arquivo> : indica o 'arquivo' que contem o grafo de entrada." << endl;
    cout << "   Exemplo: ./dijkstra -f arquivo-entrada.txt" << endl << endl;
    cout << "-s : mostra a solucao." << endl << endl;
    cout << "-i <vertice> : indica vertice inicial." << endl;
    cout << "   Exemplo: ./dijkstra -i 3" << endl << endl;

    exit(0);
}

void getNodesPath(solution solution, int finalVertex, bool print, ofstream * file) {
    int current = finalVertex;

    while(current != solution.initialVertex) {
        if(print) {
           cout << current << "<-";
        }
        *(file) << current << "<-";
        current = solution.prevNode[current];
    }
    if(print) {
        cout << current << endl;
    }
    *(file) << current << endl;
}

void generateNodesPaths(solution solution, int numberOfNodes, bool print, ofstream * file) {
    if(print) {
        cout << "--------Caminhos--------" << endl;
    }
    *(file) << "--------Caminhos--------" << endl;
    for(int i = 1; i <= numberOfNodes; i++) {
        if(i != solution.initialVertex) {
            getNodesPath(solution, i, print, file);
        }
    }
}

void generateTextSolution(solution solution, int numberOfNodes, bool print, ofstream * file) {
    cout << setprecision(2) << fixed;
    *(file) << setprecision(2) << fixed;

    if(print) {
        cout << "--------Menor Custo--------" << endl;
    }
    *(file) << "--------Menor Custo--------" << endl;
    for(int i = 1; i <= numberOfNodes; i++) {
        if(i != solution.initialVertex && solution.totalCost[i] != HUGE_VAL) {
            if(print) {
                cout << "O menor custo para ir de " + to_string(solution.initialVertex) + " ate " + to_string(i) + " e ";
                cout << solution.totalCost[i];
                cout << "." << endl;
            }
            *(file) << "O menor custo para ir de " + to_string(solution.initialVertex) + " ate " + to_string(i) + " e ";
            *(file) << solution.totalCost[i];
            *(file) << "." << endl;
        } else if (solution.totalCost[i] == HUGE_VAL) {
            if(print) {
                cout << "Nao existe caminho de " + to_string(solution.initialVertex) + " ate " + to_string(i) + "." << endl;
            }
            *(file) << "Nao existe caminho de " + to_string(solution.initialVertex) + " ate " + to_string(i) + "." << endl;
        }
    }
    if(print) {
        cout << endl;
    }
    *(file) << endl;
}

vector<double> getEdgeValues(string fileLine) {
    vector<double> edgeValues(3, 0.0);
    stringstream tempStream;

    tempStream << fileLine;

    for(int j = 0; j < 3; j++) {
        tempStream >> edgeValues[j];
    }

    return edgeValues;
}

void createGraphFromFile(vector<vector<double>> &graph, string inputFilePath) {
    string fileLine;
    stringstream tempStream;
    ifstream inputFile;
    int numberOfVertices;
    int numberOfEdges;
    vector<double> edgeValues;

    inputFile.open(inputFilePath);

    getline(inputFile, fileLine);
    tempStream << fileLine;
    tempStream >> numberOfVertices;
    tempStream >> numberOfEdges;

    graph = vector<vector<double>>(numberOfVertices + 1, vector<double>(numberOfVertices + 1, 0.0));

    for(int i = 0; i < numberOfEdges; i++) {
        getline(inputFile, fileLine);
        edgeValues = getEdgeValues(fileLine);
        graph[edgeValues[0]][edgeValues[1]] = edgeValues[2];
        graph[edgeValues[1]][edgeValues[0]] = edgeValues[2];
    }

    inputFile.close();
}

node createNode(int nodeId, double priority) {
    node* newNode = new node;
    (*(newNode)).nodeId = nodeId;
    (*(newNode)).priority = priority;

    return *newNode;
}

vector<node> getNeighbors(vector<vector<double>> &graph, int nodeId) {
    vector<node> neighbors;
    int numberOfNodes = graph.size() - 1;

    for(int i = 1; i <= numberOfNodes; i++) {
        if(graph[nodeId][i] > 0) {
            neighbors.push_back(createNode(i, graph[nodeId][i]));
        }
    }

    return neighbors;
}

void dijkstra(solution &dijkstraSolution, vector<vector<double>> &graph, int initialVertex) {
    if(initialVertex != -1) {
        int numberOfNodes = graph.size() - 1;
        map<int, double> totalCost;
        map<int, int> prevNode;
        set<int> visited;
        priority_queue<node> priorityQueue;
        node initialNode;
        node currentNode;
        vector<node> neighbors;
        double newPriority;

        prevNode[initialVertex] = initialVertex;
        initialNode.nodeId = initialVertex;
        initialNode.priority = 0;
        priorityQueue.push(initialNode);

        for(int i = 1; i <= numberOfNodes; i++) {
            if(i != initialVertex) {
                totalCost[i] = HUGE_VAL;
            } else {
                totalCost[initialVertex] = 0.0;
            }
        }

        while(!priorityQueue.empty()) {
            currentNode = priorityQueue.top();
            priorityQueue.pop();
            visited.insert(currentNode.nodeId);
            neighbors = getNeighbors(graph, currentNode.nodeId);

            for(node neighbor : neighbors) {
                if(!visited.count(neighbor.nodeId)) {
                    newPriority = currentNode.priority + neighbor.priority;
                    if(newPriority < totalCost[neighbor.nodeId]) {
                        neighbor.priority = newPriority;
                        
                        totalCost.erase(neighbor.nodeId);
                        totalCost[neighbor.nodeId] = newPriority;
                        
                        prevNode.erase(neighbor.nodeId);
                        prevNode[neighbor.nodeId] = currentNode.nodeId;

                        priorityQueue.push(neighbor);
                    }
                }
            }
        }

    dijkstraSolution.initialVertex = initialVertex;
    dijkstraSolution.totalCost = totalCost;
    dijkstraSolution.prevNode = prevNode;

    } else {
        cout << "Erro! E necessario indicar o vertice inicial." << endl << endl;
        printHelp();
    }
}

int main(int argc, char* argv[]) {
    vector<string> args(argv, argv + argc);
    string inputFilePath;
    string outputFilePath = "dijkstra-saida.txt";
    bool printOutput = false;
    int numberOfNodes;
    int initialVertex = -1;
    vector<vector<double>> graph;
    solution dijkstraSolution;
    ofstream outputFile;

    for(int i = 0; i < argc; i++) {
        if(args[i] == "-h") {
            printHelp();
        } else if(args[i] == "-o") {
            if(i < (argc - 1)) {
                outputFilePath = args[i+1];
            }
        } else if(args[i] == "-f") {
            if(i < (argc - 1)) {
                inputFilePath = args[i+1];
            }
        } else if(args[i] == "-s") {
            printOutput = true;
        } else if(args[i] == "-i") {
            if(i < (argc - 1)) {
                initialVertex = stoi(args[i+1]);
            }
        } else {
            continue;
        }
    }

    if(!inputFilePath.empty()) {
        createGraphFromFile(graph, inputFilePath);
        dijkstra(dijkstraSolution, graph, initialVertex);
        numberOfNodes = graph.size() - 1;
        outputFile.open(outputFilePath);

        generateTextSolution(dijkstraSolution, numberOfNodes, printOutput, &outputFile);
        generateNodesPaths(dijkstraSolution, numberOfNodes, printOutput, &outputFile);

        outputFile.close();
    } else {
        cout << "Erro! Nenhum arquivo de entrada foi passado." << endl << endl;
        printHelp();
    }

    return 0;
}