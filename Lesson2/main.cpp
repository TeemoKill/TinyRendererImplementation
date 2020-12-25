#include "tgaimage.h"
#include "model.h"
#include "components.h"


const int width = 1920;
const int height = 1080;
Model *model = NULL;


void draw_a_heart(TGAImage &image, TGAColor color) {

	Vec2i t0[3] = {Vec2i(960, 200), Vec2i(660, 360), Vec2i(1260, 360)};
	Vec2i t1[3] = {Vec2i(660, 360), Vec2i(960, 760), Vec2i(1260, 360)};
	Vec2i t2[3] = {Vec2i(660, 360), Vec2i(960, 760), Vec2i(540, 560)};
	Vec2i t3[3] = {Vec2i(1260, 360), Vec2i(960, 760), Vec2i(1380, 560)};
	Vec2i t4[3] = {Vec2i(540, 560), Vec2i(960, 760), Vec2i(660, 820)};
	Vec2i t5[3] = {Vec2i(1380, 560), Vec2i(960, 760), Vec2i(1260, 820)};
	Vec2i t6[3] = {Vec2i(660, 820), Vec2i(960, 760), Vec2i(840, 860)};
	Vec2i t7[3] = {Vec2i(1260, 820), Vec2i(960, 760), Vec2i(1080, 860)};

	triangle(t0, image, red);
	triangle(t1, image, red);
	triangle(t2, image, red);
	triangle(t3, image, red);
	triangle(t4, image, red);
	triangle(t5, image, red);
	triangle(t6, image, red);
	triangle(t7, image, red);
}


int main(int argc, char** argv) {
	if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/african_head.obj");
    }

	TGAImage image(width, height, TGAImage::RGB);

	draw_a_heart(image, red);

	Vec3f light_dir = {0, 0, -1};
	light_dir.normalize();
	
	int scale = std::min(width, height) / 2;
    int xshift = width / 2;
    int yshift = height / 2;
	for (int i=0; i<model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		Vec2i screen_coords[3];
		Vec3f world_coords[3];
		for (int j=0; j<3; j++) {
			Vec3f v = model->vert(face[j]);
			screen_coords[j] = Vec2i(v.x*scale + xshift, v.y*scale + yshift);
			// screen_coords[j] = Vec2i(world_coords.z*scale + xshift, world_coords.y*scale + yshift);
			world_coords[j] = v;
		}
		Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
		n.normalize();
		float intensity = n * light_dir;
		if (intensity > 0) {
			triangle(
				screen_coords, 
				image, 
				TGAColor(intensity*255, intensity*255, intensity*255, 255)
			);
		}
	}


	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

