#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <math.h>       /* sqrt */
#include <vector>
#include "filters.h"
#include <atomic>  
#include <thread>

#define BLACK 0

using namespace std;

// COMPLETAR :)

// Filtro plano como ejemplo

void plain(ppm& img, int c, int start, int end)
{

	for(int i = 0; i < img.height; i++)
		for(int j = start; j < end; j++)		
			img.setPixel(i, j, pixel(c,c,c));

}

void blackWhite(ppm& img, int start, int end){

	for(int i = 0; i < img.height; i++)
		for(int j = start; j < end; j++){

			int R = img.getPixel(i, j).r;
			int G =	img.getPixel(i, j).g;
			int B = img.getPixel(i, j).b;

			int BW = (R + G + B)/3;

			if (BW > 255){
				BW = 255;
			}

			img.setPixel(i, j, pixel(BW, BW, BW));
		}			

}

void shades(ppm& img, int shades, int start, int end){
	for(int i = 0; i < img.height; i++)
		for(int j = start; j < end; j++){

			int R = img.getPixel(i, j).r;
			int G =	img.getPixel(i, j).g;
			int B = img.getPixel(i, j).b;

			int BW = (R + G + B)/3;

			if (BW > 255){
				BW = 255;
			}

			int rango = (255 - 255 % (int(shades) - 1))/(int(shades) - 1);

			int resultado = (BW - BW % rango)/rango * rango;

			img.setPixel(i, j, pixel(resultado, resultado, resultado));
		}	
}

void contrast(ppm& img, int c, int start, int end){

	float contraste = (259 * (c + 255))/(255 * (259 - c));

	for(int i = 0; i < img.height; i++)
		for(int j = start; j < end; j++){

			int R = img.getPixel(i, j).r;
			int G =	img.getPixel(i, j).g;
			int B = img.getPixel(i, j).b;

			R = floor(contraste * (R - 128) + 128);
			G = floor(contraste * (G - 128) + 128);
			B = floor(contraste * (B - 128) + 128);

			if(R > 255)
				R = 255;
			if(G > 255)
				G = 255;
			if(B > 255)
				B = 255;

			img.setPixel(i, j, pixel(R, G, B));
		}	
}

void brightness(ppm& img, float b, int start, int end){

	for(int i = 0; i < img.height; i++)
		for(int j = start; j < end; j++){

			int R = img.getPixel(i, j).r;
			int G =	img.getPixel(i, j).g;
			int B = img.getPixel(i, j).b;

			R = floor(R + 255 * b);
			G = floor(G + 255 * b);
			B = floor(B + 255 * b);

			if(R > 255)
				R = 255;
			if(G > 255)
				G = 255;
			if(B > 255)
				B = 255;

			if(R < 0)
				R = 0;
			if(G < 0)
				G = 0;
			if(B < 0)
				B = 0;

			img.setPixel(i, j, pixel(R, G, B));
		}	
}

void merge(ppm& img1, ppm& img2, float perc1, int start, int end){

	float perc2 = 1 - perc1;

	for(int i = 0; i < img1.height; i++)
		for(int j = start; j < end; j++){

			int R1 = img1.getPixel(i, j).r;
			int G1 = img1.getPixel(i, j).g;
			int B1 = img1.getPixel(i, j).b;

			int R2 = img2.getPixel(i, j).r;
			int G2 = img2.getPixel(i, j).g;
			int B2 = img2.getPixel(i, j).b;

			int RT = floor(R1 * perc1 + R2 * perc2);
			int GT = floor(G1 * perc1 + G2 * perc2);
			int BT = floor(B1 * perc1 + B2 * perc2);

			if(RT > 255)
				RT = 255;
			if(GT > 255)
				GT = 255;
			if(BT > 255)
				BT = 255;

			img1.setPixel(i, j, pixel(RT, GT, BT));
		}
}

