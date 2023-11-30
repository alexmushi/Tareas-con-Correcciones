/*
*  autor: Benjamin Valdes Aguirre
*  fecha: Diciembre 2020
*  programa: act 4.1 Implementacion Grafos
*  desc: estos programas son solciones parciales a algunos de
*  las actividades planteadas en el curso TC1031
*
*		Nota: Esta imlpementación es Top Down, no divide al árbol para la operacións
*		de barrado
*/

#ifndef Graph_H_
#define Graph_H_

#include <string>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <list>
#include <algorithm>

using namespace std;


class Graph {
private:
		// Tamaño de lista
		int edgesList;
		// Tamaño de matriz
		int edgesMat;
		// Numero de nodos
		int nodes;
    	vector<int> *adjList;
		int *adjMatrix;

public:
		// Constructor de Grapho
		Graph(int);
		Graph();

		// Prints para las conexiones de la lista o de la matriz
		string printAdjList();
		string printAdjMat();
		string printAdjMat_clean();

		string DFS(int, int);
		string depthHelper(int, int, stack<int>&, list<int>&, vector<vector<int> >&);
		string BFS(int, int);
		string breadthHelper(int, int, queue<int>&, list<int>&, vector<vector<int> >&);

		string print_visited(list<int>);
		string print_path(vector<vector<int> >&,int ,int);
		bool contains(list<int>, int);

		// Funciones que cargan las conexiones a la lista o matriz
		void sortAdjList();
		void addEdgeAdjList(int, int);
		void addEdgeAdjMatrix(int, int);
		void loadGraphList(string, int);
		void loadGraphMat(string, int, int);
};

Graph::Graph() {
	edgesList = edgesMat = 0;
}

// Constructor con tamaño de nodos
Graph::Graph(int n) {
	nodes = n;
	adjList = new vector<int>[nodes];
	adjMatrix = new int [nodes*nodes];
	for (int i = 0; i < nodes*nodes; i++)
		adjMatrix[i] = 0;
	edgesList = edgesMat = 0;
}

void Graph::loadGraphList(string data, int a){
	adjList = new vector<int>[a];
	nodes = a;
	int u, v, i = 0;
	while ( i < data.size()) {
			u = (int)data[i+1] - 48;
			v = (int)data[i+3] - 48;
			i = i + 6;
			addEdgeAdjList(u,v);
//			cout << "u  "  << u << " v " << v << endl;
	}
	sortAdjList();
}


void Graph::loadGraphMat(string data, int a, int b){
	adjMatrix = new int [a*b];
	nodes = a;
	for (int i = 0; i < a*b; i++)
		adjMatrix[i] = 0;
	int u, v, i = 0;
	while (i < data.size()) {
		u = (int)data[i+1] - 48;
		v = (int)data[i+3] - 48;
		i = i + 6;
		addEdgeAdjMatrix(u,v);
	}
}
// Agrega cada edge individualmente a la lista
void Graph::addEdgeAdjList(int u, int v){
	/* u que es el renglon, la agregas en ese indice a v, por lo que tienes
	 * una lista por cada elemento del grafo, que dicta las conexiones que tiene
	*/
	// Aquí a u (elemento u) le agregas v (la conexión)
	adjList[u].push_back(v);
	// Haces lo mismo para que sea simetrico
	adjList[v].push_back(u);
	// Contador de lista (conexiones)
	edgesList++;
}

// Agrega cada edge individualmente a la matriz
void Graph::addEdgeAdjMatrix(int u, int v){
	// Aquí le pones un uno para demostrar que existe la conexión en ambos sentidos
	// La formula para asegurar es (indice = renglon * tam + columna)
	adjMatrix[u * nodes + v] = 1;
	// Como es conexión de ambos lados, lo haces del otro lado también
	adjMatrix[v * nodes + u] = 1;
	// Tamaño de la matriz
	edgesMat++;
}

// Imprima las conexiones de cada lista
string Graph::printAdjList(){
	  stringstream aux;
		for (int i = 0; i < nodes; i++){
	        aux << "vertex "
	             << i << " :";
	        for (int j = 0; j < adjList[i].size(); j ++){
							 aux << " " << adjList[i][j];
					}
	        aux << " ";
    }
		return aux.str();

}

// Sort del list
void Graph::sortAdjList(){
	// Se le hace la función del sort
	for (int i = 0; i < nodes; i++)
		sort(adjList[i].begin(), adjList[i].end());
}

// Imprime lo que tiene la matriz
string Graph::printAdjMat(){
	stringstream aux;
	for (int i = 0; i < nodes; i++){
	   for (int j = 0; j < nodes; j++){
			 aux << adjMatrix[i*nodes+j] << " ";
		 }
  }
	return aux.str();
}

// Imprime la matriz de conexiones de forma limpia
string Graph::printAdjMat_clean(){
	stringstream aux;
	aux << "\n nodes \t|";
	for (int i = 0; i < nodes; i++){
			aux << "\t" << i ;
	}
	aux << "\n";
	for (int i = 0; i < nodes; i++){
			aux << "__________";
	}
	aux << "\n";
	for (int i = 0; i < nodes; i++){
		 aux << i << "\t|";
	   for (int j = 0; j < nodes; j++){
			 aux << "\t" << adjMatrix[i*nodes+j];
		 }
	   aux << "\n";
  }
	return aux.str();
}

