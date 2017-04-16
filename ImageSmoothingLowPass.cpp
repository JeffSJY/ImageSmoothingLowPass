#include "stdafx.h"
#include <stdio.h>
#include <omp.h>
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning (disable : 4996)
#include <iostream>

int p[1940][3000];
int mainList[1940][3000];
int width=0, height=0;
int magic=0;
void loadImage(char[]);
void saveImage(char[]);
void smooth();

int main()
{
	omp_set_num_threads(8);
	auto start = omp_get_wtime();
	loadImage("kot.pgm");
	for (int k = 0; k < 100; k++) {
	smooth();}
	saveImage("kot3.pgm");
	auto koniec = omp_get_wtime();
	printf(" Zajelo %f sekund.\n", koniec - start);
	return 0;
}

void loadImage(char filename[])
{
	{
	FILE *f;
	char liniaWczytana[4000];
	//int x, y;
	f = fopen(filename, "r");
	
	fscanf(f, "%[^\n]\n", liniaWczytana);
	fscanf(f, "%d %d\n", &width, &height);
	fscanf(f, "%d\n", &magic);
	for (int y = 0; y < height; y++)
	{

		for (int x = 0; x < width; x++)
		{
			fscanf(f, "%d", &p[y][x]);
		}
	}
	fclose(f);}}

void smooth()
{
#pragma omp parallel
	{

		int i = 0, j = 0;
#pragma omp for
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				mainList[i][j] = p[i][j];
			}
		}
#pragma omp for
		for (int i = 1; i < height; i++) {
			for (int j = 1; j < width; j++) {
				/// dla wag 1 i srodek 1
				float wartoscNowa3x3 = p[i - 1][j - 1] + p[i - 1][j] +
					p[i - 1][j + 1] + p[i][j - 1] + p[i][j] + p[i][j + 1] +
					p[i + 1][j - 1] + p[i + 1][j] + p[i + 1][j];
				//dla wag 1 i srodek 2;
				float wartoscNowa3x3_waga3 = p[i - 1][j - 1] + p[i - 1][j] +
					p[i - 1][j + 1] + p[i][j - 1] + 3 * p[i][j] + p[i][j + 1] +
					p[i + 1][j - 1] + p[i + 1][j] + p[i + 1][j];
				mainList[i][j] = (int)(wartoscNowa3x3_waga3 / 11);
			}
		}
memcpy(&p, &mainList, sizeof(mainList)); 
	}
}


void saveImage(char filename[])
{
	{
		FILE *f;
		f = fopen(filename, "w");

		// dane naglowkowe (jak przy otwarciu)
		fprintf(f, "P2\n");
		fprintf(f, "%d %d\n", width, height);
		fprintf(f, "%d\n", magic);

		for (int y = 0; y < height; y++)
		{

			for (int x = 0; x < width; x++)
			{
				fprintf(f, "%d ", mainList[y][x]);
			}
			fprintf(f, "\n");
		}
		fclose(f);
	}
}