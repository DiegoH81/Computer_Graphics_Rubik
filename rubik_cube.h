#ifndef RUBIK_CUBE_H
#define RUBIK_CUBE_H

#include <vector>
#include <queue>
#include <random>

#include "shape.h"
#include "shader_list.h"
#include "matrix.h"
#include "scene.h"
#include "vertex.h"
#include "texture_list.h"
#include "color.h"

#include <algorithm>
#include "animation_list.h"



class Rubik
{
public:
	Rubik(const float &in_animation_time);
	void draw(ShaderList& shaders);
    void move(int dir, std::string move_cmd, bool is_stacking = false);
	void process_animation(const float& in_delta);
    void scramble(int moves);
    SceneNode* get_center();
    std::vector <std::pair<std::string, Point3>> get_face_colors(char face);
    
private:
    std::vector<Cube*> cubes;
    std::queue<std::pair<float, char>> layer_queue;
	SceneNode *center, *pivot;
    TextureList textures;
    bool is_animating;
    float animation_time;
	AnimationList animations;

    void execute_move(int dir, float pos, char axis, int dir_sign, bool is_stacking);
    SceneNode* find_layer(float value, char axis);
    void replace_layers_child(SceneNode* in_pivot);
	void destroy_temp_pivot();    
};

#endif