#ifndef CLASSES_H // include guard
#define CLASSES_H

#include <vector>
#include <iostream>
#include <cmath>
namespace st = std;

namespace N
{
    class Object;
    class Ray;

    #define VPO st::vector<Object *>

    class Vec3d
    {
        friend class Ray;
        friend class Sphere;
        friend class Object;

    public:
        Vec3d(float x, float y, float z);
        Vec3d() = default;

        float getx() const;

        float gety() const;

        float getz() const;

        Vec3d operator-();
        Vec3d operator+(Vec3d const &other);
        Vec3d operator-(Vec3d const &other);
        Vec3d operator*(float scalar);
        Vec3d operator/(float scalar);

        float norm();
        Vec3d unit();
        float dot(Vec3d const &other);
        Vec3d cross(Vec3d const &other);

    protected:
        float x;
        float y;
        float z;
    };

    class Object
    {
        friend class Vec3d;

    public:
        explicit Object(float x, float y, float z);
        virtual bool hit(Ray const &ray) const = 0; // Floor hit and Sphere hit are different. improve this.

    protected:
        Vec3d center;
    };

    class Ray
    {
        friend class Vec3d;

    public:
        Ray(Vec3d orig, Vec3d dir, VPO &objects);
        bool scan();

        Vec3d getDir() const;

        Vec3d getOrig() const;

    protected:
        VPO &objects;
        Vec3d orig;
        Vec3d dir;
    };

    class Sphere : public Object
    {

        friend class Vec3d;
        friend class Ray;

    public:
        Sphere(float x, float y, float z, float radius);

        float distFromRay(Ray const &ray) const;
        bool hit(Ray const &ray) const;
        Vec3d hitPoint(Ray const &ray) const; // marco suggested to make hitpoint in Floor and object to make them the same again.
        //the abstract Object kinda has no point now.

    protected:
        Vec3d center;
        float radius;
    };

    class Floor : public Object
    {
        friend class Vec3d;
        friend class Ray;

    public:
        Floor(int x, int y, int z);
        bool hit(Ray const &ray) const;

        Vec3d getCenter() const;
        Vec3d getNormal() const;

    protected:
        int x;
        int y;
        int z;
        st::vector<st::vector<int>> brightness;
        Vec3d center;
        Vec3d normal;
    };

    class Rayscanner
    {
    public:
        Rayscanner(int x, int y, int z, VPO &objects);
        void scan();

    protected:
        Vec3d orig;
        VPO &objects;
    };
}

#endif