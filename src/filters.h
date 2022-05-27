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

void blackWhite(ppm& img); //Sanna Finalizado
void contrast(ppm& img, float contrast); //Santella
void brightness(ppm& img, float b); //Santella
void shades(ppm& img, int shades); //Sanna Finalizado
void merge(ppm& img1, ppm &img2, float alpha); //Sanna Finalizado
void frame(ppm& img, pixel color, int x); //Santella
void boxBlur(ppm& img); //Sanna
void zoom(ppm& img, ppm& img_zoomed, int n); //Santella
void edgeDetection(ppm& img, ppm& img_target); //Sanna
void plain(ppm& img, int c); //Sanna
void sharpen(ppm& img); //Sanna
void crop(ppm& img, float rows, float columns); //Santella

// MULTI-THREAD FILTERS

#endif