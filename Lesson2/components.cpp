#include <vector>
#include <iostream>
#include "components.h"
#include "geometry.h"
#include "tgaimage.h"

void line(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color) {
	bool steep = false;
	if (std::abs(p0.x-p1.x) < std::abs(p0.y-p1.y)) {
		std::swap(p0.x, p0.y);
		std::swap(p1.x, p1.y);
		steep = true;
	}
	if (p0.x > p1.x) {
		std::swap(p0.x, p1.x);
		std::swap(p0.y, p1.y);
	}
	int dx = p1.x - p0.x;
	int dy = p1.y - p0.y;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = p0.y;
	for (int x=p0.x; x<=p1.x; x++) {
		if (steep) {
			image.set(y, x, color);
		} else {
			image.set(x, y, color);
		}
		error2 += derror2;
		if (error2 >= dx) {
			y += (p1.y > p0.y ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

Vec3f cross(Vec3f a, Vec3f b) {
	return Vec3f(
		a.y*b.z-a.z*b.y,
		a.z*b.x-a.x*b.z,
		a.x*b.y-a.y*b.x
	);
}

Vec3f barycentric(Vec2i *pts, Vec2i P) {
	Vec3f u = cross(
		Vec3f(pts[2].x-pts[0].x, pts[1].x-pts[0].x, pts[0].x-P.x), 
		Vec3f(pts[2].y-pts[0].y, pts[1].y-pts[0].y, pts[0].y-P.y)
	);
	if (std::abs(u.z) < 1) {
		return Vec3f(-1, 1, 1);
	}
	return Vec3f(1.f - (u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
}


void triangle(Vec2i *pts, TGAImage &image, TGAColor color) {
	Vec2i bboxmin(image.get_width()-1, image.get_height()-1);
	Vec2i bboxmax(0, 0);
	Vec2i clamp(image.get_width()-1, image.get_height()-1);
	
	for (int i=0; i<3; i++) {
		for (int j=0; j<2; j++) {
			bboxmin[j] = std::max(0, std::min(bboxmin[j], pts[i][j]));
			bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
		}
	}
	Vec2i P;
	for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) {
		for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) {
			Vec3f bc_screen = barycentric(pts, P);
			if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) {
				continue;
			}
			image.set(P.x, P.y, color);
		}
	}
}


void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);

	int total_height = t2.y - t0.y;
	int lower_height = t1.y - t0.y;
	int upper_height = t2.y - t1.y;
	int topdown_width = t2.x - t0.x;
	int lower_width = t1.x - t0.x;
	int upper_width = t2.x - t1.x;
	int bound_a, bound_b;
	for (int i = 0; i < total_height; i++) {
		bound_a = std::lround((float)(i * topdown_width) / total_height) + t0.x;
		if (i <= lower_height && t1.y > t0.y) {  // <= is necessary to prevent dividing by zero;
			bound_b = std::lround((float)(i * lower_width) / lower_height) + t0.x;
		}
		else {
			bound_b = std::lround((float)((i-lower_height) * upper_width) / upper_height) + t1.x;
		}
		if (bound_a > bound_b) {
			std::swap(bound_a, bound_b);
		}
		for (int j = bound_a; j <= bound_b; j++) {
			image.set(j, t0.y+i, color);
		}
	}
    // line(t0, t1, image, orange);
    // line(t1, t2, image, orange);
    // line(t2, t0, image, blue);
}

