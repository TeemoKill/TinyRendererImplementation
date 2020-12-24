#include "tgaimage.h"
#include "geometry.h"

const TGAColor white   = TGAColor(255, 255, 255, 255);
const TGAColor black   = TGAColor(0,   0,   0,   255);
const TGAColor red     = TGAColor(255, 0,   0,   255);
const TGAColor orange  = TGAColor(255, 127, 0,   255);
const TGAColor yellow  = TGAColor(255, 255, 0,   255);
const TGAColor green   = TGAColor(0,   255, 0,   255);
const TGAColor cyan    = TGAColor(0,   255, 255, 255);
const TGAColor blue    = TGAColor(0,   0,   255, 255);
const TGAColor violet  = TGAColor(255, 0,   255, 255);

void line(Vec2i, Vec2i, TGAImage&, TGAColor);
void triangle(Vec2i, Vec2i, Vec2i, TGAImage&, TGAColor);
