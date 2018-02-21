#define _USE_MATH_DEFINES
#include "vec3d.h"
#include <cmath>
#include <algorithm>
#include <limits>
#include <iostream>

using namespace std;

Vec3d::Vec3d() : x(0), y(0), z(0)
{
}

Vec3d::Vec3d(double xvalue, double yvalue, double zvalue) : x(xvalue), y(yvalue), z(zvalue)
{
}

double Vec3d::magnitude()
{
    return sqrt(x*x + y*y + z*z);
}

void Vec3d::scale(double s)
{
    x *= s;
    y *= s;
    z *= s;
}

void Vec3d::rotateZ(double radians)
{
    *this = { x * cos(radians) - y * sin(radians), x * sin(radians) + y * cos(radians), z };
}

void Vec3d::translate(Vec3d offset)
{
    x += offset.x;
    y += offset.y;
    z += offset.z;
}

bool Vec3d::equals(Vec3d other, double threshold)
{
    return fabs(x - other.x) < threshold && fabs(y - other.y) < threshold && fabs(z - other.z) < threshold;
}

Vec3d operator-(Vec3d p1, Vec3d p2)
{
    return Vec3d {p1.x - p2.x, p1.y - p2.y , p1.z - p2.z };
}

Vec3d operator+(Vec3d p1, Vec3d p2)
{
    return Vec3d {p1.x + p2.x, p1.y + p2.y, p1.z + p2.z };
}

Vec3d operator*(Vec3d v, double s)
{
    return Vec3d { v.x * s, v.y * s, v.z * s };
}

Vec3d operator*(double s, Vec3d v)
{
    return Vec3d { v.x * s, v.y * s, v.z * s };
}

