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

    std::vector <std::pair<std::string, Point3>> get_face_colors(char face);

    std::string get_front()
    {
        auto face_colors = get_face_colors('F');
        
        std::sort(face_colors.begin(), face_colors.end(), order('Y', INVERSO));
        std::sort(face_colors.begin(), face_colors.begin() + 3, order('X', NORMAL));
        std::sort(face_colors.begin() + 3, face_colors.begin() + 6, order('X', NORMAL));
        std::sort(face_colors.begin() + 6, face_colors.end(), order('X', NORMAL));

        std::string to_return;

        for (auto &c : face_colors)
            to_return += c.first;


        return to_return;
    }

    std::string get_back()
    {
        auto face_colors = get_face_colors('B');
        
        std::sort(face_colors.begin(), face_colors.end(), order('Y', INVERSO));
        std::sort(face_colors.begin(), face_colors.begin() + 3, order('X', INVERSO));
        std::sort(face_colors.begin() + 3, face_colors.begin() + 6, order('X', INVERSO));
        std::sort(face_colors.begin() + 6, face_colors.end(), order('X', INVERSO));

        std::string to_return;

        for (auto &c : face_colors)
            to_return += c.first;


        return to_return;
    }

    std::string get_left()
    {
        auto face_colors = get_face_colors('L');
        
        std::sort(face_colors.begin(), face_colors.end(), order('Y', INVERSO));
        std::sort(face_colors.begin(), face_colors.begin() + 3, order('Z', NORMAL));
        std::sort(face_colors.begin() + 3, face_colors.begin() + 6, order('Z', NORMAL));
        std::sort(face_colors.begin() + 6, face_colors.end(), order('Z', NORMAL));

        std::string to_return;

        for (auto &c : face_colors)
            to_return += c.first;


        return to_return;
    }

    std::string get_right()
    {
        auto face_colors = get_face_colors('R');
        
        std::sort(face_colors.begin(), face_colors.end(), order('Y', INVERSO));
        std::sort(face_colors.begin(), face_colors.begin() + 3, order('Z', INVERSO));
        std::sort(face_colors.begin() + 3, face_colors.begin() + 6, order('Z', INVERSO));
        std::sort(face_colors.begin() + 6, face_colors.end(), order('Z', INVERSO));

        std::string to_return;

        for (auto &c : face_colors)
            to_return += c.first;


        return to_return;
    }

    std::string get_top()
    {
        auto face_colors = get_face_colors('T');
        
        std::sort(face_colors.begin(), face_colors.end(), order('Z', NORMAL));
        std::sort(face_colors.begin(), face_colors.begin() + 3, order('X', NORMAL));
        std::sort(face_colors.begin() + 3, face_colors.begin() + 6, order('X', NORMAL));
        std::sort(face_colors.begin() + 6, face_colors.end(), order('X', NORMAL));

        std::string to_return;

        for (auto &c : face_colors)
            to_return += c.first;


        return to_return;
    }

    std::string get_down()
    {
        auto face_colors = get_face_colors('D');
        
        std::sort(face_colors.begin(), face_colors.end(), order('Z', INVERSO));
        std::sort(face_colors.begin(), face_colors.begin() + 3, order('X', NORMAL));
        std::sort(face_colors.begin() + 3, face_colors.begin() + 6, order('X', NORMAL));
        std::sort(face_colors.begin() + 6, face_colors.end(), order('X', NORMAL));

        std::string to_return;

        for (auto &c : face_colors)
            to_return += c.first;


        return to_return;
    }

    void solve()
    {
        auto front = get_front();
        auto back = get_back();
        auto left = get_left();
        auto right = get_right();
        auto top = get_top();
        auto down = get_down();

        std::string total = top + left + front + right + back + down;

        rb::RubikCube rb(total.c_str(), 3);
        rb::RubikCubeSolver* solver = new rb::RubikCube3BasicSolver(rb);

        // Parse
        std::string total_moves = solver->Solve();

        std::stringstream ss(total_moves);
        std::string token;

        std::vector<std::string> moves_lists;
        std::cout << "MOVES: " << total_moves << "\n";
        while (ss >> token)
            moves_lists.push_back(token);
        
        for (auto &cur_move : moves_lists)
        {
            int dir = 1;
            if (cur_move.back() == '\'')
            {
                cur_move.pop_back();
                dir = -1;
            }

            move(dir, cur_move, true);
        }
        
        delete solver;
    }
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