#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "gpro/hittable.h"
#include "gpro/gpro-math/gproVector.h"

class sphere : public hittable {
public:
    sphere() {
        
    }
    sphere(point3 cen, float r) : center(cen), radius(r) {};

    virtual bool hit(
        const ray& r, float tmin, float tmax, hit_record& rec) const override;

public:
    point3 center{};
    float radius{};
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    /*
    1. (P(t) - C)^2 - r^2
    2. (P(t)^2 - 2(P(t)*C) + C^2 - r^2 =0
    3. (t^2)b*b+2tb*(A−C)+(A−C)*(A−C)−r^2=0
    */

    //Calculation to see if the point on the screen is on the circle
    vec3 oc = r.origin() - center; //ray pointing at center of circle from the start of the ray
    float a = r.direction().length_squared(); //th P(t) ^2 in #2
    float half_b = dot(oc, r.direction()); //half b makes caclulation of discriminant easier no constants needed. 2(P(t)*C) in #2
    float c = oc.length_squared() - radius * radius; // C^2 - r^2 in #2 
    float discriminant = half_b * half_b - a * c; //sovlving for the discriminate in quadratic equasion

    if (discriminant > 0) {
        float root = sqrt(discriminant);

        float temp = (-half_b - root) / a; //solving for t
        //Calculates the normal vector and makes sure the hit is valid
        if (temp < t_max && temp > t_min) { //if t is within the bounds of the given parameters
            rec.t = temp;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - center) / radius;
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            return true;
        }

        temp = (-half_b + root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - center) / radius;
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            return true;
        }
    }

    return false;
}


#endif