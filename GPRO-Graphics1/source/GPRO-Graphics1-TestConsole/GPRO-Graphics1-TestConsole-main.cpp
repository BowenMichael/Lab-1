/*
   Copyright 2020 Daniel S. Buckstein

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

/*
	GPRO-Graphics1-TestConsole-main.c/.cpp
	Main entry point source file for a Windows console application.

	Modified by: ____________
	Modified because: ____________
*/


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "gpro/gpro-math/gproVector.h"
#include "gpro/gproColor.h"
#include "gpro/gproRay.h"
#include "gpro/hittableList.h"
#include "gpro/rtWeekend.h"
#include "gpro/sphere.h"
#include "gpro/camera.h"

void testVector()
{
	// test array vector initializers and functions
	float3 av, bv, cv, dv;
	vec3default(av);								// -> a = (0, 0, 0)
	vec3init(bv, 1.0f, 2.0f, 3.0f);					// -> b = (1, 2, 3)
	vec3copy(dv, vec3init(cv, 4.0f, 5.0f, 6.0f));	// -> d = c = (4, 5, 6)
	vec3copy(av, dv);								// a = d			-> a = (4, 5, 6)
	vec3add(dv, bv);								// d += b			-> d = (4 + 1, 5 + 2, 6 + 3) = (5, 7, 9)
	vec3sum(dv, bv, bv);							// d = b + b		-> d = (1 + 1, 2 + 2, 3 + 3) = (2, 4, 6)
	vec3add(vec3sum(dv, cv, bv), av);				// d = c + b + a	-> d = (4 + 1 + 4, 5 + 2 + 5, 6 + 3 + 6) = (9, 12, 15)

#ifdef __cplusplus
	// test all constructors and operators
	vec3 a, b(1.0f, 2.0f, 3.0f), c(cv), d(c);		// default; init; copy array; copy
	a = d;											// assign						-> a = (4, 5, 6)
	d += b;											// add assign					-> d = (5, 7, 9)
	d = b + b;										// sum, init, assign			-> d = (2, 4, 6)
	d = c + b + a;									// sum, init, sum, init, assign	-> d = (9, 12, 15)
#endif	// __cplusplus
}

float hit_sphere(const point3& center, float radius, const ray& r) {
	vec3 oc = r.origin() - center;
	float a = r.direction().length_squared();
	float half_b = dot(oc, r.direction());
	float c = oc.length_squared() - radius * radius;
	float discriminant = half_b * half_b - a * c;
	if (discriminant < 0) {
		return -1.0f;
	}
	else {
		return (-half_b - sqrt(discriminant)) / a;
	}
}

/// <summary>
/// Compares the ray to the world and returns a color
/// </summary>
/// <param name="r"></param>
/// <returns></returns>
color ray_color(const ray& r, const hittable& world) {
	hit_record rec;
	if (world.hit(r, 0, infinity, rec)) {// casts a ray into infinity and if it hits an image flag true //infinity
		//std::cerr << (r.origin() - rec.normal).x * 2 << std::endl;
		//system("pause");
		return vec3(0.5f/rec.t, 0.5f /rec.t, 0.5f /rec.t); // The further a pixel is away from the camera the datker the pixel
		
	}
	//Backround color gradiant
	vec3 unit_direction = normalizeVector(r.direction());
	float t = 0.5f * (unit_direction.y + 1.0f);
	return  (1.0f - t) * color(1.0, 1.0, 1.0) + (t*1.0f) * color(0.5f, 0.7f, 1.0f); // magic numbers are for the backround color
}

int main(int const argc, char const* const argv[])
{
	// Image set up
	const float aspect_ratio = 16.0f / 9.0f; //sets aspect ratio to 16:9
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 100; // Numper of pixel samples take around a each pixel. The higher the number the better the anti-aliasing but the higher the compute time

	// World Creation
	hittable_list world; //List of hittable objects

	//creating objects to add to the world
	world.add(make_shared<sphere>(point3(0.0f, 0.0f, -1.0f), 0.5f)); //ball
	//world.add(make_shared<sphere>(point3(0.0f, -100.5f, -1.0f), 100.0f)); //ground
	//world.add(make_shared<sphere>(point3(-1.0f, 0.0f, -1.0f), .5f)); //ball
	//world.add(make_shared<sphere>(point3(1.0f, 0.0f, -1.0f), .5f)); //ball

	// Camera init
	camera cam;

	// Render

	//outputing to a file
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	//Loops through the specified size of the file to assign RGB values to each specified pixel
	for (int j = image_height - 1; j >= 0; --j) { // runs backwards to that the first pixel read is the bottom left pixel
		std::cerr << "\rScanlines remaing: " << j << ' ' << std::flush; //using the error output stream so to not conflict with the standard output stream
		for (int i = 0; i < image_width; ++i) {
			color pixel_color(0, 0, 0); //creates pixel color variable
			for (int s = 0; s < samples_per_pixel; ++s) { //the more this look runs the more blended the image will look
				//get the uv or x/y of the veiwport then calls ray color to check what color to assign. 
				float u = (i + random_float()) / (image_width - 1); //the x poisition on the screen converted to be between 0 and 1
				float v = (j + random_float()) / (image_height - 1); //the y poisition on the screen converted to be between 0 and 1
				ray r = cam.get_ray(u, v); //u and v ouline the direction of the ray from the camera's origin to that point
				pixel_color += ray_color(r, world); //adds up the aggragrate of all the rays to get a more smooth antialiased image
			}
			write_color(std::cout, pixel_color, samples_per_pixel); // writes the color to the image
		}
	}
	std::cerr << "\nDone. \n";

	printf("\n\n");
	system("pause");
}
