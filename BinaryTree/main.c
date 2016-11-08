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

	puts("<���� ��ȸ>");
	inorder(ROOT);
	putchar('\n');
	
	puts("<���� ��ȸ>");
	preorder(ROOT);
	putchar('\n');
	
	puts("<���� ��ȸ>");
	levelorder(ROOT);
	putchar('\n');

	return 0;
}
