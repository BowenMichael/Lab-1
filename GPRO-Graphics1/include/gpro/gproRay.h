//Credit: Peter Shirley https://raytracing.github.io/books/RayTracingInOneWeekend.html#rays,asimplecamera,andbackground/therayclass
#pragma once
#ifndef RAY_H
#define RAY_H

#include <gpro\gpro-math\gproVector.h>

//A ray is the origin or point a that is headed in a direction or point b
class ray {
public:
    ray() {}
    ray(const point3& origin, const vec3& direction)
        : orig(origin), dir(direction)
    {}

    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }

    //returns the point at which the ray exists for a given t
    point3 at(float t) const {
        return orig + (t*dir);
    }
  

public:
    point3 orig;
    vec3 dir;
};

#endif