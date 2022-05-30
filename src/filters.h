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

void blackWhite(ppm& img); 
void contrast(ppm& img, float contrast);  
void brightness(ppm& img, float b);
void shades(ppm& img, int shades); 
void merge(ppm& img1, ppm &img2, float alpha); 
void frame(ppm& img, int color, int x); 
void boxBlur(ppm& img); 
void zoom(ppm& img, ppm& img_zoomed, int n); //Santella
void edgeDetection(ppm& img); 
void plain(ppm& img, int c); 
void sharpen(ppm& img); 
void crop(ppm& img, int rows, int columns);

// MULTI-THREAD FILTERS

#endif