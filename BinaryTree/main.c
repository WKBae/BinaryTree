#include "binarytree.h"
#include <stdio.h>
#include <stdlib.h>

typeNodePtr ROOT = NULL;

int main() {
	FILE* in = fopen("Studentdata.txt", "r");
	char name[30];
	double gpa;

	while(fscanf(in, "%s%lf", name, &gpa) != EOF) {
		ROOT = pushNode(ROOT, makeNode(name, gpa));
	}

	puts("<중위 순회>");
	inorder(ROOT);
	putchar('\n');
	
	puts("<전위 순회>");
	preorder(ROOT);
	putchar('\n');
	
	puts("<레벨 순회>");
	levelorder(ROOT);
	putchar('\n');

	return 0;
}
