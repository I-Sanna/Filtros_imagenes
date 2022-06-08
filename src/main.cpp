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
#define ONE_OVER_BILLION 1E-9


using namespace std;

// El siguiente es un template basico que pueden usar como base

vector<string> split(string str, char pattern) {
    
    int posInit = 0;
    int posFound = 0;
    string splitted;
    vector<string> results;
    
    while(posFound >= 0){
        posFound = str.find(pattern, posInit);
        splitted = str.substr(posInit, posFound - posInit);
        posInit = posFound + 1;
        results.push_back(splitted);
    }
    
    return results;
}

int main(int argc , char* argv[]){
	
	// Asumimos que los filtros sin p1 se escriben primero (por lo tanto, el primer p1 es no nulo)
	// Asumimos que Zoom no se puede encadenar
	if(string(argv[1]) == "-help"){
		cout << "Uso: ./main <filtro> <nthreads> <[p1]> <img1> <custom_output> <[p2]> <img2>" << endl;
	}
	else{
		string filter = string(argv[1]);
		unsigned int n = atoi(argv[2]);
		string p1L = argv[3];
		string img1(argv[4]);
		string out = string(argv[5]);
		string p2L;
		vector<string> p2S;
		vector<float> p2V;
		string img2 = "";
		if(argc > 6){
			p2L = argv[6];
			vector<string> p2S = split(p2L, ' ');
			for (int i = 0; i < p2S.size(); i++){
				p2V.push_back(stof(p2S[i]));
			}
		}
			
		if(argc > 7){
			img2 = string(argv[7]);
		}

		vector<string> filters = split(filter, ' ');
		vector<string> p1S = split(p1L, ' ');

		vector<float> p1V;
		for (int i = 0; i < p1S.size(); i++){
			p1V.push_back(stof(p1S[i]));
		}
		
		ppm img(img1);
		
		cout << "Aplicando filtros"<< endl;
		struct timespec start, stop;    	
		clock_gettime(CLOCK_REALTIME, &start);

		int k = 0;
		float p2 = 0;

		for(int i = 0; i < filters.size(); i++){

			filter = filters[i];
			float p1 = p1V[i];

			if (n == 0){			

				if (filter == "plain")
					if(p1 > 0)
						plain(img, p1, 0, img.width);
					else
						cout << "ingrese un numero mayor a 0" << endl;

				if (filter == "blackWhite")
					blackWhite(img, 0, img.width);

				if (filter == "shades")
					if(p1 > 1)
						shades(img, p1, 0, img.width);
					else
						cout << "ingrese un numero mayor a 1" << endl;

				if (filter == "merge")
					if(p1 >= 0 && p1 <= 1){
						ppm img3(img2);
						merge(img, img3, p1, 0, img.width);
					}
					else
						cout << "ingresar un numero entre 0 y 1 para el porcentaje";

				if (filter == "contrast"){
					if (p1 < -255)
						cout << "ingrese un valor entre -255 y 255" << endl;
					else if (p1 > 255)
						cout << "ingrese un valor entre -255 y 255" << endl;
					else
						contrast(img, p1, 0, img.width);
				}

				if (filter == "boxBlur")
					boxBlur(img, img, 0, img.width);

				if (filter == "edgeDetection")
					edgeDetection(img, img, 0, img.width);

				if (filter == "sharpen")
					sharpen(img, img, 0, img.width);

				if (filter == "brightness"){
					if (p1 <= 1 && p1 >= -1 )
						brightness(img, p1, 0, img.width);
					else
						cout << "ingrese un valor 1 o -1" << endl;
				}

				if (filter == "frame")
					if (p1 < 0)
						cout << "ingrese un valor entre 0 y 255" << endl;
					else if (p1 > 255)
						cout << "ingrese un valor entre 0 y 255" << endl;
					else{
						p2 = p2V[k];
						k++;
						frame(img, p1, p2, 0, img.width);
					}

				if (filter == "crop"){

					p2 = p2V[k];
					k++;
					
					ppm imgCopia(img.width - p2, img.height - p1);

					crop(img, imgCopia, p1 , p2, 0, img.width);
					
					img = imgCopia;
				}

				if (filter == "zoom")
					if (p1 <= 1)
						cout << "ingrese un valor mayor a 2" << endl;
					else{
						ppm img_zoomed(img.width * p1, img.height * p1);
						zoom(img, img_zoomed, p1, 0, img.width);
						img = img_zoomed;
					}

			}
			else{

				if (filter == "plain")
					if(p1 > 0)
						multiPlain(img, n, p1);
					else
						cout << "ingrese un numero mayor a 0" << endl;

				if (filter == "blackWhite")
					multiBlackWhite(img, n);

				if (filter == "shades")
					if(p1 > 1)
						multiShades(img, n, p1);
					else
						cout << "ingrese un numero mayor a 1" << endl;
				
				if (filter == "contrast"){
					if (p1 < -255)
						cout << "ingrese un valor entre -255 y 255" << endl;
					else if (p1 > 255)
						cout << "ingrese un valor entre -255 y 255" << endl;
					else
						multiContrast(img, n, p1);
				}

				if (filter == "brightness"){
					if (p1 <= 1 && p1 >= -1 )
						multiBrightness(img, n, p1);
					else
						cout << "ingrese un valor 1 o -1" << endl;
				}

				if (filter == "merge")
					if(p1 >= 0 && p1 <= 1){
						ppm img3(img2);
						multiMerge(img, img3, n, p1);
					}
					else
						cout << "ingresar un numero entre 0 y 1 para el porcentaje";

				if (filter == "boxBlur")
					multiBoxBlur(img, n);

				if (filter == "edgeDetection")
					multiEdgeDetection(img, n);

				if (filter == "sharpen")
					multiSharpen(img, n);

				if (filter == "zoom")
					if (p1 <= 1)
						cout << "ingrese un valor mayor a 2" << endl;
					else{
						multiZoom(img, n, p1);
					}
				
				if (filter == "frame")
					if (p1 < 0)
						cout << "ingrese un valor entre 0 y 255" << endl;
					else if (p1 > 255)
						cout << "ingrese un valor entre 0 y 255" << endl;
					else{
						p2 = p2V[k];
						k++;
						multiFrame(img, n, p1, p2);
					}

				if (filter == "crop"){
					p2 = p2V[k];
					k++;
					multiCrop(img, n, p1, p2);
				}

			}
		}

		clock_gettime(CLOCK_REALTIME, &stop);

		double accum;
		accum = ( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
		printf("%lf s\n", accum);

		cout << "Escribiendo imagen" << endl;
		img.write(out);	
				
		cout << "Listo" << endl;
		
	}
	return 0;
}