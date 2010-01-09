#include "Graph.h"
#include "Simul.h"
#include <iostream>

int main (int argc, char * const argv[]) {
    // insert code here...
    std::cout << "SPINIFY\nThe Ising model simulator\n\n";
	Graph G;
	G.initRect(3, 5);
	Simul S(&G);
	for (int i = 0; i < 10; i++) {
		S.thermalize();
	}
	
    return 0;
}
