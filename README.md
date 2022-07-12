# Teoria dos Grafos
Implementação em C++ dos algoritmos: 
 * Dijkstra
 * Prim
 * Floyd-Warshall
 * Kruskal

## Execução
 * Para ver instruções sobre a execução de cada algoritmo execute-os utilizando a flag **-h** no terminal.

## Observações
  * Todas as implementações descritas consideram grafos com vértices identificados por números inteiros maiores que 0.
  * Todas as implementações descritas consideram arquivos de entrada no seguinte formato:
      ```
      |V| |E|
      ```
      seguido de |E| linhas na forma:
      ```
      Vn Vm W(VnVm)
      ```
      Sendo: 
      * |V| o número de vértices do grafo;
      * |E| o número de arestas do grafo;
      * Vn um vértice de V;
      * VnVm uma aresta de E formada pelos vértices Vn e Vm;
      * W(VnVm) o peso da aresta VnVm (para grafos sem pesos, esse elemento pode ser omitido no arquivo de entrada).
  
  * As saídas dos algoritmos não seguem um padrão específico.
