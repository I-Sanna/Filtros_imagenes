#include <iostream>
#include <string>
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

void plain(ppm& img, unsigned char c)
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

void shades(ppm& img, unsigned char shades){
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

