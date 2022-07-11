#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

typedef struct Edge {
    int vertex1;
    int vertex2;
    double weight;
} Edge;

typedef struct Graph {
    vector<vector<double>> adjMatrix;
    int numberOfNodes;
} Graph;

void printHelp() {
    cout << "E necessario passar um arquivo de entrada para este programa, caso contrario ele nao sera executado." << endl;
    cout << "Caso nao seja passado um vertice inicial, sera adotado por padrao o vertice 1 como inicial." << endl;
    cout << "Caso nao seja passado um arquivo de saida, sera criado um arquivo de saida de nome 'prim-saida.txt'." << endl << endl;
    cout << "Lista de comandos:" << endl << endl;
    cout << "-h : mostra as informacoes sobre a execucao do programa." << endl;
    cout << "   Uma vez passado esse comando na execucao, o programa apenas exibira as informacoes e nao sera executado." << endl << endl;
    cout << "-o <arquivo> : redireciona a saida para o 'arquivo'." << endl;
    cout << "   Exemplo: ./prim -o arquivo-saida.txt" << endl << endl;
    cout << "-f <arquivo> : indica o 'arquivo' que contem o grafo de entrada." << endl;
    cout << "   Exemplo: ./prim -f arquivo-entrada.txt" << endl << endl;
    cout << "-s : mostra a solucao." << endl << endl;
    cout << "-i <vertice> : indica vertice inicial." << endl;
    cout << "   Exemplo: ./prim -i 3" << endl << endl;

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
    vector<vector<double>> adjMatrix(graph.numberOfNodes + 1, vector<double>(graph.numberOfNodes + 1, HUGE_VAL));
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
        newGraph.adjMatrix[edgeValues[1]][edgeValues[0]] = edgeValues[2];
    }

    inputFile.close();
}

void getMinEdge(Graph &graph, Graph &tree, vector<bool> &treeNodes, bool print, ofstream * file) {
    double minWeight = HUGE_VAL;
    int vertex1 = 0;
    int vertex2 = 0;

    for(int i = 1; i <= graph.numberOfNodes; i++) {
        if(treeNodes[i] == true) {
            for(int j = 1; j <= graph.numberOfNodes; j++) {
                if(!treeNodes[j] && (i != j)) {
                    if(graph.adjMatrix[i][j] != HUGE_VAL && (graph.adjMatrix[i][j] < minWeight)) {
                        minWeight = graph.adjMatrix[i][j];
                        vertex1 = i;
                        vertex2 = j;
                    }
                }
            }
        }
    }

    if((vertex1 != 0) && (vertex2 != 0)) {
        tree.adjMatrix[vertex1][vertex2] = minWeight;
        tree.adjMatrix[vertex2][vertex1] = minWeight;
        treeNodes[vertex2] = true;

        (*(file)) << vertex1 << " " << vertex2 << " " << minWeight << endl;
        if(print) {
            cout << vertex1 << " " << vertex2 << " " << minWeight << endl;
        }
    }
}

void prim(Graph &graph, Graph &tree, int initialVertex, bool print, ofstream * file) {
    vector<bool> treeNodes(graph.numberOfNodes + 1, false);
    Edge auxEdge;

    treeNodes[initialVertex] = true;
    
    tree.numberOfNodes = graph.numberOfNodes;
    initializeGraphAdjMatrix(tree);

    (*(file)) << "MST (V1, V2, W):" << endl;
    if(print) {
        cout << "MST (V1, V2, W):" << endl;
    }

    for(int i = 1; i < graph.numberOfNodes; i++) {
        getMinEdge(graph, tree, treeNodes, print, file);
    }
}

int main(int argc, char* argv[]) {
    vector<string> args(argv, argv + argc);
    string inputFilePath;
    string outputFilePath = "prim-saida.txt";
    bool printOutput = false;
    int initialVertex = 1;
    Graph graph;
    Graph tree;
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
        outputFile.open(outputFilePath);
        createGraphFromFile(graph, inputFilePath);
        prim(graph, tree, initialVertex, printOutput, &outputFile);

    } else {
        cout << "Erro! E necessario passar um arquivo de entrada." << endl << endl;
        printHelp();
    }

    return 0;
}