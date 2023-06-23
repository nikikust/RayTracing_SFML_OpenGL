#version 450 core
#define MAX_SPHERES 128

struct Sphere
{
    vec4 position_and_radius;
    vec4 color;
};

struct HitInfo
{
    vec3 hit_origin;
    vec3 normal;

    float hit_distance;
};

struct Ray
{
    vec3 origin;
    vec3 direction;
    vec4 color;

    int reflections;
};

layout(std140, binding = 0) uniform spheres_UBO {
    Sphere[MAX_SPHERES] spheres;
};

uniform vec2 resolution;
uniform float screen_ratio;

uniform int spheres_amount;

uniform vec3 sun_direction;


out vec4 fragColor;

HitInfo sphere_intersection(in Ray ray, in Sphere sphere)
{
    vec3 dist = ray.origin - sphere.position_and_radius.xyz;

    float b = dot(ray.direction, dist);
    float c = dot(dist, dist) - sphere.position_and_radius.w * sphere.position_and_radius.w;
    float delta = b * b - c;

    if (delta > 0)
    {
        float t1 = -b + sqrt(delta);
        float t2 = -b - sqrt(delta);

        if (t1 > 0 && t2 > 0)
        {
            vec3 touch_point = ray.origin + ray.direction * (t1 < t2 ? t1 : t2);

            return HitInfo(touch_point, normalize(touch_point - sphere.position_and_radius.xyz), distance(ray.origin, touch_point));
        }
    }
    return HitInfo(ray.origin, ray.direction, 0.0);
}

void main() {
    vec2 pos = vec2(2.0 * gl_FragCoord.x / resolution.x - 1.0,
                    2.0 * gl_FragCoord.y / resolution.y - 1.0);

    vec3 ray_direction = normalize(
        vec3(
             pos.x * screen_ratio,
             2.145,
            -pos.y
        )
    );

    Ray ray = Ray(vec3(0, 0, 0), ray_direction, vec4(1, 1, 1, 1), 0);

    vec4 color = {0.0, 0.0, 0.0, 1.0};
    float closest_hit = 1.0 / 0.0;

    for (int i = 0; i < spheres_amount; i++)
    {
        HitInfo hit = sphere_intersection(ray, spheres[i]);
        if (hit.hit_distance > 0.0 && hit.hit_distance < closest_hit)
        {
            closest_hit = hit.hit_distance;
            color = spheres[i].color * max(dot(hit.normal, -sun_direction), 0);
        }
    }
    
    fragColor = color;
}
