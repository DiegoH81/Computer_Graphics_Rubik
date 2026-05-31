#ifndef RUBIK_CUBE_H
#define RUBIK_CUBE_H

#define INVERSO false
#define NORMAL true

#include <vector>
#include <queue>
#include <random>
#include <algorithm>
#include <string>
#include <algorithm>
#include <sstream>

#include "shape.h"
#include "shader_list.h"
#include "matrix.h"
#include "scene.h"
#include "vertex.h"
#include "texture_list.h"
#include "color.h"

#include "animation_list.h"

#include "SOLVER_rubik_cube.h"
#include "SOLVER_rubik_cube_solver.h"


class order
{
public:
    order(char in_axis, bool in_is_minor):
        axis(in_axis), is_minor(in_is_minor)
    {}

    bool operator() (const std::pair<std::string, Point3>& a, const std::pair<std::string, Point3>& b)
    {
        if (axis == 'X' && is_minor)
            return a.second.x < b.second.x;
        else if (axis == 'X' && !is_minor)
            return a.second.x > b.second.x;
        else if (axis == 'Y' && is_minor)
            return a.second.y < b.second.y;
        else if (axis == 'Y' && !is_minor)
            return a.second.y > b.second.y;
        else if (axis == 'Z' && is_minor)
            return a.second.z < b.second.z;
        else if (axis == 'Z' && !is_minor)
            return a.second.z > b.second.z;
    }

private:
    char axis;
    bool is_minor;
};

class Rubik
{
public:
	Rubik(const float &in_animation_time);
	void draw(ShaderList& shaders);
    void move(int dir, std::string move_cmd, bool is_stacking = false);
	void process_animation(const float& in_delta);
    void scramble(int moves);
    SceneNode* get_center();

    void solve();
private:
    std::vector<Cube*> cubes;
    std::queue<std::pair<float, char>> layer_queue;
	SceneNode *center, *pivot;
    TextureList textures;
    bool is_animating;
    float animation_time;
	AnimationList animations;

    void execute_move(int dir, float pos, char axis, int dir_sign, bool is_stacking);
    SceneNode* find_layer(float value, char axis, SceneNode* in_node);
    void replace_layers_child(SceneNode* in_pivot);
	void destroy_temp_pivot();
    
    std::vector <std::pair<std::string, Point3>> get_face_colors(char face);
    std::string get_front();
    std::string get_back();
    std::string get_left();
    std::string get_right();
    std::string get_top();
    std::string get_down();
};

#endif