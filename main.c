#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <float.h>
#include <time.h>

#include "raylib.h"
#include "lib/llist.h"

#define PI 3.14159265358979323846

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 1000
#define FPS 30

#define NODE_RADIUS 5
#define EXTINCTION_RADIUS 20
#define CELL_COLOR GREEN
#define SOURCE_COLOR RED

void generate_sources_rectangular(Nlist *sl_head, int nr_sources, int radius, int width, int height);
void generate_sources_circular(Nlist *sl_head, int nr_sources, int source_radius, int circle_radius);

void extinction(Nlist *cl, Nlist *sl, int radius);
void generate_cells(Nlist *cl, Nlist **deltas, int radius);
Nlist** get_deltas(Nlist *cl, Nlist *sl);

void draw_nodes(Nlist *nl_iterator, int radius, Color color);

double l2norm(double x, double y);
double node_l2norm(Node *node);

int main()
{
	srand(time(NULL));

	int leaf_width = 40;
	int leaf_height = 40;
	int radius = 20;

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "simulation");
	SetTargetFPS(FPS);

	//initial nodes
	Node *cell1 = malloc(sizeof(Node));
	cell1->x = SCREEN_WIDTH/2; cell1->y = SCREEN_HEIGHT*4/5;

	//initialize node-list
	Nlist *cl_head = malloc(sizeof(Nlist));
	cl_head->node = cell1;
	cl_head->next = NULL;

	//define nr_sources
	int nr_sources = 4;	

	Nlist *sl_head = malloc(sizeof(Nlist));
	sl_head->node = NULL;
	sl_head->next = NULL;
	Nlist **deltas = NULL;

	bool first_cycle = false;

	while(!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		
		//if(IsKeyPressed(KEY_SPACE))
		{
			//leaf_width += 5;
			//leaf_height += 5;
			if (radius < 400) radius +=1;

			//generate_sources_rectangular(sl_head, nr_sources, NODE_RADIUS, leaf_width, leaf_height);
			generate_sources_circular(sl_head, nr_sources, NODE_RADIUS, radius);

			extinction(cl_head, sl_head, EXTINCTION_RADIUS);
			deltas = get_deltas(cl_head, sl_head);
			generate_cells(cl_head, deltas, NODE_RADIUS);

			//Nprint(sl_head, "sl");
		}
		draw_nodes(cl_head, NODE_RADIUS, CELL_COLOR);
		draw_nodes(sl_head, NODE_RADIUS, WHITE);
		EndDrawing();
	}


	CloseWindow();

	Nfree(cl_head);

	return 0;
}

void draw_nodes(Nlist *nl_iterator, int radius, Color color)
{
	if(nl_iterator == NULL || nl_iterator->node == NULL) return;
	while(nl_iterator->next != NULL)
	{
		DrawCircle(nl_iterator->node->x, nl_iterator->node->y, radius, color);
		nl_iterator = nl_iterator->next;
	}
	DrawCircle(nl_iterator->node->x, nl_iterator->node->y, radius, color);
}

void generate_sources_rectangular(Nlist *sl_head, int nr_sources, int radius, int width, int height)
{
	if(nr_sources == 0) return;
	for(int i = 0; i < nr_sources; i++)
	{
		Node *source = malloc(sizeof(Node));
		source->x = (rand() % (width - 2*radius)) + radius + (int)(SCREEN_WIDTH/2 - width/2);
		source->y = (rand() % (height - 2*radius)) + radius + (int)(SCREEN_HEIGHT/2 - height/2);
		Nappend(sl_head, source);
	}
}

void generate_sources_circular(Nlist *sl_head, int nr_sources, int source_radius, int circle_radius)
{
	if(nr_sources == 0) return;
	for(int i = 0; i < nr_sources; i++)
	{
		Node *source = malloc(sizeof(Node));
		int r = rand() % circle_radius;
		int phi= rand() % 360; //angle in degrees
		source->x = r * cos(phi * PI / 180) + SCREEN_WIDTH/2;
		source->y = r * sin(phi * PI / 180) + SCREEN_HEIGHT/2;
		Nappend(sl_head, source);
	}
}

void extinction(Nlist *cl, Nlist *sl, int radius)
{
	if(sl->node == NULL) return;
	int cl_length = Nlength(cl);
	for(int i = 0; i < Nlength(sl); i++)
	{
		Node *source = Nget(sl, i);
		for(int j = 0; j < cl_length; j++)
		{
			Node *cell = Nget(cl, j);
			if(l2norm((float)(cell->x - source->x), (float)(cell->y - source->y)) < radius)
			{
				Nremove(sl, i);
				i--;
				break;
			}
		}
		if(Nlength(sl) == 0) break;
	}
}


void generate_cells(Nlist* cl, Nlist **deltas, int radius)
{
	if(deltas == NULL) return;
	int cl_length = Nlength(cl);
	for(int i = 0; i < cl_length; i++)
	{
		Nlist *delta = deltas[i];
		double dx = 0;
		double dy = 0;
		if(delta->node == NULL) continue;
		for(int j = 0; j < Nlength(delta); j++)
		{
			double magnitude = node_l2norm(Nget(delta, j));
			dx += Nget(delta, j)->x / magnitude;
			dy += Nget(delta, j)->y / magnitude;
		}
		double magnitude = l2norm(dx, dy);
		Node *node = malloc(sizeof(Node));
		node->x = (int) round(2*radius*dx / magnitude) + Nget(cl, i)->x;	
		node->y = (int) round(2*radius*dy / magnitude) + Nget(cl, i)->y;	
		Nappend(cl, node);
	}
}

Nlist** get_deltas(Nlist *cl, Nlist *sl)
{
	if(sl->node == NULL) return NULL;
	int sl_length = Nlength(sl);
	int cl_length = Nlength(cl);
	Nlist** deltas = malloc(cl_length * sizeof(Nlist*));

	for(int i = 0; i < cl_length; i++)
	{
		deltas[i] = malloc(sizeof(Nlist));
		deltas[i]->node = NULL;
		deltas[i]->next = NULL;
	}


	Node *min_delta;
	Node *source;
	Node *cell;
	int min_index;
	int dx;
	int dy;
	double dist;
	double min_dist;

	for(int i = 0; i < sl_length; i++)
	{
		source = Nget(sl, i);
		min_delta= malloc(sizeof(Node));
		min_dist = FLT_MAX;
		min_index = 0;
		for(int j = 0; j < cl_length; j++)
		{
			cell = Nget(cl, j);
			dx = source->x - cell->x;
			dy = source->y - cell->y;
			dist = l2norm((double)dx, (double)dy);
			if(dist < min_dist)
			{
				min_dist = dist;
				min_delta->x = dx;
				min_delta->y = dy;
				min_index = j;
			}
		}
		Nappend(deltas[min_index], min_delta);
	}	
	return deltas;
}

double l2norm(double x, double y)
{
	return sqrt(x*x + y*y);
}
		
double node_l2norm(Node *node)
{
	return l2norm((double) node->x, (double) node->y);
}
