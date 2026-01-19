#include <stdio.h>
#include <stdlib.h>
#include "llist.h"

void Nappend(Nlist *nl, Node *node)
{
	if(nl->node == NULL)
	{
		nl->node = node;
		nl->next = NULL;
		return;
	}
	while(nl->next != NULL)
	{
		nl = nl->next;
	}
	Nlist *new_elem = malloc(sizeof(Nlist));
	new_elem->node = node;
	new_elem->next = NULL;
	nl->next = new_elem;
}

int Nremove(Nlist *nl, int index)
{
	if(index < 0 || index > Nlength(nl) - 1) return -1;
	Nlist *next_ptr;
	if(nl->next == NULL)
	{
		free(nl->node);
		nl->node = NULL;
		return 0;
	}
	if(index == 0)
	{
		next_ptr = nl->next;
		nl->node = nl->next->node;
		nl->next = nl->next->next;
		free(next_ptr);
		return 0;
	}
	for(int i = 0; i < index - 1; i++)
	{
		nl = nl->next;	
	}
	next_ptr = nl->next;
	nl->next = nl->next->next;
	free(next_ptr);
	return 0;
}

int Nlength(Nlist *nl)
{
	if(nl == NULL || nl->node == NULL) return 0;
	int i = 1;
	while(nl->next != NULL)
	{
		nl = nl->next;
		i++;
	}
	return i;
}

Node* Nget(Nlist *nl, int index)
{
	if(index < 0 || index > Nlength(nl) - 1 || nl == NULL) return NULL;
	for(int i = 0; i < index; i++)
	{
		nl = nl->next;
	}
	return nl->node;	
}

void Nfree(Nlist *nl)
{
	bool first_it = true;
	while(nl->next != NULL)
	{
		nl->node->x = 0;
		nl->node->y = 0;
		free(nl->node);
		Nlist *prev_ptr = nl;
		nl = nl->next;
		if(first_it) first_it = false;
		else free(prev_ptr);
	}
	free(nl->node);
}

void Nprint(Nlist *nl, char *name)
{
	printf("%s -> ", name);
	if(nl->node == NULL) 
	{
		printf("\n");
		return;
	}
	while(nl->next != NULL)
	{
		printf("x:%d,y:%d -> ", nl->node->x, nl->node->y);
		nl = nl->next;
	}
	printf("x:%d,y:%d\n", nl->node->x, nl->node->y);
}

void print_node(Node *node)
{
	if(node == NULL)
	{
		printf("Node is empty!\n");
		return;
	}
	printf("x:%d,y:%d\n", node->x, node->y);
}
/*
int main()
{
	Node *n1 = malloc(sizeof(Node));	
	Node *n2 = malloc(sizeof(Node));
	Node *n3 = malloc(sizeof(Node));  
	Node *n4 = malloc(sizeof(Node));  

	n1->x = 1; n1->y = 1;
	n2->x = 2; n2->y = 2;
	n3->x = 3; n3->y = 3;
	n4->x = 4; n4->y = 4;
	
	Nlist *nl_head = malloc(sizeof(Nlist));

	nl_head->node = n1;
	nl_head->next = NULL;

	Npush(nl_head, n2);
	Npush(nl_head, n3);
	Npush(nl_head, n4);
	
	Nprint(nl_head, "list");
	print_node(Nget(nl_head, 6));
	Nremove(nl_head, 2);
	Nprint(nl_head, "list");

	print_node(Nget(nl_head, 1));

	Nfree(nl_head);

	return 0;
}
*/
