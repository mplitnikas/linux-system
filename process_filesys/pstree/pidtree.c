#include "pidtree.h"

//struct Node {
//				char *pid;
//				char *ppid;
//				char *name;
//				int depth;
//				struct Node *firstChild;
//				struct Node *nextSibling;
//};

struct Node *createNode(char *pid, char *name, char *ppid, int depth) {
				struct Node *n = malloc(sizeof(*n));
				strncpy(n->pid, pid, 64);
				strncpy(n->name, name, 64);
				strncpy(n->ppid, ppid, 64);
				n->depth = 0;
				return n;
}

void destroyNode(struct Node *node) {
				free(node);
}

void addToParentNode(struct Node *root, struct Node *node) {
				node->depth = root->depth + 1;
				if (root->firstChild == NULL) {
								root->firstChild = node;
				} else {
								addToSiblings(root->firstChild, node);
				}
}

void addToSiblings(struct Node *root, struct Node *node) {
				if (root == NULL) return;
				struct Node *target = root;
				while (target->nextSibling != NULL) {
								target = target->nextSibling;
				}
				target->nextSibling = node;
}

struct Node *popFromSiblings(struct Node *node) {
				struct Node *prev = node;
				struct Node *curr = node;
				while (curr->nextSibling != NULL) {
								prev = curr;
								curr = curr->nextSibling;
				}
				prev->nextSibling = NULL;
				return curr;
}

struct Node *findNodeByPid(struct Node *current, char *pid) {
				if (current == NULL) return NULL;
				struct Node *result = NULL;
				if (strcmp(current->pid, pid) == 0) result = current;
				else if (result == NULL && current->firstChild != NULL) result =  findNodeByPid(current->firstChild, pid);
				else if (result == NULL && current->nextSibling != NULL) result = findNodeByPid(current->nextSibling, pid);
				return result;
}

void forEachNodeDepthFirst(struct Node *current, void (*callback)(struct Node *node)) {
				if (current == NULL) return;
				callback(current);
				if (current->firstChild != NULL) {
								forEachNodeDepthFirst(current->firstChild, callback);
				}
				if (current->nextSibling != NULL) {
								forEachNodeDepthFirst(current->nextSibling, callback);
				}
}

void printNode(struct Node *node) {
				if (node == NULL) return;
				char buf[256] = "";
				printf("%s%s (%s)\n", indent(buf, node->depth), node->name, node->pid);
}

char *indent(char *buf, int depth) {
				for (int i = 0; i < depth; i++) {
								strcat(buf, "  ");
				}
				return buf;
}
