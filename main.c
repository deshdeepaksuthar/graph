#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>

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
	xnodes[0] = 0.5;
	ynodes[0] = 0.5;



	// time for animation
	float dt = 0.1;
	float sigma = 0.06;
	float epsilon = 1e2;
	// v = 4 epsilon ( (sigma/r)**12 - (sigma/r)**6)
	// f = 24 epsilon ( -2(sigma/r)**13 rvec + (sigma/r)**7 rvec)
	SetTargetFPS(20);
	while(!WindowShouldClose()){
		wwidth = GetScreenWidth();
		wheight = GetScreenHeight();
		BeginDrawing();
		ClearBackground(BLACK);
		DrawText("Press Esc to quit!",10,10,  20, WHITE);

		// TODO: remove this anchoring
		xnodes[0] = 0.5;
		ynodes[0] = 0.5;
		// update the positions due to the  forces
		// force only due to  links
		// from reamains fixes, to gets changed
		//making the links
		// only attracion is not good
		for(int i=0; i< num_links; i++) {
			float r = 0;
			float rx = -xnodes[from[i]-1] + xnodes[to[i]-1];
			float ry = -ynodes[from[i]-1] + ynodes[to[i]-1];
			r = sqrt( (rx)* (rx) + (ry) * (ry) );
			float sbr = sigma/r;
			float fac = 24.0*epsilon*pow(sbr, 7)*(2*pow(sbr, 6) - 1.0);
			// force == acceleration
			fac = -1;
			float dx ;
			float dy ;
			if(r>=0.2){
				dx = fac*rx;
				dy = fac*ry;
			}else {
				dx = 0 ;
				dy = 0 ;
			}
			xnodes[to[i]-1] += 0.5 * dx *dt*dt/r;
			ynodes[to[i]-1] += 0.5 * dy *dt*dt/r;
			xnodes[from[i]-1] -= 0.5 * dx *dt*dt/r;
			ynodes[from[i]-1] -= 0.5 * dy *dt*dt/r;

			char buf[80];
			sprintf(buf, "dx: %0.1f\n", dx);
			// printf( "dx: %0.1f\n", dx);
			// printf( "r: %0.1f\n", r);
			// printf( "fac: %0.1f\n", fac);
			DrawText(buf, 20,50,20, WHITE);
		}
		// repulsion among allnodes
		for(int i=0; i< num_nodes; i++) {
			for(int j=0; j< num_nodes; j++) {
			if(i!=j){
			float r = 0;
			float rx = -xnodes[i] + xnodes[j];
			float ry = -ynodes[i] + ynodes[j];
			r = sqrt( (rx)* (rx) + (ry) * (ry) );
			float sbr = sigma/r;
			float fac ;
			// force == acceleration
			fac = 1e-3;
			float dx ;
			float dy ;
			if( r<0.8){
				dx = fac/(r+0.01);
                                dy = fac/(r+0.01);
			}else {
				dx = 0 ;
				dy = 0 ;
			}
			xnodes[i] += 0.5 * dx *dt*dt;
			ynodes[i] += 0.5 * dy *dt*dt;

			char buf[80];
			sprintf(buf, "dx: %0.1f\n", dx);
			// printf( "dx: %0.1f\n", dx);
			// printf( "r: %0.1f\n", r);
			// printf( "fac: %0.1f\n", fac);
			DrawText(buf, 20,50,20, WHITE);
			}
			}
		}


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

