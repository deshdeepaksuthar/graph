#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[])
{
	if(argc ==1){
		printf("Insufficient arguments\n");
		printf("Usage: %s <filename>\n", argv[0]);
		return 1;
	}
	int wwidth = 100, wheight = 100;
	const char *title = "Graphs";
	float scale = 1.0;
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
	InitWindow(wwidth, wheight, title);


	// reading the file
	FILE *f = fopen(argv[1], "r");
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
	fclose(f);

	float *xnodes, *ynodes;
	xnodes = malloc(sizeof(float)*num_nodes);
	ynodes = malloc(sizeof(float)*num_nodes);
	srand(42);
	int seed= (int)(time(NULL));
	srand(seed);
	for(int i=0; i< num_nodes; i++){
		xnodes[i]  = (float)rand()/RAND_MAX;
	        ynodes[i]  = (float)rand()/RAND_MAX;
	}
	xnodes[0] = 0.5;
	ynodes[0] = 0.5;



	// time for animation
	float dt = 0.1;
	// sigma = r_opt /  2**1/6
	float sigma = 0.01/pow(2,1.0/6.0);
	// depth of the potential
	float epsilon = 1e1;
	// v = 4 epsilon ( (sigma/r)**12 - (sigma/r)**6)
	// f = 24 epsilon ( -2(sigma/r)**13 rvec + (sigma/r)**7 rvec)
	SetTargetFPS(120);
	Color BG;
	BG.r = 0x0;
	BG.g = 0x0;
	BG.b = 0x0;
	BG.a = 0xff;
	char sseed[100];
	sprintf(sseed, "Seed is %d\n", seed);
	while(!WindowShouldClose()){
		wwidth = GetScreenWidth();
		wheight = GetScreenHeight();
		BeginDrawing();
		ClearBackground(BG);
		DrawText("Press Esc to quit!, mouse scroll to partial zoom",10,10,  20, WHITE);
		DrawText("Left mouse button to pan.", 10, 30,  20, WHITE);
		DrawText(sseed, 10, 50,  20, WHITE);

		// TODO: remove this anchoring
		// xnodes[0] = 0.5;
		// ynodes[0] = 0.5;
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

			float p = 0.001;
			float s = 0.9;
			fac = p*pow(r, 0.5) + s/r - p*p ;
			fac = p/r - r  ;
			float dx ;
			float dy ;
			// if(r>=0.1){
			if(true){
				// dx = fac*rx + 0.05*fac/rx ;
				// dy = fac*ry + 0.05*fac/ry;
				dx = fac*rx ;
				dy = fac*ry ;
			}else {
				dx = 0 ;
				dy = 0 ;
			}
			float temp =  1 - r;
			xnodes[to[i]-1] += 0.5 * dx *dt*dt/(r+temp);
			ynodes[to[i]-1] += 0.5 * dy *dt*dt/(r+temp);
			xnodes[from[i]-1] -= 0.5 * dx *dt*dt/(r+temp);
			ynodes[from[i]-1] -= 0.5 * dy *dt*dt/(r+temp);
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
			fac = -2e-1;
			float dx ;
			float dy ;
			if( r<0.8){
				dx = fac*rx/(r+0.01);
                                dy = fac*ry/(r+0.01);
				dx = fac*rx/(r*r*r);
                                dy = fac*ry/(r*r*r);
			}else {
				dx = 0 ;
				dy = 0 ;
			}
			xnodes[i] += 0.5 * dx *dt*dt;
			ynodes[i] += 0.5 * dy *dt*dt;

		}
		}
		}


		//making the links
		for(int i=0; i< num_links; i++) {
			DrawLine(scale*wwidth*xnodes[from[i]-1 ], scale*wheight*ynodes[from[i]-1 ],
				 scale*wwidth*xnodes[to[i]-1 ], scale*wheight*ynodes[to[i] -1],
				 GREEN);
		}


		// making one node
		for(int i=0; i< num_nodes; i++){
			DrawCircle(scale*wwidth*xnodes[i], scale*wheight*ynodes[i], 10, WHITE);
			char buf[10];
			sprintf(buf, "%d", i+1);
			DrawText(buf, scale*wwidth*xnodes[i], scale*wheight*ynodes[i], 20, RED);
		}
		char buf[80];
		float mouse = GetMouseWheelMove();
		scale += mouse*0.1;
		int flag, transx, transy;
		Vector2 move;
		flag = 0;
		if(IsMouseButtonDown(0)){
			flag = 1;
			move = GetMouseDelta();
		}
		for(int i=0; i<num_nodes; i++){
			xnodes[i] +=move.x/wwidth;
			ynodes[i] +=move.y/wheight;
		}


		sprintf(buf, "scale %f", scale);
		DrawText(buf, 10, 80, 20, WHITE);

		EndDrawing();
	}

	free(xnodes);
	free(ynodes);
	free(from);
	free(to);
EXIT:
	CloseWindow();
	return 0;

}

