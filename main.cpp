#include <iostream>
#include "RBTree.h"
int main () {
	RBTree root;
	int a[] = {1, 4, 3, 6, 9, 7, 2, 5, 8};
	for(int i = 0;i < 9;i++) {
		root. Insert (a[i]);
	}
	root.PrintTree();
	/*
	for(int i = 0;i < 9;i++) {
		root.Print();
		root. remove(a[i]);
	}
	 */
	return 0;
}
