/*
*  File: negativeColorPicker.cpp
*  Implements the negative color picker for CPSC 221 PA2.
*
*/

#include "negativeColorPicker.h"

NegativeColorPicker::NegativeColorPicker(PNG& inputimg)
{
img = inputimg;
  
}

HSLAPixel NegativeColorPicker::operator()(PixelPoint p)
{
  int px = p.x;
  int py = p.y;
  int imgx = img.width();
  int imgy = img.height();

  if(px >= imgx){
    px = px % imgx;
  }
  if(py >= imgy){
    py = py % imgy ;
  }

  HSLAPixel *pixel = img.getPixel(px,py);
  if(pixel->h + 180.0 > 360.0){
    pixel->h = pixel->h -= 180.0;
  } else{
    pixel->h = pixel->h += 180.0; 
  }

  pixel->l = 1.0 - pixel->l;
  HSLAPixel temp = HSLAPixel(pixel->h,pixel->s,pixel->l,pixel->a); 
  p.color = temp;
return temp; 
}
