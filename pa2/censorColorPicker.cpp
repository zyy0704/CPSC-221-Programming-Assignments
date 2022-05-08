/*
*  File: censorColorPicker.cpp
*  Implements the censor color picker for CPSC 221 PA2.
*
*/

#include "censorColorPicker.h"

#include <math.h> // gives access to sqrt function

/*
*  Useful function for computing the Euclidean distance between two PixelPoints
*/
double PointDistance(PixelPoint a, PixelPoint b) {
  unsigned int diff_x, diff_y;
  if (a.x > b.x)
    diff_x = a.x - b.x;
  else
    diff_x = b.x - a.x;
  if (a.y > b.y)
    diff_y = a.y - b.y;
  else
    diff_y = b.y - a.y;
  return sqrt(diff_x * diff_x + diff_y * diff_y);
}

CensorColorPicker::CensorColorPicker(unsigned int b_width, PixelPoint ctr, unsigned int rad, PNG& inputimage)
{
blockwidth = b_width;
center = ctr;
radius = rad;
img = inputimage; 
}

HSLAPixel CensorColorPicker::operator()(PixelPoint p)
{
unsigned int px = p.x;
unsigned int py= p.y;
double avgH = 0;
double avgS = 0;
double avgL = 0;
double avgA = 0;

if(PointDistance(center, p) > radius){ // if p is outside active circle
return *img.getPixel(px,py); // return original image pixel
} else {
  //cout<<"BlockWidth: "<<blockwidth<<endl;
  while(fmod(px,blockwidth)!= 0){ 
    px--; //decrement x-coord until it is a multiple of blockwidth
  }
  //cout<<"X-coord: "<<px<<endl;
  while(fmod(py,blockwidth)!= 0){
    py--; //decrement y-coord until it is a multiple of blockwidth
  }
  //cout<<"Y-coord: "<<py<<endl;
  HSLAPixel square = *img.getPixel(px,py);

 int xPixs = img.width() - px; 
 int yPixs = img.height() - py;

  if(img.width() - px < blockwidth && img.height() - py < blockwidth){ // x and y coord edge case
  int denom = 0;
      for(unsigned int x = px; x < px + xPixs; x++){ // 
      for(unsigned int y = py; y < py + yPixs; y++){
      HSLAPixel pixel = *img.getPixel(x,y);
      avgH += pixel.h;
      avgS += pixel.s;
      avgL += pixel.l;
      avgA += pixel.a;
      denom++;
    }
  }
  square.h = avgH/denom; // set averages of the HSLA vals
  square.s = avgS/denom;
  square.l = avgL/denom;
  square.a = avgA/denom;
  return square; 
    }


  if(img.width() - px < blockwidth && img.height() - py >= blockwidth){ //only x coord edge case
  int denom = 0;
    for(unsigned int x = px; x < px + xPixs; x++){
      for(unsigned int y = py; y < py + blockwidth; y++){
      HSLAPixel pixel = *img.getPixel(x,y);
      avgH += pixel.h;
      avgS += pixel.s;
      avgL += pixel.l;
      avgA += pixel.a;
      denom++;
    }
  }
  square.h = avgH/denom; // set averages of the HSLA vals
  square.s = avgS/denom;
  square.l = avgL/denom;
  square.a = avgA/denom;
  return square; 
    }

if(img.width() - px >= blockwidth && img.height() - py < blockwidth){ // only y-coord edge case
int denom = 0;
    for(unsigned int x = px; x < px + blockwidth; x++){ 
      for(unsigned int y = py; y < py + yPixs; y++){
      HSLAPixel pixel = *img.getPixel(x,y);
      avgH += pixel.h;
      avgS += pixel.s;
      avgL += pixel.l;
      avgA += pixel.a;
      denom++;
    }
  }
  square.h = avgH/denom; // set averages of the HSLA vals
  square.s = avgS/denom;
  square.l = avgL/denom;
  square.a = avgA/denom;
  return square; 
    }

if(img.width() - px >= blockwidth && img.height() - py>= blockwidth){ // x and y coord both NOT edge cases
int denom = 0;
  for(unsigned int x = px; x < px + blockwidth; x++){ // 
    for(unsigned int y = py; y < py + blockwidth; y++){
      HSLAPixel pixel = *img.getPixel(x,y);
      avgH += pixel.h;
      avgS += pixel.s;
      avgL += pixel.l;
      avgA += pixel.a;
      denom++;
    }
  }
  square.h = avgH/denom; // set averages of the HSLA vals
  square.s = avgS/denom;
  square.l = avgL/denom;
  square.a = avgA/denom;
  return square; 

}
}


}