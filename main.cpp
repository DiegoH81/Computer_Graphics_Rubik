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
#include "shape.h"
#include "matrix.h"
#include "camera.h"
#include "scene.h"


float bgR = 0.0f, bgG = 0.0f, bgB = 0.0f;

AnimationList animations, camera_animations;
std::vector<SceneNode*> nodes;

float offset = 0.1f;
float angle = 10.0f;
bool is_moving = true;

int current_id = 0;
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "uniform mat4 model;\n"
                                 "uniform mat4 view;\n"
                                 "uniform mat4 projection;\n"
                                 "void main()\n"
                                 "{\n"
                                 "  gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
                                 "}\0";

const char *fragmentShader = "#version 330 core\n"
                             "out vec4 FragColor;\n"
                             "uniform vec3 color;\n"
                             "void main()\n"
                             "{\n"
                             "   FragColor = vec4(color, 1.0f);\n"
                             "}\0";
						   								   



void traslate(const Vector3& in_m)
{
    for (auto &m_i : nodes)
    {
		if (m_i->id != current_id)
			continue;
		
        m_i->traslate(in_m, false);
    }
}

void scale(float factor)
{
    for (auto &m_i : nodes)
	{
		if (m_i->id != current_id)
			continue;

		m_i->scale(Vector3(factor, factor, factor), false);
	}
}

void rotate_c_x(float angle)
{
    for (auto &m_i : nodes)
    {
		if (m_i->id != current_id)
			continue;
        m_i->rotate_x_local(angle, false);
    }
}

void rotate_c_y(float angle)
{
    for (auto &m_i : nodes)
    {
		if (m_i->id != current_id)
			continue;
        m_i->rotate_y_local(angle, false);
    }
}

void rotate_c_z(float angle)
{
    for (auto &m_i : nodes)
    {
		if (m_i->id != current_id)
			continue;
        m_i->rotate_z_local(angle, false);
    }
}

void frame_buffer_size_call_back(GLFWwindow* in_window, int in_w, int in_h)
{
    glViewport(0, 0, in_w, in_h);
}

