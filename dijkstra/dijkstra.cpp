#include <iostream>
#include <climits>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <queue>
#include <set>

using namespace std;

typedef struct Node{
    int nodeId;
    int priority;
} node;

typedef struct Solution {
    int initialVertex;
    map<int, int> totalCost;
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
    if(print) {
        cout << "--------Menor Custo--------" << endl;
    }
    *(file) << "--------Menor Custo--------" << endl;
    for(int i = 1; i <= numberOfNodes; i++) {
        if(i != solution.initialVertex && solution.totalCost[i] != INT_MAX) {
            if(print) {
                cout << "O menor custo para ir de " + to_string(solution.initialVertex) + " ate " + to_string(i) + " e " + to_string(solution.totalCost[i]) + "." << endl;
            }
            *(file) << "O menor custo para ir de " + to_string(solution.initialVertex) + " ate " + to_string(i) + " e " + to_string(solution.totalCost[i]) + "." << endl;
        } else if (solution.totalCost[i] == INT_MAX) {
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

vector<int> getEdgeValues(string fileLine) {
    vector<int> edgeValues(3, 0);
    stringstream tempStream;

    tempStream << fileLine;

    for(int j = 0; j < 3; j++) {
        tempStream >> edgeValues[j];
    }

    return edgeValues;
}

vector<vector<int>> createGraphFromFile(string inputFilePath) {
    string fileLine;
    stringstream tempStream;
    ifstream inputFile;
    int numberOfVertices;
    int numberOfEdges;
    vector<int> edgeValues;

    inputFile.open(inputFilePath);

    getline(inputFile, fileLine);
    tempStream << fileLine;
    tempStream >> numberOfVertices;
    tempStream >> numberOfEdges;

    vector<vector<int>> adjMatrix(numberOfVertices + 1, vector<int>(numberOfVertices + 1, 0));

    for(int i = 0; i < numberOfEdges; i++) {
        getline(inputFile, fileLine);
        edgeValues = getEdgeValues(fileLine);
        adjMatrix[edgeValues[0]][edgeValues[1]] = edgeValues[2];
        adjMatrix[edgeValues[1]][edgeValues[0]] = edgeValues[2];
    }

    inputFile.close();

    return adjMatrix;
}

node* createNode(int nodeId, int priority) {
    node* newNode = new node;
    (*(newNode)).nodeId = nodeId;
    (*(newNode)).priority = priority;

    return newNode;
}

vector<node> getNeighbors(vector<vector<int>> graph, int nodeId) {
    vector<node> neighbors;
    int numberOfNodes = graph.size() - 1;
    node* newNode;

    for(int i = 1; i <= numberOfNodes; i++) {
        if(graph[nodeId][i] > 0) {
            newNode = createNode(i, graph[nodeId][i]);
            neighbors.push_back(*(newNode));
        }
    }

    return neighbors;
}

solution dijkstra(vector<vector<int>> graph, int initialVertex) {
    if(initialVertex != -1) {
        solution dijkstraSolution;
        int numberOfNodes = graph.size() - 1;
        map<int, int> totalCost;
        map<int, int> prevNode;
        set<int> visited;
        priority_queue<node> priorityQueue;
        node initialNode;
        node currentNode;
        vector<node> neighbors;
        int newPriority;

        prevNode[initialVertex] = initialVertex;
        initialNode.nodeId = initialVertex;
        initialNode.priority = 0;
        priorityQueue.push(initialNode);

        for(int i = 1; i <= numberOfNodes; i++) {
            if(i != initialVertex) {
                totalCost[i] = INT_MAX;
            } else {
                totalCost[initialVertex] = 0;
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

            delete &currentNode;
        }

    dijkstraSolution.initialVertex = initialVertex;
    dijkstraSolution.totalCost = totalCost;
    dijkstraSolution.prevNode = prevNode;

    return dijkstraSolution;

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
    vector<vector<int>> graph;
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
        graph = createGraphFromFile(inputFilePath);
        dijkstraSolution = dijkstra(graph, initialVertex);
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