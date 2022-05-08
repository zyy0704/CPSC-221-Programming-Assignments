/*
*  File: imageTileColorPicker.cpp
*  Implements the image tile color picker for CPSC 221 PA2.
*
*/

#include "imageTileColorPicker.h"

ImageTileColorPicker::ImageTileColorPicker(PNG& otherimage) {
  img_other = otherimage;
  
}

HSLAPixel ImageTileColorPicker::operator()(PixelPoint p) {
  int px = p.x;
  int py = p.y;
  int imgx = img_other.width();
  int imgy = img_other.height();

if(px >= imgx){
    px = px % imgx;
  }
  if(py >= imgy){
    py = py % imgy ;
  }

  HSLAPixel pixel = *img_other.getPixel(px,py);
return pixel; 
}