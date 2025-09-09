#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>


int main()
{
	int wwidth = 100, wheight = 100;
	const char *title = "Graphs";
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(wwidth, wheight, title);


	// reading the file
	FILE *f = fopen("file.txt", "r");
	if (f == NULL){
		fprintf(stderr, "cannot open the file name\n");
		goto EXIT;
	}
	// loading the graph in the memory
	char buff[80];
	int a,b ;
	int num_links = 0;
	int num_nodes = 0;
	int max_node;
	// don't want to make a fancy data structure so
	// doing a stupid thing
	while(fscanf(f, "%d-%d\n", &a, &b) != EOF) {
		num_links +=1;
	}
	int *from;
	int *to;
	from = malloc(sizeof(int)*num_links);
	to = malloc(sizeof(int)*num_links);
	num_nodes = a;
	rewind(f);


	int count = 0;
	while(fscanf(f, "%d-%d\n", &a, &b) != EOF) {
		printf("nodes %d - %d\n", a , b);
		from[count] = a;
		to[count] = b;
		if(num_nodes<a)
			num_nodes = a;
		if(num_nodes<b)
			num_nodes = b;
		count++;
	}

	float *xnodes, *ynodes;
	xnodes = malloc(sizeof(float)*num_nodes);
	ynodes = malloc(sizeof(float)*num_nodes);
	srand(42);
	for(int i=0; i< num_nodes; i++){
		xnodes[i]  = (float)rand()/RAND_MAX;
	        ynodes[i]  = (float)rand()/RAND_MAX;
	}



	SetTargetFPS(60);
	while(!WindowShouldClose()){
		wwidth = GetScreenWidth();
		wheight = GetScreenHeight();
		BeginDrawing();
		ClearBackground(BLACK);
		DrawText("Press Esc to quit!",10,10,  20, WHITE);


		//making the links
		for(int i=0; i< num_links; i++) {
			DrawLine(wwidth*xnodes[from[i]-1 ], wheight*ynodes[from[i]-1 ],
				 wwidth*xnodes[to[i]-1 ], wheight*ynodes[to[i] -1],
				 GREEN);
		}


		// making one node
		for(int i=0; i< num_nodes; i++){
			DrawCircle(wwidth*xnodes[i], wheight*ynodes[i], 10, WHITE);
			char buf[10];
			sprintf(buf, "%d", i+1);
			DrawText(buf, wwidth*xnodes[i], wheight*ynodes[i], 20, RED);
		}

		EndDrawing();
	}

	fclose(f);
	free(xnodes);
	free(ynodes);
	free(from);
	free(to);
EXIT:
	CloseWindow();
	return 0;

}

