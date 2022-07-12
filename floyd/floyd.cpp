#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

typedef vector<vector<double>> matrix;

typedef struct Graph {
    matrix adjMatrix;
    int numberOfNodes;
} Graph;

void printHelp() {
    cout << "E necessario passar um arquivo de entrada para este programa, caso contrario ele nao sera executado." << endl;
    cout << "Caso nao seja passado um arquivo de saida, sera criado um arquivo de saida de nome 'floyd-saida.txt'." << endl << endl;
    cout << "Lista de comandos:" << endl << endl;
    cout << "-h : mostra as informacoes sobre a execucao do programa." << endl;
    cout << "   Uma vez passado esse comando na execucao, o programa apenas exibira as informacoes e nao sera executado." << endl << endl;
    cout << "-o <arquivo> : redireciona a saida para o 'arquivo'." << endl;
    cout << "   Exemplo: ./floyd -o arquivo-saida.txt" << endl << endl;
    cout << "-f <arquivo> : indica o 'arquivo' que contem o grafo de entrada." << endl;
    cout << "   Exemplo: ./floyd -f arquivo-entrada.txt" << endl << endl;
    cout << "-s : mostra a solucao (em ordem crescente)." << endl << endl;

    exit(0);
}

vector<double> getEdgeValues(string fileLine) {
    vector<double> edgeValues(3, 0);
    stringstream tempStream;

    tempStream << fileLine;

    for(int j = 0; j < 3; j++) {
        tempStream >> edgeValues[j];
    }

    return edgeValues;
}

void initializeGraphAdjMatrix(Graph &graph) {
    matrix adjMatrix(graph.numberOfNodes + 1, vector<double>(graph.numberOfNodes + 1, HUGE_VAL));

    graph.adjMatrix = adjMatrix;
}

void createGraphFromFile(Graph &newGraph, string inputFilePath) {
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

    newGraph.numberOfNodes = numberOfVertices;

    initializeGraphAdjMatrix(newGraph);

    for(int i = 0; i < numberOfEdges; i++) {
        getline(inputFile, fileLine);
        edgeValues = getEdgeValues(fileLine);
        newGraph.adjMatrix[edgeValues[0]][edgeValues[1]] = edgeValues[2];
    }

    inputFile.close();
}

void initializeDistMatrix(Graph &graph, matrix &distMatrix) {
    matrix newDistMatrix(graph.numberOfNodes + 1, vector<double>(graph.numberOfNodes + 1, HUGE_VAL));
    distMatrix = newDistMatrix;

    for(int i = 1; i <= graph.numberOfNodes; i++) {
        for(int j = 1; j <= graph.numberOfNodes; j++) {
            distMatrix[i][j] = graph.adjMatrix[i][j];
        }
    }
}

void initializePredecessorMatrix(Graph &graph, vector<vector<int>> &predecessors) {
    vector<vector<int>> newPredecessors(graph.numberOfNodes + 1, vector<int>(graph.numberOfNodes + 1, 0));
    predecessors = newPredecessors;

    for(int i = 0; i <= graph.numberOfNodes; i++) {
        predecessors[i][i] = 0;
    }
}

void printResult(int numberOfNodes, matrix &distMatrix, vector<vector<int>> &predecesors, bool print, ofstream * file) {

    for(int i = 1; i <= numberOfNodes; i++) {
        for(int j = 1; j <= numberOfNodes; j++) {
            if(distMatrix[i][j] != HUGE_VAL) {
                if(i != j) {
                    (*(file)) << "A menor distancia de " << i << " para " << j << " e " << distMatrix[i][j] << "." << endl;
                    if(print) {
                        cout << "A menor distancia de " << i << " para " << j << " e " << distMatrix[i][j] << "." << endl;
                    }
                }
            } else {
                if(i != j) {
                    (*(file)) << "Nao existe caminho de " << i << " para " << j << "." << endl;
                    if(print) {
                        cout << "Nao existe caminho de " << i << " para " << j << "." << endl;
                    }
                }
            }
            
        }
    }
}

void floyd(Graph &graph, bool print, ofstream * file) {
    matrix distMatrix;
    vector<vector<int>> predecessors;

    initializeDistMatrix(graph, distMatrix);
    initializePredecessorMatrix(graph, predecessors);

    for(int i = 1; i <= graph.numberOfNodes; i++) {
        for(int j = 1; j <= graph.numberOfNodes; j++){
            if(distMatrix[i][j] < HUGE_VAL) {
                predecessors[i][j] = i;
            }
        }
    }

    for(int k = 1; k <= graph.numberOfNodes; k++) {
        for(int i = 1; i <= graph.numberOfNodes; i++) {
            for(int j = 1; j <= graph.numberOfNodes; j++) {
                if(distMatrix[i][k] + distMatrix[k][j] < distMatrix[i][j]) {
                    distMatrix[i][j] = distMatrix[i][k] + distMatrix[k][j];
                    predecessors[i][j] = k;
                }
            }
        }
    }
    
    printResult(graph.numberOfNodes, distMatrix, predecessors, print, file);
}

int main(int argc, char* argv[]) {
    vector<string> args(argv, argv + argc);
    string inputFilePath;
    string outputFilePath = "floyd-saida.txt";
    bool printOutput = false;
    Graph graph;
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
        } else {
            continue;
        }
    }

    if(!inputFilePath.empty()) {
        createGraphFromFile(graph, inputFilePath);

        outputFile.open(outputFilePath);
        floyd(graph, printOutput, &outputFile);
        outputFile.close();
    } else {
        cout << "Erro! E necessario passar um arquivo de entrada." << endl << endl;
        printHelp();
    }

    return 0;
}