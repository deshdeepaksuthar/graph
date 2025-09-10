#include <stdio.h>
#include <stdlib.h>

#define N 15
int main()
{
	int num_points = N*N;
	char buff[80];
	FILE *f = fopen("file.txt", "w");
	for(int i=1; i< N; i++){
		for(int j=1; j<=N; j++){
			fprintf(f, "%d-%d\n", i+ (j-1)*N, i+1 + (j-1)*N);
		}
	}
	for(int i=1; i<=N; i++){
		for(int j=1; j<N; j++){
			fprintf(f, "%d-%d\n", i+ (j-1)*N, i +N+ (j-1)*N);
		}
	}
	// fprintf(f, "1-%d\n",  num_points);
	fclose(f);
	return 0;
}

