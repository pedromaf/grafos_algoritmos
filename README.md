# Teoria dos Grafos
Implementação em C++ dos algoritmos: 
 * Dijkstra
 * Prim
 * Bellman-Ford
 * Kruskal

Observações:
  * Todas as implementações descritas consideram grafos com vértices identificados por números maiores que 0.
  * Todas as implementações descritas consideram arquivos de entrada no seguinte formato:
      
      |V| |E|
      
      seguido de |E| linhas na forma:
      
      Vn Vm W(VnVm)
      
      Sendo: 
      * |V| o número de vértices do grafo;
      * |E| o número de arestas do grafo;
      * Vn um vértice de V;
      * VnVm uma aresta de E formada pelos vértices Vn e Vm;
      * W(VnVm) o peso da aresta VnVm.
  
  * As saídas dos algoritmos não seguem um padrão específico.
