#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <cstdlib>
#define NN 10000

int A[NN][NN], B[NN][NN];
long long C[NN][NN];

void solve(int n1, int n2, int n3, int num_thread)
{
	int i, j, t, k;
	double paralleltime, serialtime;
	clock_t startTime, endTime;
	long long sum;

	omp_set_num_threads(num_thread);
	//--------------init matrix A and B------------
	for (i = 0; i < n1; i++)
	{
		t = i + 1;
		for (j = 0; j < n2; j++)
		{
			A[i][j] = t++;

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
	startTime = clock();
	sum = 0;
#pragma omp parallel shared(A,B,C) private(i,j,k)
	{
#pragma omp for //schedule(dynamic)
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

	for (i = 0; i < n1; i++)
		for (j = 0; j < n3; j++)
			sum += C[i][j];

	endTime = clock();

	paralleltime = endTime - startTime;

	printf("sum=%lld time=%.4fms  Threads = %d\n", sum, paralleltime, num_thread);
	//--------------------------------------------------------
	//--------------------------------------------------------
	startTime = clock();
	sum = 0;
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
	for (i = 0; i < n1; i++)
		for (j = 0; j < n3; j++)
			sum += C[i][j];
	endTime = clock();
	serialtime = endTime - startTime;
	printf("C elements sum=%lld time=%dms\n", sum, serialtime);
	printf("speedUp： %d/%d\n", serialtime, paralleltime);
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
	n2 = 1000;
	n3 = 1000;
	num_thread = 5;
	printf("%d\n", argc);
	if (argc >= 4) {
		n1 = atoi(argv[1]);
		n2 = atoi(argv[2]);
		n3 = atoi(argv[3]);
		num_thread = atoi(argv[4]);
	}

	num_thread = (int)argv[4];
	solve(n1, n2, n3, num_thread);
	return 0;
}

