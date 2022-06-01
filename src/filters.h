#ifndef FILTERS_H
#define FILTERS_H

#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include "ppm.h"
#include <atomic>


// SINGLE-THREAD FILTERS

void blackWhite(ppm& img, int start, int end); 
void contrast(ppm& img, int contrast, int start, int end);  
void brightness(ppm& img, float b, int start, int end);
void shades(ppm& img, int shades, int start, int end); 
void merge(ppm& img1, ppm &img2, float alpha); 
void frame(ppm& img, int color, int x); 
void boxBlur(ppm& img); 
void zoom(ppm& img, int n); 
void edgeDetection(ppm& img); 
void plain(ppm& img, int c, int start, int end); 
void sharpen(ppm& img); 
void crop(ppm& img, int rows, int columns);

// MULTI-THREAD FILTERS

void multiPlain(ppm& img, int threads, int c);
void multiBlackWhite(ppm& img, int threads);
void multiShades(ppm& img, int threads, int shadesNr);
void multiContrast(ppm& img, int threads, int contr);
void multiBrightness(ppm& img, int threads, float percentage);

#endif