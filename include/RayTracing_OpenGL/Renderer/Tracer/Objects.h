#pragma once
#include <RayTracing_OpenGL/gears/Functions.h>


namespace tracer
{
    struct Camera
    {
        gears::Origin origin;
        gears::Angles angles;

        void rotate(float x, float y);

        void move_forward(float a);
        void move_backward(float a);
        void move_left(float a);
        void move_right(float a);
        void move_up(float a);
        void move_down(float a);
    };

    struct Ray
    {
        gears::Origin origin;
        gears::LookAt direction;
        gears::Color color;

        uint32_t reflections;

        Ray(const gears::Origin& origin, const gears::LookAt& direction, 
            const gears::Color& color = {1.f, 1.f, 1.f, 1.f },
            const uint32_t& reflections = 0)
            : origin(origin), direction(direction),
              color(color), reflections(reflections) {}
    };

    struct Material
    {
        glm::vec4 stats = {
            0.0f, // roughness
            0.0f, // metallic
            0.0f, 0.0f
        };

        gears::Color color;
    };

    struct Sphere
    {
        glm::vec4 position_and_radius;

        uint32_t material_id;      // packed as vec4
    };

} // namespace tracer
