#include <stdio.h>
#include <string.h>
#include "foreachproc.h"
#include "pidtree.h"

void addProcToTree(FILE *fd, char *pid);
int addNodeToTree(struct Node *tree, struct Node *node);
void getPropertyFromStatus(char* buf, int len, FILE *fd, char *key);
struct Node *treeBase;
struct Node *orphanBase; // orphans as siblings only

int main(int argc, char * argv[]) {
				treeBase = createNode("0", "login", "-1", -1);
				orphanBase = createNode("-1", "orphan", "-2", -1);

				forEachProcessDir(addProcToTree);
				struct Node *nextOrphan = popFromSiblings(orphanBase);
				while (1) {
								nextOrphan = popFromSiblings(orphanBase);
								if (nextOrphan != NULL && (strcmp(nextOrphan->pid, "-1") != 0)) {
												if (addNodeToTree(treeBase, nextOrphan)) continue;
												if (addNodeToTree(orphanBase, nextOrphan)) continue;
												destroyNode(nextOrphan);
												continue;
								} else {
												break;
								}
				} 
				forEachNodeDepthFirst(treeBase, printNode);
				//printf("--------------\n");
				//forEachNodeDepthFirst(orphanBase, printNode);
				return 0;
}

void addProcToTree(FILE *fd, char *pid) {
				int len = 64;
				char ppidBuf[len];
				char nameBuf[len];
				getPropertyFromStatus(ppidBuf, len, fd, "PPid");
				getPropertyFromStatus(nameBuf, len, fd, "Name");

				struct Node *n = createNode(pid, nameBuf, ppidBuf, 0);
				if (addNodeToTree(treeBase, n)) return;
				else addToSiblings(orphanBase, n);
}

int addNodeToTree(struct Node *tree, struct Node *node) {
				if (node == NULL || node->ppid == NULL) return -1;
				struct Node *parent = findNodeByPid(tree, node->ppid);
				if (parent != NULL) {
								addToParentNode(parent, node);
								return 1;
				} else {
								return 0;
				}
}

void getPropertyFromStatus(char* buf, int len, FILE *fd, char *key) {
				char line[256];
				char *delim = "\t";
				char lineKey[strlen(key)+2];

				strcpy(lineKey, key);
				strcat(lineKey, ":");
				strcat(lineKey, delim);
				while (fgets(line, 256, fd) != NULL) {
								if (strncmp(line, lineKey, strlen(lineKey)) == 0) {
												strncpy(buf, 
																strtok(strtok(line+strlen(lineKey), delim), "\n")
																, len);
								}
				}
				rewind(fd);
}
