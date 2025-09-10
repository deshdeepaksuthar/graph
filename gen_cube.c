// incomplete
#include <stdio.h>
#include <stdlib.h>

#define N 10
int main()
{
	int num_points = N*N;
	char buff[80];
	FILE *f = fopen("file.txt", "w");
	//making lines
	for(int i=1; i<= N; i++){
		for(int j=1; j<=N; j++){
		for(int k=1; k<N; k++){

			fprintf(f, "%d-%d\n", k+ (j-1)*N + (i-1)*N*N, k+1 + (j-1)*N+ (i-1)*N*N);
		}
		}
	}
	//making planes
	for(int i=1; i<=N; i++){
		for(int j=1; j<N; j++){
		for(int k=1; k<=N; k++){
			fprintf(f, "%d-%d\n", k+ (j-1)*N +(i-1)*N*N, k + (j)*N+(i-1)*N*N);
		}
		}
	}
	for(int i=1; i<N; i++){
		for(int j=1; j<=N; j++){
		for(int k=1; k<=N; k++){
			fprintf(f, "%d-%d\n", k+ (j-1)*N +(i-1)*N*N, k +N*N+ (j-1)*N+(i-1)*N*N);
		}
		}
	}
	// fprintf(f, "1-%d\n",  num_points);
	fclose(f);
	return 0;
}

