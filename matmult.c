#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <cstdlib>
#define NN 10000

float A[NN][NN], B[NN][NN];
long long C[NN][NN];

void solve(int n1, int n2, int n3, int num_thread)
{
	int i, j, t, k;
	struct timespec start, end;
	long long sum;

	omp_set_num_threads(num_thread);
	//--------------init matrix A, B, and C------------
	for (i = 0; i < n1; i++)
	{
		for (j = 0; j < n2; j++)
		{
			A[i][j] = (float)rand() / (RAND_MAX);

		}
	}

	for (i = 0; i < n2; i++)
	{
		for (j = 0; j < n3; j++)
		{
			B[i][j] = 1;
		}
	}

	for (i = 0; i < n1; i++)
	{
		for (j = 0; j < n3; j++)
		{
			C[i][j] = 0;
		}
	}

	//----------------parallel------------------
	//startTime = clock();
	clock_gettime(CLOCK_MONOTONIC, &start);
	sum = 0;
#pragma omp parallel shared(A,B,C) private(i,j,k)
	{
#pragma omp for schedule(static)
		for (i = 0; i < n1; i++)
		{
			for (j = 0; j < n3; j++)
			{
				C[i][j] = 0;
				for (k = 0; k < n2; k++)
				{
					C[i][j] += A[i][k] * B[k][j];
				}
			}
		}
	}
	/*
	for (i = 0; i < n1; i++)
		for (j = 0; j < n3; j++)
			sum += C[i][j];
	*/
	
	clock_gettime(CLOCK_MONOTONIC, &end);

	if (end.tv_nsec < start.tv_nsec) {
		end.tv_sec -= 1;
		end.tv_nsec += 1000000000;
	}
	printf("%lld.%09ld secs\n", (long long)end.tv_sec - start.tv_sec, end.tv_nsec - start.tv_nsec);
	//endTime = clock();

	//paralleltime = endTime - startTime;

	//printf("sum=%lld time=%.4fms  Threads = %d\n", sum, paralleltime, num_thread);
	//--------------------------------------------------------
	//--------------------------------------------------------
	/*
	startTime = clock();
	sum = 0;
	for (i = 0; i < n1; i++)
	{
		for (j = 0; j < n3; j++)
		{
			for (k = 0; k < n2; k++)
			{
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	for (i = 0; i < n1; i++)
		for (j = 0; j < n3; j++)
			sum += C[i][j];
	endTime = clock();
	serialtime = endTime - startTime;
	printf("C elements sum=%lld time=%dms\n", sum, serialtime);
	printf("speedUp： %d/%d\n", serialtime, paralleltime);
	*/
	//----------------------------------------------------
}


int main(int argc, char* argv[])
{
	int n1, n2, n3, num_thread;
	/*
	printf("------------------  ------------------\n");
	printf("please enter （N<=2000），thread。\n\n");

	while (scanf_s("%d%d%d%d", &n1, &n2, &n3, &num_thread) != EOF)
	{
		int threads[10] = { 1, 2, 4, 8, 12, 14, 16, 20, 24, 28 };

		printf("n1：%d n2：%d n3：%d\n", n1, n2, n3);

		solve(n1, n2, n3, num_thread);
	}
	*/
	n1 = 1000;
	n2 = 2000;
	n3 = 5000;
	num_thread = 5;

	if (argc >= 4) {
		n1 = atoi(argv[1]);
		n2 = atoi(argv[2]);
		n3 = atoi(argv[3]);
		num_thread = atoi(argv[4]);
	}
	printf("***********");
	printf("%d   %d   %d    %d", n1, n2, n3, num_thread);
	printf("***********\n");
	solve(n1, n2, n3, num_thread);
	return 0;
}

