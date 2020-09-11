//Credit: Peter Shirley https://raytracing.github.io/books/RayTracingInOneWeekend.html#rays,asimplecamera,andbackground/therayclass
#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
public:
    camera() {
        //camera set up
        float aspect_ratio = 16.0f / 9.0f;
        float viewport_height = 2.0f;
        float viewport_width = aspect_ratio * viewport_height;
        float focal_length = 1.0f;

        origin = point3(0, 0, 0);
        horizontal = vec3(viewport_width, 0.0, 0.0);
        vertical = vec3(0.0, viewport_height, 0.0);
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
    }

    /// <summary>
    /// returns a ray for a give point on the veiwport
    /// </summary>
    /// <param name="u">x</param>
    /// <param name="v">y</param>
    /// <returns>ray from camera to that u,v point</returns>
    ray get_ray(float u, float v) const {
        return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};
#endif