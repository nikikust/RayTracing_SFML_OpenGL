#include <RayTracing_OpenGL/Renderer/Renderer.h>


Renderer::Renderer(DataStorage& data_storage)
    : data_storage_(data_storage)
{
    prepare_buffers();
    prepare_shaders();

    on_resize(true);
}
Renderer::~Renderer()
{
    // delete[] dotPositions;
    // delete[] dotColors;
}

void Renderer::update()
{
    // --- Ray Tracing

    float focus_distance = 1.f / tanf(data_storage_.vFOV_half);

    glUseProgram(shader_program);

    //////

    GLint resUniform = glGetUniformLocation(shader_program, "resolution");
    glUniform2f(resUniform, (float)data_storage_.screen_size.x, (float)data_storage_.screen_size.y);

    GLint ratioUniform = glGetUniformLocation(shader_program, "screen_ratio");
    glUniform1f(ratioUniform, data_storage_.screen_ratio);

    GLint sizeUniform = glGetUniformLocation(shader_program, "spheres_amount");
    glUniform1i(sizeUniform, (GLint)data_storage_.spheres.size());

    GLint sunDirUniform = glGetUniformLocation(shader_program, "sun_direction");
    glUniform3f(sunDirUniform, data_storage_.sun_direction.x, data_storage_.sun_direction.y, data_storage_.sun_direction.z);

    /*
    auto rotator = tracer::rotation_matrix(data_storage_.camera.angles.x, data_storage_.camera.angles.y);
    

    std::for_each(std::execution::par_unseq, m_ImageVerticalIter.begin(), m_ImageVerticalIter.end(),
        [this, size, &rotator, focus_distance](int y)
        {
            std::for_each(std::execution::par_unseq, m_ImageHorizontalIter.begin(), m_ImageHorizontalIter.end(),
            [this, y, size, &rotator, focus_distance](int x)
                {
                    float pos_x = 2.f * x / size.x - 1.f;
                    float pos_y = 2.f * y / size.y - 1.f;

                    gears::LookAt ray_direction{
                        pos_x* data_storage_.screen_ratio,
                        focus_distance,
                        -pos_y
                    };

                    ray_direction = ray_direction * rotator;

                    auto color = trace_ray(
                        tracer::Ray{
                            data_storage_.camera.origin,
                            glm::normalize(ray_direction)
                        }
                    );

                    this->dotPositions[(y * (int)size.x + x) * 2 + 0] =  pos_x;
                    this->dotPositions[(y * (int)size.x + x) * 2 + 1] = -pos_y;

                    this->dotColors[(y * (int)size.x + x) * 4 + 0] = color.r;
                    this->dotColors[(y * (int)size.x + x) * 4 + 1] = color.g;
                    this->dotColors[(y * (int)size.x + x) * 4 + 2] = color.b;
                    this->dotColors[(y * (int)size.x + x) * 4 + 3] = color.a;
                }
            );
        }
    );
    
    glBindBuffer(GL_ARRAY_BUFFER, id_VBO);
    glBufferData(GL_ARRAY_BUFFER, size_of_dotPositions + size_of_dotColors, NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, size_of_dotPositions, dotPositions);
    glBufferSubData(GL_ARRAY_BUFFER, size_of_dotPositions, size_of_dotColors, dotColors);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    */
}

void Renderer::draw()
{
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer::on_resize(bool init)
{
    // --- Update ray tracing boundaries
    // m_ImageHorizontalIter.resize(data_storage_.screen_size.x);
    // m_ImageVerticalIter.resize(data_storage_.screen_size.y);
    // for (int i = 0; i < data_storage_.screen_size.x; i++)
    //     m_ImageHorizontalIter[i] = i;
    // for (int i = 0; i < data_storage_.screen_size.y; i++)
    //     m_ImageVerticalIter[i] = i;
    //
    // --- Realloc frame buffer
    // if (!init)
    // {
    //     delete[] dotPositions;
    //     delete[] dotColors;
    // }
    //
    // dotPositions = new float[data_storage_.screen_size.x * data_storage_.screen_size.y * 2];
    // dotColors    = new float[data_storage_.screen_size.x * data_storage_.screen_size.y * 4];
    // 
    // size_of_dotPositions = 2 * sizeof(float) * data_storage_.screen_size.x * data_storage_.screen_size.y;
    // size_of_dotColors    = 4 * sizeof(float) * data_storage_.screen_size.x * data_storage_.screen_size.y;
    //
    // --- Recalculate OpenGL buffer pointers
    // glBindBuffer(GL_ARRAY_BUFFER, id_VBO);
    // 
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)(0));
    // glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(size_of_dotPositions));
    // 
    // glEnableVertexAttribArray(0);
    // glEnableVertexAttribArray(1);
    // 
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Renderer::prepare_buffers()
{
    // Create a full-screen quad
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, 0.0f, 1.0f
	};
	
	// --- VAO

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// --- VBO

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// --- UBO

	GLuint ubo;
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);

	glBufferData(GL_UNIFORM_BUFFER, sizeof(tracer::Sphere) * 128, data_storage_.spheres.data(), GL_DYNAMIC_DRAW);

	GLuint bindingPoint = 0;
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ubo);
}
void Renderer::prepare_shaders()
{
    // --- Compile shaders
    auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    auto vertex_shader_source = gears::load_shader_text("shaders/shader.vert");

    const char* vs = vertex_shader_source.c_str();
    glShaderSource(vertex_shader, 1, &vs, NULL);
    glCompileShader(vertex_shader);

    auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    auto fragment_shader_source = gears::load_shader_text("shaders/shader.frag");

    const char* fs = fragment_shader_source.c_str();
    glShaderSource(fragment_shader, 1, &fs, NULL);
    glCompileShader(fragment_shader);

    // --- Create shader program
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    // --- Delete shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    GLint posAttrib = glGetAttribLocation(shader_program, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

gears::Color Renderer::trace_ray(const tracer::Ray& ray)
{
    using namespace tracer;
    
    if (ray.reflections > data_storage_.max_reflections)
        return { 0.f, 0.f, 0.f, 1.f };

    float min_hit_distance = FLT_MAX;
    gears::Color res_color = sky_intersection(ray, glm::dot(-data_storage_.sun_direction, ray.direction) > 0.999f);

    for (auto& sphere : data_storage_.spheres)
    {
        if (auto intr = sphere_intersection(ray, sphere))
        {
            float curr_distance = glm::length(intr.value().hit_origin - ray.origin);
            if (curr_distance < min_hit_distance)
            {
                auto diff = sphere.material.color * light_intensity(intr.value().normal, data_storage_.sun_direction);
                if (true) // sphere.material.albedo == 0.f)
                    res_color = diff;
                else
                {
                    auto new_ray = reflect(ray, intr.value());

                    res_color = glm::mix(diff, trace_ray(new_ray), 0.0f); // sphere.material.albedo);
                }
                min_hit_distance = curr_distance;
            }
        }
    }

    return res_color;
}
