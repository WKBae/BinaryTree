
typedef struct node* typeNodePtr;
typedef struct node {
	char name[30];
	double gpa;
	typeNodePtr leftChild, rightChild;
} typeNode;

typeNodePtr makeNode(char name[30], double gpa);
typeNodePtr pushNode(typeNodePtr root, typeNodePtr node);
void inorder(typeNodePtr root);
void preorder(typeNodePtr root);
void levelorder(typeNodePtr root);
