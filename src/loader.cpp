#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "filters.h"
#include <thread>
#include <unistd.h>
#include <fstream>      // std::ofstream
#include <filesystem>
#define ONE_OVER_BILLION 1E-9

using namespace std;

int main(int argc , char* argv[]){
    if(string(argv[1]) == "-help"){
		cout << "Uso: ./loader <filtro> <nthreads> <[ROOT_DIR]> <[p1]> <[p2]>" << endl;
	}
	else{
        string filter = string(argv[1]);
		unsigned int n = atoi(argv[2]);
		string DIR = argv[3];
		float p1;
		float p2;
		if (argc > 4){
			float p1 = atof(argv[4]);
		}
		if (argc > 5){
			float p2 = atof(argv[5]);
		}

        cout << "Aplicando filtros"<< endl;

		for (const auto & file : filesystem::directory_iterator(DIR)){

			cout << file.path() << " / " << n << endl;
			
			struct timespec start, stop;    	
			clock_gettime(CLOCK_REALTIME, &start);

			ppm img(file.path());

			if (n == 0){	

				if (filter == "plain")
					if(p1 > 0){
						plain(img, p1, 0, img.width);
					}
					else{
						cout << "ingrese un numero mayor a 0" << endl;
						break;
					}

				if (filter == "blackWhite")
					blackWhite(img, 0, img.width);

				if (filter == "shades")
					if(p1 > 1){
						shades(img, p1, 0, img.width);
					}
					else{
						cout << "ingrese un numero mayor a 1" << endl;
						break;
					}

				if (filter == "contrast"){
					if (p1 < -255){
						cout << "ingrese un valor entre -255 y 255" << endl;
						break;
					}
					else if (p1 > 255){
						cout << "ingrese un valor entre -255 y 255" << endl;
						break;
					}
					else{
						contrast(img, p1, 0, img.width);
					}
				}

				if (filter == "boxBlur"){
					boxBlur(img, img, 0, img.width);
				}

				if (filter == "edgeDetection"){
					edgeDetection(img, img, 0, img.width);
				}

				if (filter == "sharpen"){
					sharpen(img, img, 0, img.width);
				}

				if (filter == "brightness"){
					if (p1 <= 1 && p1 >= -1 ){
						brightness(img, p1, 0, img.width);
					}
					else{
						cout << "ingrese un valor 1 o -1" << endl;
						break;
					}
				}

				if (filter == "frame")
					if (p1 < 0){
						cout << "ingrese un valor entre 0 y 255" << endl;
						break;
					}
					else if (p1 > 255){
						cout << "ingrese un valor entre 0 y 255" << endl;
						break;
					}
					else{
						frame(img, p1, p2, 0, img.width);
					}

				if (filter == "crop"){
					ppm imgCopia(img.width - p2, img.height - p1);

					crop(img, imgCopia, p1 , p2, 0, img.width);
						
					img = imgCopia;
				}

				if (filter == "zoom")
					if (p1 <= 1){
						cout << "ingrese un valor mayor a 2" << endl;
						break;
					}
					else{
						ppm img_zoomed(img.width * p1, img.height * p1);

						zoom(img, img_zoomed, p1, 0, img.width);
							
						img = img_zoomed;
					}

			}
			else{

				if (filter == "plain")
					if(p1 > 0){
						multiPlain(img, n, p1);
					}							
					else{
						cout << "ingrese un numero mayor a 0" << endl;
					}

				if (filter == "blackWhite"){
					multiBlackWhite(img, n);
				}

				if (filter == "shades")
					if(p1 > 1){
						multiShades(img, n, p1);
					}
					else{
						cout << "ingrese un numero mayor a 1" << endl;
						break;
					}
					
				if (filter == "contrast"){
					if (p1 < -255){
						cout << "ingrese un valor entre -255 y 255" << endl;
						break;
					}
					else if (p1 > 255){
						cout << "ingrese un valor entre -255 y 255" << endl;
						break;
					}
					else{
						multiContrast(img, n, p1);
					}							
				}

				if (filter == "brightness"){
					if (p1 <= 1 && p1 >= -1 ){
						multiBrightness(img, n, p1);
					}							
					else{
						cout << "ingrese un valor 1 o -1" << endl;
						break;
					}
				}

				if (filter == "boxBlur"){
					multiBoxBlur(img, n);
				}						

				if (filter == "edgeDetection"){
					multiEdgeDetection(img, n);
				}						

				if (filter == "sharpen"){
					multiSharpen(img, n);
				}						

				if (filter == "zoom")
					if (p1 <= 1){
						cout << "ingrese un valor mayor a 2" << endl;
						break;
					}
					else{
						multiZoom(img, n, p1);
					}
					
				if (filter == "frame")
					if (p1 < 0){
						cout << "ingrese un valor entre 0 y 255" << endl;
						break;
					}							
					else if (p1 > 255){
						cout << "ingrese un valor entre 0 y 255" << endl;
						break;
					}							
					else{
						multiFrame(img, n, p1, p2);
					}

				if (filter == "crop"){
					multiCrop(img, n, p1, p2);
				}
		
			}

			clock_gettime(CLOCK_REALTIME, &stop);

			double accum;
			accum = ( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
			printf("%lf s\n", accum);

			cout << "Escribiendo imagen" << endl;
			img.write(file.path());	
		}
		cout << "Listo" << endl;
	}
	return 0;
}