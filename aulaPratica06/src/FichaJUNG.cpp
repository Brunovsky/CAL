#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <iostream>
#include <sstream>

void exercicio1();
void exercicio2();
void exercicio3();


// Estrutura base de um grafo
void exercicio1() {
	// ai. Init GraphViewer
	GraphViewer* gv = new GraphViewer(600, 600, true);
	gv->createWindow(600, 600);

	// aii. Color vertices blue
	gv->defineVertexColor("blue");

	// aiii. Color edges black
	gv->defineEdgeColor("black");

	// bi. Create a vertex id=0
	gv->addNode(0);
	gv->rearrange();

	// bii. Create a vertex id=1
	gv->addNode(1);
	gv->rearrange();

	// biii. Create edges id=73, id=42
	gv->addEdge(73, 0, 1, EdgeType::UNDIRECTED);
	gv->addEdge(42, 0, 1, EdgeType::DIRECTED);
	gv->rearrange();

	// biv. Remove vertices
	gv->removeNode(1);
	gv->rearrange();

	// bv. Add vertex id=2
	gv->addNode(2);
	gv->rearrange();

	// bvi. Add edge id=73 again
	gv->addEdge(73, 0, 2, EdgeType::UNDIRECTED);
	gv->rearrange();

	// bvii. Label a vertex
	gv->setVertexLabel(2, "I'm a node!");
	gv->rearrange();

	// bviii. Label an edge
	gv->setEdgeLabel(73, "I'm an edge!");
	gv->rearrange();

	// bix. Change single vertex color
	gv->setVertexColor(1, "green");
	gv->rearrange();

	// bx. Change single edge color
	gv->setEdgeColor(73, "yellow");
	gv->rearrange();

	// bxi. Add background image.
	gv->setBackground("background.jpg");
}

void exercicio2() {
	// ai. Init GraphViewer
	GraphViewer* gv = new GraphViewer(600, 600, false);
	gv->createWindow(600, 600);

	gv->addNode(0, 300, 50);
	gv->addNode(1, 318, 58);
	gv->addNode(2, 325, 75);
	gv->addNode(3, 318, 93);
	gv->addNode(4, 300, 100);
	gv->addNode(5, 282, 93);
	gv->addNode(6, 275, 75);
	gv->addNode(7, 282, 58);
	gv->addNode(8, 150, 200);
	gv->addNode(9, 300, 200);
	gv->addNode(10, 450, 200);
	gv->addNode(11, 300, 400);
	gv->addNode(12, 200, 550);
	gv->addNode(13, 400, 550);

	gv->addEdge(0, 0, 1, EdgeType::UNDIRECTED);
	gv->addEdge(1, 1, 2, EdgeType::UNDIRECTED);
	gv->addEdge(2, 2, 3, EdgeType::UNDIRECTED);
	gv->addEdge(3, 3, 4, EdgeType::UNDIRECTED);
	gv->addEdge(4, 4, 5, EdgeType::UNDIRECTED);
	gv->addEdge(5, 5, 6, EdgeType::UNDIRECTED);
	gv->addEdge(6, 6, 7, EdgeType::UNDIRECTED);
	gv->addEdge(7, 7, 0, EdgeType::UNDIRECTED);
	gv->addEdge(8, 4, 9, EdgeType::UNDIRECTED);
	gv->addEdge(9, 9, 8, EdgeType::UNDIRECTED);
	gv->addEdge(10, 9, 10, EdgeType::UNDIRECTED);
	gv->addEdge(11, 9, 11, EdgeType::UNDIRECTED);
	gv->addEdge(12, 11, 12, EdgeType::UNDIRECTED);
	gv->addEdge(13, 11, 13, EdgeType::UNDIRECTED);

	gv->rearrange();


}

void exercicio3() {

}


int main() {
	//exercicio1();
	exercicio2();
	//exercicio3();
	getchar();
	return 0;
}
