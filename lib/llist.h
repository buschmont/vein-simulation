#pragma once

#include <stdbool.h>

typedef struct Node
{
	int x;
	int y;
} Node;

typedef struct Nlist
{
	Node *node;
	struct Nlist *next;
} Nlist;
	
void Nappend(Nlist *nl, Node *node);

int Nremove(Nlist *nl, int index);

int Nlength(Nlist *nl);

Node* Nget(Nlist *nl, int index);

void Nfree(Nlist *nl);

void Nprint(Nlist *nl, char *name);

void print_node(Node *node);