void boxBlur(ppm& img){

	ppm img2 = img;

	for(int i = 0; i < img.height; i++){

		if (i == 0 || i == img.height - 1)
			continue;

		for(int j = 0; j < img.width; j++){

			if(j == 0 || j == img.width - 1)
				continue;

			vector<int> columna1 = {1, 1, 1};
			vector<int> columna2 = {1, 1, 1};
			vector<int> columna3 = {1, 1, 1};
			vector<vector<int>> matriz = {columna1, columna2, columna3};	

			int RT = 0;
			int GT = 0;
			int BT = 0;

			int R = 0;
			int G = 0;
			int B = 0;

			int x = 0;
			int y = 0;

			for (int height = -1; height <= 1; height++){

				for (int width = -1; width <= 1; width++){
					R = img2.getPixel(i + height, j + width).r;
					G =	img2.getPixel(i + height, j + width).g;
					B = img2.getPixel(i + height, j + width).b;

					RT += R * matriz[x][y];
					GT += G * matriz[x][y];
					BT += B * matriz[x][y];

					x++;
				}
				x = 0;
				y++;
			}

			RT = floor(RT / 9);
			GT = floor(GT / 9);
			BT = floor(BT / 9);

			if(RT > 255)
				RT = 255;
			if(GT > 255)
				GT = 255;
			if(BT > 255)
				BT = 255;

			img.setPixel(i, j, pixel(RT, GT, BT));
		}
	}
}

void edgeDetection(ppm& img){

	blackWhite(img, 0, img.width);
	boxBlur(img);

	ppm imgCopia = img;

	for(int i = 0; i < img.height; i++){

		if (i == 0 || i == img.height - 1)
			continue;

		for(int j = 0; j < img.width; j++){

			if(j == 0 || j == img.width - 1)
				continue;

			vector<int> columnaV1 = {1, 2, 1};
			vector<int> columnaV2 = {0, 0, 0};
			vector<int> columnaV3 = {-1, -2, -1};
			vector<vector<int>> matrizV = {columnaV1, columnaV2, columnaV3};	

			vector<int> columnaH1 = {1, 0, -1};
			vector<int> columnaH2 = {2, 0, -2};
			vector<int> columnaH3 = {1, 0, -1};
			vector<vector<int>> matrizH = {columnaH1, columnaH2, columnaH3};	

			int BW = 0;

			int BWV = 0;

			int BWH = 0;

			int x = 0;
			int y = 0;

			for (int height = -1; height <= 1; height++){

				for (int width = -1; width <= 1; width++){

					BW = imgCopia.getPixel(i + height, j + width).r;

					BWV += BW * matrizV[x][y];

					BWH += BW * matrizH[x][y];

					x++;
				}
				x = 0;
				y++;
			}

			if(BWV > 255)
				BWV = 255;

			if(BWV < 0)
				BWV = 0;

			if(BWH > 255)
				BWH = 255;

			if(BWH < 0)
				BWH = 0;

			int BWT = floor(sqrt(pow(BWH, 2) + pow(BWV, 2)));

			img.setPixel(i, j, pixel(BWT, BWT, BWT));
		}
	}
}

void sharpen(ppm& img){

	ppm img2 = img;

	for(int i = 0; i < img.height; i++){

		if (i == 0 || i == img.height - 1)
			continue;

		for(int j = 0; j < img.width; j++){

			if(j == 0 || j == img.width - 1)
				continue;

			vector<int> columna1 = {0, -1, 0};
			vector<int> columna2 = {-1, 5, -1};
			vector<int> columna3 = {0, -1, 0};
			vector<vector<int>> matriz = {columna1, columna2, columna3};	

			int RT = 0;
			int GT = 0;
			int BT = 0;

			int R = 0;
			int G = 0;
			int B = 0;

			int x = 0;
			int y = 0;

			for (int height = -1; height <= 1; height++){

				for (int width = -1; width <= 1; width++){
					R = img2.getPixel(i + height, j + width).r;
					G =	img2.getPixel(i + height, j + width).g;
					B = img2.getPixel(i + height, j + width).b;

					RT += R * matriz[x][y];
					GT += G * matriz[x][y];
					BT += B * matriz[x][y];

					x++;
				}
				x = 0;
				y++;
			}

			if(RT > 255)
				RT = 255;
			if(GT > 255)
				GT = 255;
			if(BT > 255)
				BT = 255;

			if(RT < 0)
				RT = 0;
			if(GT < 0)
				GT = 0;
			if(BT < 0)
				BT = 0;

			img.setPixel(i, j, pixel(RT, GT, BT));
		}
	}
}

