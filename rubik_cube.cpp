#include "rubik_cube.h"


Color white(255.0f, 255.0f, 255.0f, true);
Color yellow(255.0f, 255.0f, 0.0f, true);

Color red(255.0f, 0.0f, 0.0f, true);
Color orange(255.0f, 128.0f, 0.0f, true);

Color blue(0.0f, 70.0f, 255.0f, true);
Color green(0.0f, 255.0f, 0.0f, true);

Color black(0.0f, 0.0f, 0.0f, true);

bool error(float a, float b)
{
	return std::abs(a-b) < 0.05f;
}

Rubik::Rubik(const float& in_animation_time):
	animations(), pivot(nullptr), is_animating(false), animation_time(in_animation_time), layer_queue()
{
    center = new SceneNode(0);
    center->traslate(Vector3(0.0, 0.0, 0.0f), true);

    //cubes.reserve(27);
    Color pink(255.0f, 0.0f, 255.0f, true);
    
    textures.add_texture("Dirt", "dirt.png");
    textures.add_texture("Black", "black_face.png");
    textures.add_texture("Blue", "blue_face.png");
    textures.add_texture("Green", "green_face.png");
    textures.add_texture("Orange", "orange_face.png");
    textures.add_texture("Red", "red_face.png");
    textures.add_texture("White", "white_face.png");
    textures.add_texture("Yellow", "yellow_face.png");
    textures.add_texture("NO_TEXTURE", "no_texture.png");

    float size = 0.5;
    
    for (int i = 0; i < 27; i++)
    {
        Cube* cube_to_push = new Cube(size);
        cube_to_push->add_faces();
        cube_to_push->add_textures("Black");
        //cube_to_push->add_edges(&black);
        //cube_to_push->set_edge_color(ALL_IDs, &black);
        cubes.push_back(cube_to_push);
        SceneNode *to_push = new SceneNode(i, cube_to_push);
        center->add_children(to_push);
    }    
    
    
    // Move
    float possible_moves[3] = {0.52f, 0.0f, -0.52f};
    
    int cur_idx = 0;
    for (auto &z: possible_moves)
        for (auto &y: possible_moves)
            for (auto &x: possible_moves)
            {
                Shape* cube = center->children[cur_idx]->shape;
                center->children[cur_idx]->traslate(Vector3(x, y, z), true);
                

                if (x == 0.52f)
                    cube->set_textures(3, "Red");
                    //cube->set_textures(3, "Dirt");
                    //cube->set_face_color(3, &red);

                if (x == -0.52f)
                    cube->set_textures(2, "Orange");
                    //cube->set_textures(2, "Dirt");
                    //cube->set_face_color(2, &orange);

                if (y == 0.52f)
                    cube->set_textures(4, "White");
                    //cube->set_textures(4, "Dirt");
                    //cube->set_face_color(4, &white);

                if (y == -0.52f)
                    cube->set_textures(5, "Yellow");
                    //cube->set_textures(5, "Dirt");
                    //cube->set_face_color(5, &yellow);

                if (z == 0.52f)
                    cube->set_textures(0, "Blue");
                    //cube->set_textures(0, "Dirt");
                    //cube->set_face_color(0, &blue);

                if (z == -0.52f)
                    cube->set_textures(1, "Green");
                    //cube->set_textures(1, "Dirt");
                    //cube->set_face_color(1, &green);
                
                cur_idx++;
            }
}

SceneNode* Rubik::find_layer(float value, char axis)
{
    bool x_use = (axis == 'X' ? true : false);
    bool y_use = (axis == 'Y' ? true : false);
    bool z_use = (axis == 'Z' ? true : false);

	SceneNode* temp_pivot = new SceneNode(-1, nullptr);
	
	for (auto child :center->children)
    {
		Point3 centerPos = center->get_center_local();
		Point3 childPos = child->get_center_local();
		Point3 relativePos = childPos - centerPos;

		if((!x_use || error(relativePos.x, value)) && (!y_use || error(relativePos.y, value)) && (!z_use || error(relativePos.z, value)))
			temp_pivot->add_children(child);
	}
	
    return temp_pivot;
}

void Rubik::replace_layers_child(SceneNode* in_pivot)
{
	center->add_children(in_pivot);
	
	for(auto cube: in_pivot->children)
    {
		for(auto it = center->children.begin(); it != center->children.end(); it++)
        {
			if(*it == cube)
            {
				center->children.erase(it);
				break;
			}
		}
	}
}

void Rubik::destroy_temp_pivot()
{
    Matrix_4 rounded = pivot->public_transform;
    for (int i = 0; i < 16; i++)
        rounded.matrix[i] = std::round(rounded.matrix[i]);

	for(auto cube : pivot->children)
    {
		cube->private_transform = rounded * cube->private_transform;
		cube->public_transform = rounded * cube->public_transform;

		center->add_children(cube);
	}

	pivot->children.clear();

	for(auto cube: cubes)
    {
		for(auto it = center->children.begin(); it != center->children.end(); it++)
        {
			if(*it == pivot)
            {
				center->children.erase(it);
				break;
			}
		}
	}

	delete pivot;
    pivot = nullptr;
}

void Rubik::draw(ShaderList& shaders)
{
    center->draw(shaders, textures, Matrix_4());
}

