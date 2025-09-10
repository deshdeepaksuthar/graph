#include <stdio.h>
#include <stdlib.h>

#define L 20
#define H 10
int main()
{
	int num_points = L*H;
	char buff[80];
	FILE *f = fopen("file.txt", "w");
	for(int i=1; i<L; i++){
		for(int j=1; j<=H; j++){
			fprintf(f, "%d-%d\n", i+ (j-1)*L, i+1 + (j-1)*L);
		}
	}
	for(int i=1; i<=L; i++){
		for(int j=1; j<H; j++){
			fprintf(f, "%d-%d\n", i+ (j-1)*L, i + L + (j-1)*L);
		}
		fprintf(f, "%d-%d\n", i ,  L*H +i -L  );
	}
	// fprintf(f, "1-%d\n",  num_points);
	fclose(f);
	return 0;
}

