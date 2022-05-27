#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <math.h>       /* sqrt */
#include <vector>
#include "filters.h"
#include <thread>  
#include <atomic>  

#define BLACK 0

using namespace std;

// COMPLETAR :)

// Filtro plano como ejemplo

void plain(ppm& img, int c)
{

	for(int i = 0; i < img.height; i++)
		for(int j = 0; j < img.width; j++)		
			img.setPixel(i, j, pixel(c,c,c));

}

void blackWhite(ppm& img){

	for(int i = 0; i < img.height; i++)
		for(int j = 0; j < img.width; j++){

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

void shades(ppm& img, int shades){
	for(int i = 0; i < img.height; i++)
		for(int j = 0; j < img.width; j++){

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

void contrast(ppm& img, float c){
	for(int i = 0; i < img.height; i++)
		for(int j = 0; j < img.width; j++){

			int R = img.getPixel(i, j).r;
			int G =	img.getPixel(i, j).g;
			int B = img.getPixel(i, j).b;

			int contraste = (259 * (c + 255))/(255 * (c + 259));

			R = contraste * (R - 128) + 128;
			G = contraste * (G - 128) + 128;
			B = contraste * (B - 128) + 128;

			img.setPixel(i, j, pixel(R, G, B));
		}	
}
void merge(ppm& img1, ppm& img2, float perc1){

	float perc2 = 1 - perc1;

	for(int i = 0; i < img1.height; i++)
		for(int j = 0; j < img1.width; j++){

			int R1 = img1.getPixel(i, j).r;
			int G1 = img1.getPixel(i, j).g;
			int B1 = img1.getPixel(i, j).b;

			int R2 = img2.getPixel(i, j).r;
			int G2 = img2.getPixel(i, j).g;
			int B2 = img2.getPixel(i, j).b;

			float RT = floor(R1 * perc1 + R2 * perc2);
			float GT = floor(G1 * perc1 + G2 * perc2);
			float BT = floor(B1 * perc1 + B2 * perc2);

			if(RT > 255)
				RT = 255;
			if(GT > 255)
				GT = 255;
			if(BT > 255)
				BT = 255;

			img1.setPixel(i, j, pixel(RT, GT, BT));
		}
}