void Rubik::move(int dir, std::string move_cmd, bool is_stacking)
{
    if (move_cmd == "U")
        execute_move(dir, 0.5f, 'Y', -1, is_stacking);
    else if (move_cmd == "U2")
    {
        execute_move(dir, 0.5f, 'Y', -1, is_stacking);
        execute_move(dir, 0.5f, 'Y', -1, is_stacking);
    }
    else if (move_cmd == "D")
        execute_move(dir, -0.5f, 'Y', 1, is_stacking);
    else if (move_cmd == "D2")
    {
        execute_move(dir, -0.5f, 'Y', 1, is_stacking);
        execute_move(dir, -0.5f, 'Y', 1, is_stacking);
    }
    else if (move_cmd == "R")
        execute_move(dir, 0.5f, 'X', -1, is_stacking);
    else if (move_cmd == "R2")
    {
        execute_move(dir, 0.5f, 'X', -1, is_stacking);
        execute_move(dir, 0.5f, 'X', -1, is_stacking);
    }
    else if (move_cmd == "L")
        execute_move(dir, -0.5f, 'X', 1, is_stacking);
    else if (move_cmd == "L2")
    {
        execute_move(dir, -0.5f, 'X', 1, is_stacking);
        execute_move(dir, -0.5f, 'X', 1, is_stacking);
    }
    else if (move_cmd == "F")
        execute_move(dir, 0.5f, 'Z', -1, is_stacking);
    else if (move_cmd == "F2")
    {
        execute_move(dir, 0.5f, 'Z', -1, is_stacking);
        execute_move(dir, 0.5f, 'Z', -1, is_stacking);
    }
    else if (move_cmd == "B")
        execute_move(dir, -0.5f, 'Z', 1, is_stacking);
    else if (move_cmd == "B2")
    {
        execute_move(dir, -0.5f, 'Z', 1, is_stacking);
        execute_move(dir, -0.5f, 'Z', 1, is_stacking);
    }
}

void Rubik::process_animation(const float& in_delta)
{
    if (animations.animation_queue.empty())
        return;

    if (!is_animating) // Change layer
    {
        auto top = layer_queue.front();
        pivot = find_layer(top.first, top.second);
        replace_layers_child(pivot);

        layer_queue.pop();
        is_animating = true;
    }

    
    int prev_size = animations.animation_queue.size();
    animations.process_animations({pivot}, in_delta);
    int new_size = animations.animation_queue.size();

    if (prev_size != new_size)
    {
        destroy_temp_pivot();
        is_animating = false;
    }
}

void Rubik::scramble(int moves)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> distribucion_move(0, 11);
    std::uniform_int_distribution<int> distribucion_prime(0, 1);
    std::vector<std::string> possible_moves = {"U", "U2", "D", "D2", "R", "R2", "L", "L2", "F", "F2", "B", "B2"};

    for (int i = 0; i < moves; i++)
    {
        int random_move = distribucion_move(gen);
        int dir = (distribucion_prime(gen))? 1 : -1;

        move(dir, possible_moves[random_move], true);
    }
}

SceneNode* Rubik::get_center()
{
    return center;
}

void Rubik::execute_move(int dir, float pos, char axis, int dir_sign, bool is_stacking)
{
    if (!animations.animation_queue.empty() && !is_stacking)
        return;
    
    layer_queue.push({pos, axis});
    float time = (is_stacking? animation_time / 2.0f : animation_time);

    std::string type = "ROTATE_" + std::string(1, axis);
    animations.add_animation({AnimationInfo(-1, dir * dir_sign* 90, type, "PUBLIC")}, time);
}

std::vector <std::pair<std::string, Point3>> Rubik::get_face_colors(char face)
{
    std::vector <std::pair<std::string, Point3>> to_return;

    auto normal_objective = Vector3();
    SceneNode* new_layer = nullptr;
      
    switch (face)
    {
    case 'F':
    {
        new_layer = find_layer(0.52f, 'Z');
        normal_objective = Vector3(0, 0, 1);
        break;
    }
    case 'B':
    {
        new_layer = find_layer(-0.52f, 'Z');
        normal_objective = Vector3(0, 0, -1);
        break;
    }
    case 'R':
    {
        new_layer = find_layer(0.52f, 'X');
        normal_objective = Vector3(1, 0, 0);
        break;
    }
    case 'L':
    {
        new_layer = find_layer(-0.52f, 'X');
        normal_objective = Vector3(-1, 0, 0);
        break;
    }
    case 'T':
    {
        new_layer = find_layer(0.52f, 'Y');
        normal_objective = Vector3(0, 1, 0);
        break;
    }
    case 'D':
    {
        new_layer = find_layer(-0.52f, 'Y');
        normal_objective = Vector3(0, -1, 0);
        break;
    }
    default:
        break;
    }

    
    // Front - 0
    // Back - 1
    // Left - 2
    // Right - 3
    // Top - 4
    // Down - 5

    for (auto &cube : new_layer->children)
    {
        for (int face_id = 0; face_id < 6; face_id++)
        {
            if (vec_length(cube->get_normal(face_id) - normal_objective) < 0.05f)
            {
                std::string tex_name = cube->shape->info_faces[face_id].texture_name;
                auto cube_center = cube->get_center_local();
                to_return.push_back({tex_name, cube_center});
            }
        }
    }

    delete new_layer;
    return to_return;
}