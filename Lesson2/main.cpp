#include "tgaimage.h"
#include "components.h"


int main(int argc, char** argv) {
	TGAImage image(1920, 1080, TGAImage::RGB);

	Vec2i t0[3] = {Vec2i(10, 70), Vec2i(50, 160), Vec2i(70, 80)};
	Vec2i t1[3] = {Vec2i(560, 780), Vec2i(960, 720), Vec2i(340, 620)};
	Vec2i t2[3] = {Vec2i(450, 740), Vec2i(530, 640), Vec2i(340, 180)};

	triangle(t0[0], t0[1], t0[2], image, red);
	triangle(t1[0], t1[1], t1[2], image, white);
	triangle(t2[0], t2[1], t2[2], image, green);


	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

