/*
 *  randtest.cpp
 *  spinify2
 *
 *  Created by Loïc Séguin-Charbonneau on 10-01-04.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include <iostream>
#include "../src/tezuka.h"

using namespace std;

int main () {
	for (int i = 0; i < 100; i++) {
		cout << alea() << endl;
	}
	return 0;
}