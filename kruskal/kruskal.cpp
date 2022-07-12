#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>

using namespace std;

typedef vector<pair<double, pair<int, int>>> edgeVector; 

typedef struct Graph {
    vector<vector<double>> adjMatrix;
    int numberOfNodes;
    int numberOfEdges;
} Graph;

int find(vector<int> &fathers, int vertex) {
    if(fathers[vertex] == vertex) {
        return vertex;
    }

    return find(fathers, fathers[vertex]);
}

void unite(vector<int> &fathers, int vertex1, int vertex2) {
    int father1 = find(fathers, vertex1);
    int father2 = find(fathers, vertex2);

    fathers[father1] = father2;

}

void printHelp() {
    cout << "E necessario passar um arquivo de entrada para este programa, caso contrario ele nao sera executado." << endl;
    cout << "Caso nao seja passado um vertice inicial, sera adotado por padrao o vertice 1 como inicial." << endl;
    cout << "Caso nao seja passado um arquivo de saida, sera criado um arquivo de saida de nome 'kruskal-saida.txt'." << endl << endl;
    cout << "Lista de comandos:" << endl << endl;
    cout << "-h : mostra as informacoes sobre a execucao do programa." << endl;
    cout << "   Uma vez passado esse comando na execucao, o programa apenas exibira as informacoes e nao sera executado." << endl << endl;
    cout << "-o <arquivo> : redireciona a saida para o 'arquivo'." << endl;
    cout << "   Exemplo: ./kruskal -o arquivo-saida.txt" << endl << endl;
    cout << "-f <arquivo> : indica o 'arquivo' que contem o grafo de entrada." << endl;
    cout << "   Exemplo: ./kruskal -f arquivo-entrada.txt" << endl << endl;
    cout << "-s : mostra a solucao." << endl << endl;
    cout << "-i <vertice> : indica vertice inicial." << endl;
    cout << "   Exemplo: ./kruskal -i 3" << endl << endl;

    exit(0);
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
    newGraph.numberOfEdges = numberOfEdges;

    initializeGraphAdjMatrix(newGraph);

    for(int i = 0; i < numberOfEdges; i++) {
        getline(inputFile, fileLine);
        edgeValues = getEdgeValues(fileLine);
        newGraph.adjMatrix[edgeValues[0]][edgeValues[1]] = edgeValues[2];
        newGraph.adjMatrix[edgeValues[1]][edgeValues[0]] = edgeValues[2];
    }

    inputFile.close();
}

edgeVector edgesSortedList(Graph &graph) {
    edgeVector edges;
    
    for(int i = 1; i <= graph.numberOfNodes; i++) {
        for(int j = i + 1; j <= graph.numberOfNodes; j++) {
            if(graph.adjMatrix[i][j] != HUGE_VAL) {
                edges.push_back(make_pair(graph.adjMatrix[i][j], make_pair(i, j)));
            }
        }
    }

    sort(edges.begin(), edges.end());

    return edges;
}

void kruskal(Graph &graph, int initialVertex, bool print, ofstream * file) {
    vector<int> fathers;
    edgeVector edges;
    edgeVector treeEdges;
    int mstEdges = 0;
    int currentEdge = 0;
    int currentEdgeVertex1;
    int currentEdgeVertex2;
    double currentEdgeWeight;

    edges = edgesSortedList(graph);

    for(int i = 0; i <= graph.numberOfNodes; i++) {
        fathers.push_back(i);
    }

    while((mstEdges < graph.numberOfNodes - 1) && (currentEdge < graph.numberOfEdges)) {
        currentEdgeVertex1 = edges[currentEdge].second.first;
        currentEdgeVertex2 = edges[currentEdge].second.second;
        currentEdgeWeight = edges[currentEdge].first;

        if(find(fathers, currentEdgeVertex1) != find(fathers, currentEdgeVertex2)) {
            unite(fathers, currentEdgeVertex1, currentEdgeVertex2);

            treeEdges.push_back(make_pair(currentEdgeWeight, make_pair(currentEdgeVertex1, currentEdgeVertex2)));

            mstEdges++;
        }

        currentEdge++;
    }

    sort(treeEdges.begin(), treeEdges.end());

    (*(file)) << "MST (V1, V2, W):" << endl;
    
    if(print) {
        cout << "MST (V1, V2, W):" << endl;     
    }

    for(int i = 0; i < treeEdges.size(); i++) {
        (*(file)) << treeEdges[i].second.first << " " << treeEdges[i].second.second << " " << treeEdges[i].first << endl;

        if(print) {
            cout << treeEdges[i].second.first << " " << treeEdges[i].second.second << " " << treeEdges[i].first << endl;
        }
    }
}

int main(int argc, char* argv[]) {
    vector<string> args(argv, argv + argc);
    string inputFilePath;
    string outputFilePath = "kruskal-saida.txt";
    bool printOutput = false;
    int initialVertex = 1;
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
        kruskal(graph, initialVertex, printOutput, &outputFile);
        outputFile.close();
    } else {
        cout << "Erro! E necessario passar um arquivo de entrada." << endl << endl;
        printHelp();
    }

    return 0;
}