string Graph::DFS(int start, int goal){
	// Creas el stack para el DFS
	stack <int> st;
	// Creas la lista de los nodos que fueron visitados
	list <int> visited;
	/* Creas la matriz de paths donde tiene una fila por nodo, y una columna con un -1
	 * Así paths es una matriz donde se guardan el camino que toma
	 */
	vector <vector<int> > paths(nodes, vector <int> (1, -1));
	// Agregas al stack el nodo con el cual comenzar
	st.push(start);
	// Llamar la función que hace la recursión y guardar lo que regresa en un string
	string ans = depthHelper(start, goal, st, visited, paths);
	// Imprimir el path que tomó
	ans = ans + print_path(paths, start, goal);
	// Regresar ese string
	return ans;
}

string Graph::depthHelper(int current, int goal, stack<int> &st, list<int> &visited, vector<vector<int> > &paths){
	// Si encontraste el nodo que se buscaba
	if (current == goal){
		return print_visited(visited);
	}
	// Si el stack ya no tiene nada se recorrió todo el grafo
	else if (st.empty()){
		return " node not found";
	}
	// Si todavía quedan elementos
	else {
		// Sacar el nodo actual del stack con top
		current = st.top();
		// Borrarlo del stack al elemento
		st.pop();
		// Agregar a la lista de visitado que ya visitaste el nodo
		visited.push_back(current);
		// Recorre la lista de conexiones de current
		for (int i = 0; i < adjList[current].size(); i++){
			// Si el nodo en la lista de conexiones no ha sido visitado
			if (!contains(visited, adjList[current][i])){
				// Agregas ese nodo al stack para que sea el siguiente visitado (en la siguiente iteración)
				st.push(adjList[current][i]);
				/* Aquí es como tener una lista con el nodo, y quien es su padre. De esta forma
				 * tienes al nodo, y el padre, y así se puede llegar del start al goal
				 */
				paths[adjList[current][i]][0] = current;
			}
		}
		// Llamas a la recursión con el nuevo current
		return depthHelper(current, goal, st, visited, paths);
	}
}

string Graph::BFS(int start, int goal){
	// Creas el queue para el BFS
	queue <int> qu;
	// Creas la lista de los nodos que fueron visitados
	list <int> visited;
	// Crear la matriz de los paths para guardar el path que tomo con el número de nodos como fila
	vector <vector<int> > paths(nodes, vector <int> (1, -1));
	// Agregas al queue el nodo con el cual comenzar
	qu.push(start);
	// Llamar la función que hace la recursión y guardar lo que regresa en un string
	string ans = breadthHelper(start, goal, qu, visited, paths);

	// for (int i = 0; i < paths.size(); i++){
	// 	cout << endl << "Nodos en Posición: " << i << " = " << paths[i][0];
	// }
	//cout << endl;

	// Imprimir el path que tomó
	ans = ans + print_path(paths, start, goal);
	// Regresar ese string
	return ans;
}

string Graph::breadthHelper(int current, int goal, queue<int> &qu, list<int> &visited,  vector<vector<int> > &paths){
	// Si encontraste el nodo que se buscaba
	if (current == goal){
		return print_visited(visited);
	}
	// Si el stack ya no tiene nada se recorrió todo el grafo
	else if (qu.empty()){
		return " node not found";
	}
	// Si todavía quedan elementos
	else {
		// Sacar el nodo actual del queue con front
		current = qu.front();
		// Borrarlo del queue al elemento
		qu.pop();
		// Agregar a la lista de visitado que ya visitaste el nodo
		visited.push_back(current);
		// Recorre la lista de conexiones de current
		for (int i = 0; i < adjList[current].size(); i++){
			// Si el nodo en la lista de conexiones no ha sido visitado
			if (!contains(visited, adjList[current][i])){
				// Agregas ese nodo al queue para que sea agregado a la fila y se visite cuando se tenga que visitar
				qu.push(adjList[current][i]);
				//cout << "Nodo que se acaba de visitar: " << adjList[current][i] << ", Current: " << current << endl;
				/* Aquí es como tener una lista con el nodo, y quien es su padre. De esta forma
				 * tienes al nodo, y el padre, y así se puede llegar del start al goal
				 * Ya que se encontró el nodo, te vas para atrás y funciona. Por eso se usa la misma lista
				 */
				// Solo actualizo la lista cuando no se tenga un valor. Si si existe, entonces no se deja el camino más corto
				if (paths[adjList[current][i]][0] == -1){
					paths[adjList[current][i]][0] = current;
				}
			}
		}
		// Llamas a la recursión con el nuevo current
		return breadthHelper(current, goal, qu, visited, paths);
	}
}

// Checa si la lista contiene al nodo
bool Graph::contains(list<int> ls, int node){
	// Creas un iterador
	list<int>::iterator it;
	// Usas la función de find para ver si existe el elemento
	it = find(ls.begin(), ls.end(), node);
	// Si el iterador no está vacío entonces regresa true
	if(it != ls.end())
		return true;
	// Si no es falso
	else
		return false;
}

// La función que imprime los nodos que visitó
string Graph::print_visited(list<int> q){
	// Creas el stringstream
	stringstream aux;
	aux << "visited: ";
	// Recorrer la lista
	while (!q.empty()){
		// Agregar al string el frente de la lista
		aux << q.front() << " ";
		// Borrarlo de la lista hasta que quede vacío
		q.pop_front();
  	}
	// Regresar el stringstream
	return aux.str();
}

// Algoritmo que imprime el path que tomo en el DFS
string Graph::print_path(vector<vector <int> > &path, int start, int goal){
	int node =  path[goal][0];
	stack<int> reverse;
	reverse.push(goal);
	stringstream aux;
	aux  << "path:";
	while (node != start) {
		reverse.push(node);
    	node = path[node][0];
 	}
	reverse.push(start);
	while (!reverse.empty()) {
		aux << " " << reverse.top() ;
		reverse.pop();
 	}
	return aux.str();
}

#endif /* Graph_H_ */