void key_call_back(GLFWwindow* in_window, int key, int scan_code, int action, int mods)
{
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
        else if ( key == GLFW_KEY_Z)
        {
            
            animations.add_animation({AnimationInfo(0, 180, "ROTATE_Z", "PUBLIC"),
                                      AnimationInfo(1, 180, "ROTATE_C_Z", "PUBLIC")}, 1.0);
            
            animations.add_animation({AnimationInfo(0, -180, "ROTATE_Z", "PUBLIC"),
                                      AnimationInfo(1, -180, "ROTATE_C_Z", "PUBLIC")}, 1.0);

            animations.add_animation({AnimationInfo(2, 180, "ROTATE_C_Y", "PUBLIC"),
                                      AnimationInfo(2, 180, "ROTATE_Y", "PUBLIC"),
                                      AnimationInfo(3, 180, "ROTATE_C_Y", "PUBLIC"),
                                      AnimationInfo(3, 180, "ROTATE_Y", "PUBLIC")}, 1.0);

            animations.add_animation({AnimationInfo(2, -180, "ROTATE_C_Y", "PUBLIC"),
                                      AnimationInfo(2, -180, "ROTATE_Y", "PUBLIC"),
                                      AnimationInfo(3, -180, "ROTATE_C_Y", "PUBLIC"),
                                      AnimationInfo(3, -180, "ROTATE_Y", "PUBLIC")}, 1.0);

            animations.add_animation({AnimationInfo(4, 1.0, "SCALE_X", "PUBLIC"),
                                     AnimationInfo(4, 1.0, "SCALE_Y", "PUBLIC"),
                                     AnimationInfo(4, 1.0, "SCALE_Z", "PUBLIC")}, 1.0);
                                
            animations.add_animation({AnimationInfo(4, -1.0, "SCALE_X", "PUBLIC"),
                                     AnimationInfo(4, -1.0, "SCALE_Y", "PUBLIC"),
                                     AnimationInfo(4, -1.0, "SCALE_Z", "PUBLIC")}, 1.0);
        }
        else if ( key == GLFW_KEY_V )
        {
            camera_animations.add_animation({AnimationInfo(2, 0, "FOLLOW", "")}, 6.0);
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
    Camera camera;
    camera.set_pos(Point3(0.0f, 0.0f, 2.0f));
    camera.set_objective(Point3(0.0f, 0.0f, 0.0f));

    // Figuras
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    
    Cube cubeUP(0.2);
    cubeUP.add_faces(&golden);

    Cube cubeDOWN(0.2);
    cubeDOWN.add_faces(&lava);

    Pyramid pyrLEFT(0.2, 0.1);
    pyrLEFT.add_faces(&pink);

   
    Pyramid pyrRIGHT(0.2, 0.1);
    pyrRIGHT.add_faces(&turquesa);

    Sphere sph(40, 0.2f);
    sph.add_faces(&le_lime);
    sph.add_edges();
    sph.add_points();


    Cube hijoww(0.2);
    hijoww.add_faces(&red);

	SceneNode root(-1);
    root.traslate(Vector3(0.0, 0.0, 0.0f), true);

     // Nodos
    SceneNode cubeUN(0, &cubeUP);
    cubeUN.traslate(Vector3(0.0f, 0.4, 0.0f), false);

    SceneNode cubeDN(1, &cubeDOWN);
    cubeDN.traslate(Vector3(0.0f, -0.4, 0.0f), false);

    SceneNode pyrLN(2, &pyrLEFT);
    pyrLN.traslate(Vector3(-0.4f, 0.0, 0.0f), true);
    pyrLN.rotate_z_local(90, true);

    SceneNode pyrRN(3, &pyrRIGHT);
    pyrRN.traslate(Vector3(0.4f, 0.0, 0.0f), true);
    pyrRN.rotate_z_local(-90, true);
    
    SceneNode hijowww(5, &hijoww);
    hijowww.traslate(Vector3(0.0f, 0.2f, 0.0f), false);


    SceneNode sphN(4, &sph);
    
    
    
    
    root.add_children(&cubeDN);
    root.add_children(&cubeUN);
    root.add_children(&pyrLN);
    root.add_children(&pyrRN);
    root.add_children(&sphN);
    
    pyrLN.add_children(&hijowww);
    

    int v_count = 0;
    int i_count = 0;	
	

    nodes.push_back(&cubeUN);
    nodes.push_back(&cubeDN);
    nodes.push_back(&pyrLN);
    nodes.push_back(&pyrRN);
    nodes.push_back(&sphN);
    nodes.push_back(&hijowww);
    
    // Bucle
	glPointSize(10.0f);
	glLineWidth(5.0f);
	
    glEnable(GL_DEPTH_TEST);

    float delta_time = 0.0f;
    float last_frame = 0.0f;


    shaders.use_shader("UNIQUE");

    auto projection_matrix = get_perspective(45.0f, float(width)/float(height), 0.1f, 100.0f);
    shaders.set_mat4("UNIQUE", "projection", projection_matrix);

    

    while(!glfwWindowShouldClose(window))
    {
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        if (is_moving)
        {
            animations.process_animations(nodes, delta_time);
            camera_animations.process_animations_camera(camera, nodes, delta_time);
        }

        glClearColor(bgR, bgG, bgB, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
    


        //camera.set_objective(sphA.center);

        auto view_matrix = camera.get_look_at();
        shaders.set_mat4("UNIQUE", "view", view_matrix);
        
        root.draw(shaders, Matrix_4());
        /*
        for (auto &s : nodes)
        s->draw(shaders, Matrix_4());
        */
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Delete - optional
    shaders.delete_programs();
    glfwTerminate();
    return 0;
}