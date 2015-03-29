#include <iostream>
#include <ctime>
#include <cstdlib>
#include "AvlTree.h"

int main(void)
{
	srand(time(NULL));
	AvlTree<int> tree;
	set<int> b_tree;
	clock_t start, finish;

	start = clock();
	for (int i = 0; i < 10000000; i++){
		int temp = rand() % (1 << 31);
		b_tree.insert(temp);
	}
	for (int i = 0; i < 1000000; i++){
		int temp = rand() % (1 << 31);
		b_tree.erase(temp);
	}
	finish = clock();

	cout << "std time out:" << (double)(finish - start) << "ms" << endl;


	start = clock();
	for (int i = 0; i < 10000000; i++){
		int temp = rand()%(1<<31);
		tree.Insert(temp);
	}
	for (int i = 0; i < 1000000; i++){
		int temp = rand() % (1 << 31);
		tree.Delete(temp);
	}
	finish = clock();

	cout << "my time out:" << (double)(finish - start)<<"ms" << endl;
	
	return 0;
}
