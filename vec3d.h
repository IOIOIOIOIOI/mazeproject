#ifndef VEC3D_H
#define VEC3D_H


#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <string>

class Vec3d
{
public:
    double x;   // fields (also member) (variables that are part of the class)
    double y;
    double z;

    // constructor
    Vec3d();
    Vec3d(double xvalue, double yvalue, double zvalue);

    // methods
    double magnitude();

    void scale(double s);
    void rotateZ(double radians);
    void translate(Vec3d offset);

    Vec3d rotatedZ(double radians) { Vec3d res = *this; res.rotateZ(radians); return res; }
    Vec3d scaled(double s) { Vec3d res = *this; res.scale(s); return res; }
    Vec3d translated(Vec3d offset) { Vec3d res = *this; res.translate(offset); return res; }

    bool equals(Vec3d other, double threshold);

};

// some handy operators
Vec3d operator+(Vec3d p1, Vec3d p2);
Vec3d operator-(Vec3d p1, Vec3d p2);
Vec3d operator*(Vec3d p1, double s);
Vec3d operator*(double s, Vec3d p1);
bool  operator== (Vec3d p1, Vec3d p2);

#endif // VEC3D_H
