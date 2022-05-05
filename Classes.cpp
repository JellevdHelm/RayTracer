#include <vector>
#include <iostream>
#include <cmath>
#include "Classes.h"
using namespace N;
namespace st = std;

#define VPO st::vector<Object *>

//constructors
Vec3d::Vec3d(float x, float y, float z) : x(x), y(y), z(z) {}
Object::Object(float x, float y, float z) : center(x, y, z) {}
Ray::Ray(Vec3d orig, Vec3d dir, VPO &objects) : orig(orig), dir(dir), objects(objects) {}
Sphere::Sphere(float x, float y, float z, float radius) : center(x, y, z), radius(radius), Object(x, y, z) {}
Rayscanner::Rayscanner(int x, int y, int z, VPO &objects) : orig(x, y, z), objects(objects) {}
Floor::Floor(int x, int y, int z) : x(x), y(y), z(z), Object(x, y, z)
{
    center = Vec3d((x / 2), y, (z / 2));
    normal = Vec3d(0, 1, 0); //plane is always flat

    //checkerboard pattern
    brightness.reserve(z); //reserve z elements for height plane

    for (int zi = 0; zi < z; zi++)
    {
        brightness.push_back(st::vector<int>());
        brightness[zi].reserve(x); //reserve x elements for width plane
        for (int xi = 0; xi < x; xi++)
        {
            if ((zi / 20) % 2 == 1) //make squares of 20 high
            {
                if ((xi / 20) % 2 == 1) // and 20 wide
                {
                    brightness[zi].push_back(1);
                }
                else
                {
                    brightness[zi].push_back(0);
                }
            }
            else
            {
                if ((xi / 20) % 2 == 1)
                {
                    brightness[zi].push_back(0);
                }
                else
                {
                    brightness[zi].push_back(1);
                }
            }
        }
    }
}

//Vec3d functions

float Vec3d::getx() const
{
    return x;
};

float Vec3d::gety() const
{
    return y;
};

float Vec3d::getz() const
{
    return z;
};

//operator overloading
Vec3d Vec3d::operator-()
{
    return {x = -x, y = -y, z = -z};
}

Vec3d Vec3d::operator+(Vec3d const &other)
{
    return {x += other.x, y += other.y, z += other.z};
}

Vec3d Vec3d::operator-(Vec3d const &other)
{
    return {x -= other.x, y -= other.y, z -= other.z};
}

Vec3d Vec3d::operator*(float scalar)
{
    return {x *= scalar, y *= scalar, z *= scalar};
}

Vec3d Vec3d::operator/(float scalar)
{
    return {x /= scalar, y /= scalar, z /= scalar};
}

//math functions
float Vec3d::norm()
{
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
};

Vec3d Vec3d::unit()
{
    return *this / this->norm();
};

float Vec3d::dot(Vec3d const &other)
{
    return this->x * other.x + this->y * other.y + this->z * other.z;
};

Vec3d Vec3d::cross(Vec3d const &other)
{
    return Vec3d(this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x);
};

//ray functions

bool Ray::scan()
{
    for (auto object : objects)
    {
        if (object->hit(*this)) //return first hit
        {
            return 1;
        }
    }
    return 0;
}

Vec3d Ray::getDir() const
{
    return dir;
}

Vec3d Ray::getOrig() const
{
    return orig;
}

//sphere functions
float Sphere::distFromRay(Ray const &ray) const
{
    return ((ray.getDir().cross((ray.getOrig() - center))).norm() / ray.getDir().norm());
    // norm van (cross( direction, (orig - center) )) / norm direction
}

bool Sphere::hit(Ray const &ray) const
{
    if (this->radius >= this->distFromRay(ray)) // if distfromray is larger than radius the ray misses
    {
        return 1;
    }
    return 0; //Ray doesnt collide
};

Vec3d Sphere::hitPoint(Ray const &ray) const
{
    // center =  c
    // orig =    1
    // dir =     2

    float x1 = ray.getOrig().getx();
    float x2 = ray.getDir().getx();
    float xc = center.getx();

    float y1 = ray.getOrig().gety();
    float y2 = ray.getDir().gety();
    float yc = center.gety();

    float z1 = ray.getOrig().getz();
    float z2 = ray.getDir().getz();
    float zc = center.getz();

    float a = ray.getDir().dot(ray.getDir());
    float b = -2 * (ray.getDir().dot(ray.getOrig() - center));
    float c = pow(xc, 2) + pow(yc, 2) + pow(zc, 2) + pow(x1, 2) + pow(y1, 2) + pow(z1, 2) -
              2 * ((xc * x1) + (yc * y1) + (zc * z1)) - pow(radius, 2);

    float d = pow(b, 2) - 4 * a * c;

    float t = (-(b) + sqrt(d)) / (2 * a);

    return Vec3d((x1 + t * x2), (y1 + t * y2), (z1 - t * z2));
}

//floor functions
bool Floor::hit(Ray const &ray) const
{
    float d = getNormal().dot(center);

    if (getNormal().dot(ray.getDir()) == 0)
    {
        return 0;
    }

    float t = (d - getNormal().dot(ray.getOrig())) / getNormal().dot(ray.getDir());

    Vec3d newRay = ray.getDir() * t;

    Vec3d hit = ray.getOrig() + newRay;

    //st::cout << "t waarde: " << t;

    if (hit.getx() > x || hit.getx() < 0 || hit.getz() > z || hit.getz() < 0) // if the hitpoint is outside plane coords
    {
        return 0;
    }

    if (t >= 0)
    {
        // if x or z of hit are outside brightness vector size or below 0
        if (round(hit.getx()) > 0 && round(hit.getx()) < brightness.size() && round(hit.getz()) > 0 && round(hit.getz()) < brightness[0].size())
        {
            if (brightness[round(hit.getx())][round(hit.getz())] == 1) // if hit coords fall on a white square
            {
                return 1;
            }
        }
        return 0;
    }

    return 0;
};

Vec3d Floor::getCenter() const
{
    return center;
}

Vec3d Floor::getNormal() const
{
    return normal;
}

//rayscanner functions
void Rayscanner::scan()
{
    Vec3d dir;
    int width = 300;
    int height = 150;                                   //height & width of screen
    st::vector<st::string> character{"  ", "..", "##"}; //brightness characters

    for (int yi = height; yi >= 0; yi--) //loop through height in reverse because it starts at the top
    {
        for (int xi = 0; xi <= width; xi++) //loop through width
        {
            dir = (Vec3d(xi, yi, 0) - orig);
            Ray ray = Ray(orig, dir, objects);

            if (ray.scan()) //scan if the ray hits any objects
            {
                st::cout << character[1];
            }
            else
            {
                st::cout << character[0];
            }
        }
        st::cout << "\n";
    }
    st::cout << "end";
}
