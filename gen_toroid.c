#include <stdio.h>
#include <stdlib.h>

#define L 5
#define H 10
int main()
{
	int num_points = L*H;
	char buff[80];
	FILE *f = fopen("file.txt", "w");
	for(int i=1; i<=L; i++){
		for(int j=1; j<H; j++){
			fprintf(f, "%d-%d\n", j+ (i-1)*H, j+1 + (i-1)*H);
		}
	}
	for(int i=1; i<L; i++){
		for(int j=1; j<=H; j++){
			fprintf(f, "%d-%d\n", j+ (i-1)*H, j +  (i)*H);
		}
	}
	for(int j=1; j<=H; j+=1){
		fprintf(f, "%d-%d\n", j, j + L*H - H);
	}
	for(int j=H; j<H*L; j+=H){
		fprintf(f, "%d-%d\n", j, j +1);
	}
	// fprintf(f, "1-%d\n",  num_points);
	fclose(f);
	return 0;
}

