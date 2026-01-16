#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include "raylib.h"

#include "lib/llist.h"

#define NODE_RADIUS 10
#define CELL_COLOR GREEN
#define SOURCE_COLOR RED

void draw_nodes(Nlist *nl_iterator, int radius, Color color);
Nlist* generate_sources(int nr_sources, int radius, int screen_width, int screen_height);
Node* get_deltas(Nlist *cl, Nlist *sl);
void draw_deltas(Nlist *sl, Node deltas[]);

int main()
{
	const int screen_width = 800;
	const int screen_height = 500;

	InitWindow(screen_width, screen_height, "simulation");

	//initial nodes
	Node *cell1 = malloc(sizeof(Node));
	Node *cell2 = malloc(sizeof(Node));
	Node *cell3 = malloc(sizeof(Node));
	cell1->x = screen_width/2; cell1->y = screen_height*4/5;
	cell2->x = cell1->x; cell2->y = cell1->y - 2*NODE_RADIUS;
	cell3->x = cell1->x; cell3->y = cell1->y - 4*NODE_RADIUS;

	//initialize node-list
	Nlist *cl_head = malloc(sizeof(Nlist));
	cl_head->node = cell1;
	cl_head->next = NULL;
	Npush(cl_head, cell2);
	Npush(cl_head, cell3);

	Nlist *sl_head = NULL;
	//define nr_sources
	int nr_sources = 4;	

	Node *deltas = NULL;

	while(!WindowShouldClose())
	{
		if(IsKeyPressed(KEY_SPACE))
		{
			sl_head = generate_sources(nr_sources, NODE_RADIUS, screen_width, screen_height);
			deltas = get_deltas(cl_head, sl_head);
		}
		draw_nodes(cl_head, NODE_RADIUS, CELL_COLOR);
		draw_nodes(sl_head, NODE_RADIUS, WHITE);
		draw_deltas(sl_head, deltas);
	}

	CloseWindow();

	Nfree(cl_head);

	return 0;
}

void draw_nodes(Nlist *nl_iterator, int radius, Color color)
{
	if(nl_iterator == NULL) return;
	BeginDrawing();
	while(nl_iterator->next != NULL)
	{
		DrawCircle(nl_iterator->node->x, nl_iterator->node->y, radius, color);
		nl_iterator = nl_iterator->next;
	}
	DrawCircle(nl_iterator->node->x, nl_iterator->node->y, radius, color);
	EndDrawing();
}

Nlist* generate_sources(int nr_sources, int radius, int screen_width, int screen_height)
{
	if (nr_sources == 0) return NULL;
	Nlist *sl_head = malloc(sizeof(Node));
	Nlist *sl = sl_head;
	for(int i = 0; i < nr_sources - 1; i++)
	{
		Node *source = malloc(sizeof(Node));
		source->x = (rand() % (screen_width - 2*radius)) + radius;
		source->y = (rand() % (screen_height - 2*radius)) + radius;
		sl->node = source;
		sl->next = malloc(sizeof(Nlist));
		sl = sl->next;
	}
	Node *source = malloc(sizeof(Node));
	source->x = (rand() % (screen_width - 2*radius)) + radius;
	source->y = (rand() % (screen_height - 2*radius)) + radius;
	sl->node = source;
	sl->next = NULL;
	printf("SOURCES GENERATED!\n");
	return sl_head;
}

Node* get_deltas(Nlist *cl, Nlist *sl)
{
	int sl_length = Nlength(sl);
	int cl_length = Nlength(cl);
	Node* deltas = malloc(sl_length * sizeof(Node));

	Node *max_cell;
	float max_value;

	for(int i = 0; i < sl_length; i++)
	{
		max_cell = malloc(sizeof(Node));
		max_value = FLT_MAX;
		for(int j = 0; j < cl_length; j++)
		{
			Node *cell = Nget(cl, j);
			Node *source = Nget(sl, i);
			int dx = source->x - cell->x;
			int dy = source->y - cell->y;
			double dist = sqrt((double)(dx*dx + dy*dy));
			if(dist < max_value)
			{
				max_value = dist;
				max_cell->x = dx;
				max_cell->y = dy;
			}
		}
		deltas[i] = *max_cell;
	}	
	return deltas;
}

void draw_deltas(Nlist *sl, Node deltas[])
{
	if(deltas == NULL) return;
	BeginDrawing();
	for(int i = 0; i < Nlength(sl); i++)
	{
		Node *source = Nget(sl, i);
		DrawLine(source->x, source->y, source->x - deltas[i].x, source->y - deltas[i].y, WHITE);
	}
	EndDrawing();
}
		
