#include "tgaimage.h"
#include "components.h"


int main(int argc, char** argv) {
	TGAImage image(1920, 1080, TGAImage::RGB);

	Vec2i t0[3] = {Vec2i(960, 200), Vec2i(660, 360), Vec2i(1260, 360)};
	Vec2i t1[3] = {Vec2i(660, 360), Vec2i(960, 760), Vec2i(1260, 360)};
	Vec2i t2[3] = {Vec2i(660, 360), Vec2i(960, 760), Vec2i(540, 560)};
	Vec2i t3[3] = {Vec2i(1260, 360), Vec2i(960, 760), Vec2i(1380, 560)};
	Vec2i t4[3] = {Vec2i(540, 560), Vec2i(960, 760), Vec2i(660, 820)};
	Vec2i t5[3] = {Vec2i(1380, 560), Vec2i(960, 760), Vec2i(1260, 820)};
	Vec2i t6[3] = {Vec2i(660, 820), Vec2i(960, 760), Vec2i(840, 860)};
	Vec2i t7[3] = {Vec2i(1260, 820), Vec2i(960, 760), Vec2i(1080, 860)};

	triangle(t0[0], t0[1], t0[2], image, red);
	triangle(t1[0], t1[1], t1[2], image, red);
	triangle(t2[0], t2[1], t2[2], image, red);
	triangle(t3[0], t3[1], t3[2], image, red);
	triangle(t4[0], t4[1], t4[2], image, red);
	triangle(t5[0], t5[1], t5[2], image, red);
	triangle(t6[0], t6[1], t6[2], image, red);
	triangle(t7[0], t7[1], t7[2], image, red);


	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

