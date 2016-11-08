#include "binarytree.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * ~~Traversal �Լ����� ���ڷ� (��Ʈ) ���� �ݹ��Լ��� ������, ��� �湮���� �ش� ��带 ���ڷ� �ݹ��Լ��� ȣ���Ѵ�.
 * �ݹ��Լ��� ��ȯ���� 0�� ���, Traversal �Լ��� Ʈ�� ��ȸ�� �ߴ��ϰ� ���� ������ ��带 ��ȯ�Ѵ�.
 * �� ��, Ʈ�� ��ȸ�� ���������� ������ ���, Traversal �Լ��� NULL�� ��ȯ�Ѵ�.
 */
typedef int(*traversalCallback)(typeNodePtr);

/**
 * ���� ��ȸ �Լ�
 */
typeNodePtr inOrderTraversal(typeNodePtr node, traversalCallback callback) {
	if(node) {
		typeNodePtr ret;

		ret = inOrderTraversal(node->leftChild, callback);
		if(ret != NULL) return ret;

		if(!callback(node)) return node;

		ret = inOrderTraversal(node->rightChild, callback);
		if(ret != NULL) return ret;

		return NULL;
	} else {
		return NULL;
	}
}

/**
 * ���� ��ȸ �Լ�
 */
typeNodePtr preOrderTraversal(typeNodePtr node, traversalCallback callback) {
	if(node) {
		typeNodePtr ret;

		if(!callback(node)) return node;

		ret = preOrderTraversal(node->leftChild, callback);
		if(ret != NULL) return ret;

		ret = preOrderTraversal(node->rightChild, callback);
		if(ret != NULL) return ret;

		return NULL;
	} else {
		return NULL;
	}
}

/*
 * ���� ��ȸ �Լ��� ť�� �̿��ϸ�, ��ũ�� ť�� �����Ͽ���
 */
typedef struct queue QueueItem;
struct queue {
	typeNodePtr node;
	QueueItem* next;
};

/**
 * �־��� ��带 ���� ť ��Ҹ� ���� ����� ��ȯ�ϴ� �Լ�
 * �� ���� �������� ���̹Ƿ� static���� ������
 */
static QueueItem* createItem(typeNodePtr node) {
	QueueItem* item = (QueueItem*) malloc(sizeof(QueueItem));
	item->node = node;
	item->next = NULL;
	return item;
}

/**
 * ���� ��ȸ �Լ�
 */
typeNodePtr levelOrderTraversal(typeNodePtr root, traversalCallback callback) {
	// temp = �ӽ� ������, current = ��� ť�� ��� ����, tail = ��� ť�� ������ ����
	QueueItem *temp, *current, *tail;

	// ��Ʈ ��尡 �������� ���� ��� �ٷ� ����
	if(root == NULL) return NULL;

	// ó���� ��Ʈ ���� ť�� �ʱ�ȭ�ϰ�,
	for(current = tail = createItem(root);
			// ť�� ���� ��Ұ� NULL�� �ƴ� ���(ť�� ������ ���� ���)����,
			current != NULL;
			// ���� ��Ҹ� �޸𸮿��� ������Ű�� ���� ���� ����
			temp = current, current = current->next, free(temp)) {

		// callback�� ȣ���ϰ�, ���� ��ȸ�� ������ ���
		if(!callback(current->node)) {
			// ��ȸ�� �ߴܵ� ������ ��带 (��ȯ�ϱ� ����) �����صΰ�,
			typeNodePtr lastNode = current->node;
			// �����ƴ� ������ �޸𸮸� �����ϰ�
			for(; current != NULL; temp = current, current = current->next, free(temp));
			// �ߴ� ������ ��带 ��ȯ
			return lastNode;
		}

		// ���� �ڽ��� ���� ��� ���� �ڽ��� ť�� �߰�
		if(current->node->leftChild) {
			temp = createItem(current->node->leftChild);
			tail->next = temp;
			tail = temp;
		}
		// ������ �ڽ��� ���� ��� ������ �ڽ��� ť�� �߰�
		if(current->node->rightChild) {
			temp = createItem(current->node->rightChild);
			tail->next = temp;
			tail = temp;
		}
	}
	return NULL;
}

/**
 * Ʈ���� ��Ҹ� ������ ��, ��������Ʈ���� ���� ��ġ�� ã��
 * ���� ��ȸ�� �ϸ鼭, ���� ó�� �� �ڸ��� �ִ� ��带 ��ȯ�ϵ��� ��
 */
static int findPush(typeNodePtr node) {
	if(!node->leftChild || !node->rightChild) {
		return 0;
	} else {
		return 1;
	}
}

/**
 * Ʈ���� ���ο� ��带 ����
 * ���ڿ��� ������ �����ؼ� �����
 */
typeNodePtr makeNode(char name[30], double gpa) {
	int i;
	typeNodePtr node = (typeNodePtr) malloc(sizeof(typeNode));
	for(i = 0; name[i] != '\0'; i++) {
		node->name[i] = name[i];
	}
	node->name[i] = '\0';
	node->gpa = gpa;
	node->leftChild = node->rightChild = NULL;
	return node;
}

/**
 * Ʈ���� ��带 ������.
 * Ʈ���� ��������Ʈ���� ���¸� �̷絵�� ��ġ�� Ž���Ͽ� �����ϵ��� ��
 * ��Ʈ ��带 ��ȯ�Ͽ�, ���� ��Ʈ ��尡 �ٲ� ��� �̸� �ݿ��� �� �ֵ��� ����
 */
typeNodePtr pushNode(typeNodePtr root, typeNodePtr node) {
	if(root == NULL) {
		return node;
	} else {
		// findPath �Լ��� �ڽ��� ���� ��带 ã��,
		// Traversal �Լ��� ��ȯ�� �ߴܵ� ������ ��带 ��ȯ�ϹǷ�, findPath �Լ��� ������ ��ġ�� ��带 ��ȯ
		typeNodePtr toPush = levelOrderTraversal(root, findPush);
		// ���� �ڽ��� ���� ���, �ű⿡ ��带 ����
		if(toPush->leftChild == NULL) {
			toPush->leftChild = node;
		} else { // �� ��, ������ �ڽ����� ����.
			// findPath�� �ڽ��� �� �� �ϳ� �̻��� ���� ��常 ��ȯ�ϹǷ� ������ �ڽ��� ���� ���̶�� �� �� ����
			toPush->rightChild = node;
		}
		return root;
	}
}

/**
 * traversal �Լ��� �� �� �ִ� ��� �Լ�
 * ��带 ����ϰ� 1�� ��ȯ�Ͽ� Ʈ���� ������ ��ȸ��
 */
static int printNode(typeNodePtr node) {
	printf("%s\t%.1f\n", node->name, node->gpa);
	return 1;
}

/**
 * ���� ��ȸ�� �ϸ� ��带 ����ϴ� �Լ�
 */
void inorder(typeNodePtr root) {
	inOrderTraversal(root, printNode);
}

/**
 * ���� ��ȸ�� �ϸ� ��带 ����ϴ� �Լ�
 */
void preorder(typeNodePtr root) {
	preOrderTraversal(root, printNode);
}

/**
 * ���� ��ȸ�� �ϸ� ��带 ����ϴ� �Լ�
 */
void levelorder(typeNodePtr root) {
	levelOrderTraversal(root, printNode);
}