void frame(ppm& img, int color, int x){

	for(int i = 0; i < img.height; i++)
		for(int j = 0; j < img.width; j++){

			if (i < x || j < x || i > img.height - x || j > img.width - x)
				img.setPixel(i, j, pixel(color, color, color));
		}	
}

void crop(ppm& img, int rows, int columns){
	ppm img2(img.width - columns, img.height - rows);
	
	for(int i = rows; i < img.height; i++)
		for(int j = columns; j < img.width; j++){
			img2.setPixel(i - rows, j - columns, img.getPixel(i,j));
		}	
	img = img2;
}

void zoom(ppm& img, int zoom){
	ppm img_zoomed(img.width * zoom, img.height * zoom);

	for(int i = 0; i < img.height; i++)
		for(int j = 0; j < img.width; j++)

			for(int x = 0; x < zoom; x++)
				for (int y = 0; y < zoom; y++)
					img_zoomed.setPixel((i * zoom) + x, (j * zoom) + y, img.getPixel(i,j));

	img = img_zoomed;
}

// MULTI-THREAD FUNCTIONS

void multiPlain(ppm& img, int threads, int c){

	int average = (img.width - (img.width % threads + 1)) / threads + 1;

	vector<thread> ths;

	for (int i = 0; i < threads; i++){

		ths.push_back(thread(plain, ref(img), c, average * i, average * (i + 1)));
	}

	plain(img, c, average * threads, img.width);

	for (int i = 0; i < ths.size(); i++){
		ths[i].join();
	}
}

void multiBlackWhite(ppm& img, int threads){

	int average = (img.width - (img.width % threads + 1)) / (threads + 1);

	vector<thread> ths;

	for (int i = 0; i < threads; i++){

		ths.push_back(thread(blackWhite, ref(img), average * i, average * (i + 1)));
	}

	blackWhite(img, average * threads, img.width);

	for (int i = 0; i < ths.size(); i++){
		ths[i].join();
	}
}

void multiShades(ppm& img, int threads, int shadesNr){

	int average = (img.width - (img.width % threads + 1)) / (threads + 1);

	vector<thread> ths;

	for (int i = 0; i < threads; i++){

		ths.push_back(thread(shades, ref(img), shadesNr, average * i, average * (i + 1)));
	}

	shades(img, shadesNr, average * threads, img.width);

	for (int i = 0; i < ths.size(); i++){
		ths[i].join();
	}
}

void multiContrast(ppm& img, int threads, int contr){

	int average = (img.width - (img.width % threads + 1)) / (threads + 1);

	vector<thread> ths;

	for (int i = 0; i < threads; i++){

		ths.push_back(thread(contrast, ref(img), contr, average * i, average * (i + 1)));
	}

	contrast(img, contr, average * threads, img.width);

	for (int i = 0; i < ths.size(); i++){
		ths[i].join();
	}
}

void multiBrightness(ppm& img, int threads, float percentage){

	int average = (img.width - (img.width % threads + 1)) / (threads + 1);

	vector<thread> ths;

	for (int i = 0; i < threads; i++){

		ths.push_back(thread(brightness, ref(img), percentage, average * i, average * (i + 1)));
	}

	brightness(img, percentage, average * threads, img.width);

	for (int i = 0; i < ths.size(); i++){
		ths[i].join();
	}
}

void multiMerge(ppm& img, ppm& img2, int threads, float percentage){

	int average = (img.width - (img.width % threads + 1)) / (threads + 1);

	vector<thread> ths;

	for (int i = 0; i < threads; i++){

		ths.push_back(thread(merge, ref(img), ref(img2), percentage, average * i, average * (i + 1)));
	}

	merge(img, img2, percentage, average * threads, img.width);

	for (int i = 0; i < ths.size(); i++){
		ths[i].join();
	}
}