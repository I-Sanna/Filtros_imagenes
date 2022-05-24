#include <iostream>
#include <string>
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

int main(int argc , char* argv[]){
	
	// Asumimos que los filtros sin p1 se escriben primero (por lo tanto, el primer p1 es no nulo)
	// Asumimos que Zoom no se puede encadenar

	if(string(argv[1]) == "-help"){
		cout << "Uso: ./main <filtro> <nthreads> <[p1]> <img1> <custom_output> <[p2]> <img2>" << endl;
		return 0; 
	}
	else{

		string filter = string(argv[1]);
		unsigned int n = atoi(argv[2]);
		float p1 = atof(argv[3]);
		string img1(argv[4]);
		string out = string(argv[5]);
		float p2 = atof(argv[6]);
		string img2(argv[7]);
		
		ppm img(img1);
		ppm img3;
		try{
			ppm img3(img2);
		}
		catch(exception ex){
			cout << "";
		}
		
		cout << "Aplicando filtros"<< endl;
		struct timespec start, stop;    	
		clock_gettime(CLOCK_REALTIME, &start);

		if (filter == "plain")
			if(p1 > 0)
				plain(img, (unsigned char)p1);
			else
				cout << "ingrese un numero mayor a 0" << endl;
		if (filter == "blackWhite")
			blackWhite(img);
		if (filter == "shades")
			if(p1 > 0)
				shades(img, (unsigned char)p1);
			else
				cout << "ingrese un numero mayor a 0" << endl;
		if (filter == "brightness")
			brightness(img, p1);
		if (filter == "contrast")
			contrast(img, p1);
		if (filter == "merge")
			merge(img, img3, p1);
		if (filter == "boxBlur")
			boxBlur(img);
		if (filter == "edgeDetection")
			edgeDetection(img, img3);
		if (filter == "sharpen")
			sharpen(img);
		if (filter == "crop")
			crop(img, p1, p2);
		if (filter == "frame")
			frame(img, p1, p2);
		if (filter == "zoom")
			zoom(img, img3, p1);


		clock_gettime(CLOCK_REALTIME, &stop);

		double accum;
		accum = ( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
		printf("%lf s\n", accum);

		cout << "Escribiendo imagen" << endl;
		img.write(out);	
			
		cout << "Listo" << endl;
		return 0;

	}
}