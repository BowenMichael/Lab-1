#pragma once
//Credit: Peter Shirley https://raytracing.github.io/books/RayTracingInOneWeekend.html#rays,asimplecamera,andbackground/therayclass
#ifndef HITTABLE_H
#define HITTABLE_H

#include "gpro/gproRay.h"

struct hit_record {
    point3 p;
    vec3 normal;
    float t{};
    bool front_face{};

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif