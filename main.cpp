#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <thread>

#include "utils.h"
#include "shader_list.h"
#include "animation_list.h"
#include "camera.h"
#include "texture_list.h"
#include "rubik_cube.h"


Color background_color(205, 208, 255, true);
Camera camera_world;

AnimationList camera_animations;
std::vector<SceneNode*> nodes;

float offset = 0.1f;
float angle = 10.0f;
bool is_moving = true;

int current_id = 0;
int dir = 1;

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aNormal;\n"
                                 "layout (location = 2) in vec2 aTexCoord;\n"
                                 "uniform mat4 model;\n"
                                 "uniform mat4 view;\n"
                                 "uniform mat4 projection;\n"
                                 "out vec2 TexCoord;\n"
                                 "void main()\n"
                                 "{\n"
                                 "  gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
                                 "  TexCoord = aTexCoord;\n"
                                 "}\0";

const char *fragmentShader = "#version 330 core\n"
                             "out vec4 FragColor;\n"
                             "in vec2 TexCoord;\n"
                             "uniform bool useTexture;\n"
                             "uniform vec3 color;\n"
                             "uniform sampler2D ourTexture;\n"
                             "void main()\n"
                             "{\n"
                             "   if(useTexture)\n"
                             "   {\n"
                             "      FragColor = texture(ourTexture, TexCoord);\n"
                             "   }\n"
                             "   else\n"
                             "   {\n"
                             "      FragColor = vec4(color, 1.0f);\n"
                             "   }\n"
                             "}\0";
						   								   



void traslate(const Vector3& in_m)
{
    for (auto &m_i : nodes)
    {
		if (m_i->id != current_id)
			continue;
		
        m_i->traslate(in_m, true);
    }
}

void scale(float factor)
{
    for (auto &m_i : nodes)
	{
		if (m_i->id != current_id)
			continue;

		m_i->scale(Vector3(factor, factor, factor), true);
	}
}

void rotate_c_x(float angle)
{
    for (auto &m_i : nodes)
    {
		if (m_i->id != current_id)
			continue;
        m_i->rotate_x_local(angle, true);
    }
}

void rotate_c_y(float angle)
{
    for (auto &m_i : nodes)
    {
		if (m_i->id != current_id)
			continue;
        m_i->rotate_y_local(angle, true);
    }
}

void rotate_c_z(float angle)
{
    for (auto &m_i : nodes)
    {
		if (m_i->id != current_id)
			continue;
        m_i->rotate_z_local(angle, true);
    }
}

void frame_buffer_size_call_back(GLFWwindow* in_window, int in_w, int in_h)
{
    glViewport(0, 0, in_w, in_h);
}

void key_call_back(GLFWwindow* in_window, int key, int scan_code, int action, int mods)
{
    Rubik* cubito = static_cast<Rubik*>(glfwGetWindowUserPointer(in_window));
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (key ==GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(in_window, true);
        else if ( key == GLFW_KEY_A )
            traslate(Vector3(-offset, 0.0f, 0.0f));
        else if ( key == GLFW_KEY_D)
            traslate(Vector3(offset, 0.0f, 0.0f));
        else if ( key == GLFW_KEY_W)
            traslate(Vector3(0.0f, offset, 0.0f));
        else if ( key == GLFW_KEY_S)
            traslate(Vector3(0.0f, -offset, 0.0f));
        else if ( key == GLFW_KEY_1)
            cubito->move(dir, "U");
        else if ( key == GLFW_KEY_2)
            cubito->move(dir, "D");
        else if ( key == GLFW_KEY_3)
            cubito->move(dir, "R");
        else if ( key == GLFW_KEY_4)
            cubito->move(dir, "L");
        else if ( key == GLFW_KEY_5)
            cubito->move(dir, "F");
        else if ( key == GLFW_KEY_6)
            cubito->move(dir, "B");
		else if ( key == GLFW_KEY_0)
            dir *= -1;
		else if ( key == GLFW_KEY_R)
            cubito->scramble(25);
        else if ( key == GLFW_KEY_I)
            rotate_c_x(angle);
        else if ( key == GLFW_KEY_O)
            rotate_c_x(-angle);
        else if ( key == GLFW_KEY_K)
            rotate_c_y(angle);
        else if ( key == GLFW_KEY_L)
            rotate_c_y(-angle);
        else if ( key == GLFW_KEY_N)
            rotate_c_z(angle);
        else if ( key == GLFW_KEY_M)
            rotate_c_z(-angle);
        else if ( key == GLFW_KEY_X)
            scale(1.0f + offset);
        else if ( key == GLFW_KEY_C)
            scale(1.0f + -offset);
        else if ( key == GLFW_KEY_T)
            is_moving = !is_moving;
        else if ( key == GLFW_KEY_V )
        {
            camera_animations.add_animation({AnimationInfo(0, 360, "ORBIT_X", "")}, 6.0);
        }
        else if ( key == GLFW_KEY_LEFT )
        {
            current_id --;
            if (current_id < 0)
                current_id = nodes.size() -1;
            
            std::cout << "S_size: " << nodes.size() << " << current_id: " << current_id << "\n";
        }
        else if ( key == GLFW_KEY_RIGHT )
        {
            current_id ++;
            if (current_id >= nodes.size())
                current_id = 0;
            
            std::cout << "S_size: " << nodes.size() << " << current_id: " << current_id << "\n";
        }
    }	
}

int main()
{
    const int width = 900;
    const int height = 900;

    // Initialize
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Animation", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_call_back);
    glfwSetKeyCallback(window, key_call_back);

    // Init glad
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // ***********************
    // SHADERSS
    // ***********************
    
    ShaderList shaders;
    shaders.create_vertex_shader(vertexShaderSource);
    shaders.add_fragment_shader("UNIQUE", fragmentShader);
    shaders.delete_shaders();
    
    
    // Colors
    Color pink(255.0f, 0.0f, 255.0f, true);
    Color blue(10.0f, 15.0f, 40.0f, true);
    Color mint(10.0f, 15.0f, 40.0f, true);
    Color lava(226.0f, 45.0f, 0.0f, true);
    Color radioactive(199.0f, 255.0f, 0.0f, true);
    Color turquesa(0.0f, 128.0f, 128.0f, true);
    Color purple(157.0f, 0.0f, 255.0f, true);
    Color golden(225.0f, 190.0f, 150.0f, true);
    Color le_lime(133.0f, 235.0f, 52.0f, true);
    Color red(255.0f, 0.0f, 0.0f, true);
    Color white(255.0f, 255.0f, 255.0f, true);

    // Camera
    camera_world.set_pos(Point3(1.5f, 1.5f, 5.0f));
    camera_world.set_objective(Point3(0.0f, 0.0f, 0.0f));


    // Figuras
	glLineWidth(10.0f);

	Rubik cubito(0.4f);
	
	nodes.push_back(cubito.get_center());

    glfwSetWindowUserPointer(window, &cubito);
	
    // Bucle
	glPointSize(10.0f);

    glEnable(GL_DEPTH_TEST);

    float delta_time = 0.0f;
    float last_frame = 0.0f;
	
    shaders.use_shader("UNIQUE");
    auto projection_matrix = get_perspective(45.0f, float(width)/float(height), 0.1f, 100.0f);
    shaders.set_mat4("UNIQUE", "projection", projection_matrix);

    while(!glfwWindowShouldClose(window))
    {
        camera_animations.process_animations_camera(camera_world, nodes, delta_time);
        cubito.process_animation(delta_time);

        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glClearColor(background_color.r, background_color.g, background_color.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto view_matrix = camera_world.get_look_at();
        shaders.set_mat4("UNIQUE", "view", view_matrix);
        
		//layer->rotate_y_local(50.0f * delta_time, true);
		//cubito.pivot->rotate_y_local(50.0f * delta_time, true);

		
		cubito.draw(shaders);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Delete - optional
    shaders.delete_programs();
    glfwTerminate();
    return 0;
}