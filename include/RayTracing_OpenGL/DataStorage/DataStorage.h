#pragma once
#include <vector>


#include <RayTracing_OpenGL/gears/Functions.h>
#include <RayTracing_OpenGL/Renderer/Tracer/Objects.h>


class DataStorage
{
public:
    DataStorage();
    DataStorage(DataStorage&) = delete;
    DataStorage(DataStorage&&) = delete;

    // --- Data --- //

    glm::ivec2 screen_size{};

    double lastTime, deltaTime;

    bool capture_IO = false;
    bool hide_mouse = false;

    glm::ivec2 current_mouse_position{};
    glm::ivec2 mouse_position_delta{};
    glm::ivec2 last_mouse_position{};

    // --- Render data

    tracer::Camera camera = { { 0.f, 0.f, 0.f}, {0.f, 0.f} };

    float vFOV_half = 50.f / 2.f * (glm::pi<float>() / 180.f);
    float screen_ratio = 1.0f;
    float moving_speed = 0.1f;

    uint32_t max_reflections = 2;

    gears::LookAt sun_direction = glm::normalize(gears::LookAt{  -1.0f, 1.0f, -1.0f });
    gears::Color zenith_color { 0.4f, 0.6f, 0.8f, 1.0f };
    gears::Color horizon_color{ 1.0f, 1.0f, 1.0f, 1.0f};

    // --- Objects data

    std::vector<tracer::Sphere> spheres{
        { glm::vec4(-55.0f, 300.0f, -55.0f, 50.0f), 0 },
		{ glm::vec4(-55.0f, 300.0f,  55.0f, 50.0f), 1 },
		{ glm::vec4( 55.0f, 300.0f, -55.0f, 50.0f), 2 },
		{ glm::vec4( 55.0f, 300.0f,  55.0f, 50.0f), 3 }
    };
    std::vector<tracer::Material> materials{
        { glm::vec4(1.0f, 0.2f, 0.2f, 1.0f), 0.0f, 0.0f },
        { glm::vec4(0.2f, 1.0f, 0.2f, 1.0f), 0.0f, 0.0f },
        { glm::vec4(0.2f, 0.2f, 1.0f, 1.0f), 0.0f, 0.0f },
        { glm::vec4(0.1f, 0.5f, 1.0f, 1.0f), 0.0f, 0.0f }
    };
};
