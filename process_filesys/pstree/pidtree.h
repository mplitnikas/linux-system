#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
				char pid[64];
				char name[64];
				char ppid[64];
				int depth;
				struct Node *firstChild;
				struct Node *nextSibling;
};

struct Node *createNode(char *pid, char *name, char *ppid, int depth);
void destroyNode(struct Node *node);
void addToParentNode(struct Node *root, struct Node *node);
void addToSiblings(struct Node *root, struct Node *node);
struct Node *popFromSiblings(struct Node *root);
struct Node *findNodeByPid(struct Node *current, char *pid);
void forEachNodeDepthFirst(struct Node *current, void (*callback)(struct Node *node));
void printNode(struct Node *node);
char *indent(char *buf, int depth);
