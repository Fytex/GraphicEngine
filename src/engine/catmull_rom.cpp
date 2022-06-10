#define _USE_MATH_DEFINES
#include <math.h>

#include <tuple>
#include <vector>

#include "../utils/point.hpp"
#include "../utils/vector.hpp"

void buildRotMatrix(Vector x, Vector y, Vector z, float* m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


void multMatrixVector(float* m, float* v, float* res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}

}


std::tuple<Point, Vector> getCatmullRomPoint(float t, Point p0, Point p1, Point p2, Point p3) {

	// catmull-rom matrix
	float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
						{ 1.0f, -2.5f,  2.0f, -0.5f},
						{-0.5f,  0.0f,  0.5f,  0.0f},
						{ 0.0f,  1.0f,  0.0f,  0.0f} };

	float a[4];
	float v[4];

	Point pos;
	Vector deriv;

	for (int i = 0; i < 3; ++i) {
		v[0] = p0[i];
		v[1] = p1[i];
		v[2] = p2[i];
		v[3] = p3[i];

		multMatrixVector((float*)m, v, a);

		pos[i] = t * t * t * a[0] + t * t * a[1] + t * a[2] + a[3];
		deriv[i] = 3 * t * t * a[0] + 2 * t * a[1] + a[2];
	}

	return std::make_tuple(pos, deriv);

}


// given  global t, returns the point in the curve
std::tuple<Point, Vector> getGlobalCatmullRomPoint(std::vector<Point> points, float gt) {

	int points_count = points.size();

	float t = gt * points_count; // this is the real global t
	int index = static_cast <int> (floor(t));;  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4];
	indices[0] = (index + points_count - 1) % points_count;
	indices[1] = (indices[0] + 1) % points_count;
	indices[2] = (indices[1] + 1) % points_count;
	indices[3] = (indices[2] + 1) % points_count;

	return getCatmullRomPoint(t, points[indices[0]], points[indices[1]], points[indices[2]], points[indices[3]]);
}
