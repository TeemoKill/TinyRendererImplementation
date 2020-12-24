#include "tgaimage.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);

void line(Vec2i, Vec2i, TGAImage&, TGAColor);
void triangle(Vec2i, Vec2i, Vec2i, TGAImage&, TGAColor);
