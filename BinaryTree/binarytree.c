#include "binarytree.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * ~~Traversal 함수들은 인자로 (루트) 노드와 콜백함수를 받으며, 노드 방문마다 해당 노드를 인자로 콜백함수를 호출한다.
 * 콜백함수의 반환값이 0일 경우, Traversal 함수는 트리 순회를 중단하고 멈춘 지점의 노드를 반환한다.
 * 그 외, 트리 순회가 정상적으로 끝났을 경우, Traversal 함수는 NULL을 반환한다.
 */
typedef int(*traversalCallback)(typeNodePtr);

/**
 * 중위 순회 함수
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
 * 전위 순회 함수
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
 * 레벨 순회 함수는 큐를 이용하며, 링크드 큐를 구현하였음
 */
typedef struct queue QueueItem;
struct queue {
	typeNodePtr node;
	QueueItem* next;
};

/**
 * 주어진 노드를 갖는 큐 요소를 새로 만들어 반환하는 함수
 * 이 파일 내에서만 쓰이므로 static으로 선언함
 */
static QueueItem* createItem(typeNodePtr node) {
	QueueItem* item = (QueueItem*) malloc(sizeof(QueueItem));
	item->node = node;
	item->next = NULL;
	return item;
}

/**
 * 레벨 순회 함수
 */
typeNodePtr levelOrderTraversal(typeNodePtr root, traversalCallback callback) {
	// temp = 임시 포인터, current = 노드 큐의 헤드 원소, tail = 노드 큐의 마지막 원소
	QueueItem *temp, *current, *tail;

	// 루트 노드가 존재하지 않을 경우 바로 종료
	if(root == NULL) return NULL;

	// 처음에 루트 노드로 큐를 초기화하고,
	for(current = tail = createItem(root);
			// 큐의 현재 요소가 NULL이 아닌 경우(큐가 끝나지 않은 경우)동안,
			current != NULL;
			// 현재 요소를 메모리에서 해제시키고 다음 노드로 진행
			temp = current, current = current->next, free(temp)) {

		// callback을 호출하고, 만약 순회를 종료할 경우
		if(!callback(current->node)) {
			// 순회가 중단된 지점의 노드를 (반환하기 위해) 저장해두고,
			typeNodePtr lastNode = current->node;
			// 생성됐던 노드들의 메모리를 해제하고
			for(; current != NULL; temp = current, current = current->next, free(temp));
			// 중단 지점의 노드를 반환
			return lastNode;
		}

		// 왼쪽 자식이 있을 경우 왼쪽 자식을 큐에 추가
		if(current->node->leftChild) {
			temp = createItem(current->node->leftChild);
			tail->next = temp;
			tail = temp;
		}
		// 오른쪽 자식이 있을 경우 오른쪽 자식을 큐에 추가
		if(current->node->rightChild) {
			temp = createItem(current->node->rightChild);
			tail->next = temp;
			tail = temp;
		}
	}
	return NULL;
}

/**
 * 트리에 요소를 삽입할 때, 완전이진트리에 맞을 위치를 찾음
 * 레벨 순회를 하면서, 제일 처음 빈 자리가 있는 노드를 반환하도록 함
 */
static int findPush(typeNodePtr node) {
	if(!node->leftChild || !node->rightChild) {
		return 0;
	} else {
		return 1;
	}
}

/**
 * 트리의 새로운 노드를 만듦
 * 문자열은 내용을 복사해서 사용함
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
 * 트리에 노드를 삽입함.
 * 트리가 완전이진트리의 형태를 이루도록 위치를 탐색하여 삽입하도록 함
 * 루트 노드를 반환하여, 만약 루트 노드가 바뀐 경우 이를 반영할 수 있도록 했음
 */
typeNodePtr pushNode(typeNodePtr root, typeNodePtr node) {
	if(root == NULL) {
		return node;
	} else {
		// findPath 함수는 자식이 없는 노드를 찾음,
		// Traversal 함수는 순환이 중단된 시점의 노드를 반환하므로, findPath 함수가 지정한 위치의 노드를 반환
		typeNodePtr toPush = levelOrderTraversal(root, findPush);
		// 왼쪽 자식이 없는 경우, 거기에 노드를 삽입
		if(toPush->leftChild == NULL) {
			toPush->leftChild = node;
		} else { // 그 외, 오른쪽 자식으로 삽입.
			// findPath가 자식이 둘 중 하나 이상이 없는 노드만 반환하므로 오른쪽 자식이 없는 것이라고 볼 수 있음
			toPush->rightChild = node;
		}
		return root;
	}
}

/**
 * traversal 함수에 줄 수 있는 출력 함수
 * 노드를 출력하고 1을 반환하여 트리를 끝까지 순회함
 */
static int printNode(typeNodePtr node) {
	printf("%s\t%.1f\n", node->name, node->gpa);
	return 1;
}

/**
 * 중위 순회를 하며 노드를 출력하는 함수
 */
void inorder(typeNodePtr root) {
	inOrderTraversal(root, printNode);
}

/**
 * 전위 순회를 하며 노드를 출력하는 함수
 */
void preorder(typeNodePtr root) {
	preOrderTraversal(root, printNode);
}

/**
 * 레벨 순회를 하며 노드를 출력하는 함수
 */
void levelorder(typeNodePtr root) {
	levelOrderTraversal(root, printNode);
}
