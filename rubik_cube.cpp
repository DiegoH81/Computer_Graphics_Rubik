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
    return std::abs(a - b) < 0.05f;
}

Rubik::Rubik()
{
    for (int i = 0; i < 9; i++)
    {
        white[i] = '-';
        yellow[i] = '-';
        red[i] = '-';
        orange[i] = '-';
        blue[i] = '-';
        green[i] = '-';
    }

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
        SceneNode* to_push = new SceneNode(i, cube_to_push);
        center->add_children(to_push);
    }


    // Move
    float possible_moves[3] = { 0.5f, 0.0f, -0.5f };

    int cur_idx = 0;
    for (auto& z : possible_moves)
        for (auto& y : possible_moves)
            for (auto& x : possible_moves)
            {
                Shape* cube = center->children[cur_idx]->shape;
                center->children[cur_idx]->traslate(Vector3(x, y, z), true);


                if (x == 0.5f)
                    cube->set_textures(3, "Red");
                //cube->set_textures(3, "Dirt");
                //cube->set_face_color(3, &red);

                if (x == -0.5f)
                    cube->set_textures(2, "Orange");
                //cube->set_textures(2, "Dirt");
                //cube->set_face_color(2, &orange);

                if (y == 0.5f)
                    cube->set_textures(4, "White");
                //cube->set_textures(4, "Dirt");
                //cube->set_face_color(4, &white);

                if (y == -0.5f)
                    cube->set_textures(5, "Yellow");
                //cube->set_textures(5, "Dirt");
                //cube->set_face_color(5, &yellow);

                if (z == 0.5f)
                    cube->set_textures(0, "Blue");
                //cube->set_textures(0, "Dirt");
                //cube->set_face_color(0, &blue);

                if (z == -0.5f)
                    cube->set_textures(1, "Green");
                //cube->set_textures(1, "Dirt");
                //cube->set_face_color(1, &green);

                cur_idx++;
            }
}

SceneNode* Rubik::find_layer(float x, float y, float z, bool x_use, bool y_use, bool z_use)
{
    SceneNode* pivot = new SceneNode(-1, nullptr);
    center->add_children(pivot);

    std::vector<SceneNode*> cubes;
    for (auto child : center->children)
    {
        if (child == pivot)
            continue;

        Point3 centerPos = center->get_center();
        Point3 childPos = child->get_center();
        Point3 relativePos = childPos - centerPos;



        if ((!x_use || error(relativePos.x, x)) && (!y_use || error(relativePos.y, y)) && (!z_use || error(relativePos.z, z)))
            cubes.push_back(child);

    }

    for (auto cube : cubes)
    {
        for (auto it = center->children.begin(); it != center->children.end(); it++)
        {
            if (*it == cube) {
                center->children.erase(it);
                break;
            }
        }
        pivot->add_children(cube);
    }

    return pivot;
}


void Rubik::destroy_temp_pivot(SceneNode* pivot)
{
    for (auto cube : pivot->children)
    {
        cube->private_transform = pivot->public_transform * cube->private_transform;
        cube->public_transform = pivot->public_transform * cube->public_transform;

        center->add_children(cube);
    }

    pivot->children.clear();

    for (auto cube : cubes)
    {
        for (auto it = center->children.begin(); it != center->children.end(); it++)
        {
            if (*it == pivot)
            {
                center->children.erase(it);
                break;
            }
        }
    }

    delete pivot;
}

void Rubik::draw(ShaderList& shaders)
{
    center->draw(shaders, textures, Matrix_4());
}


char Rubik::get_colors(std::string color)
{
    if (color == "Yellow")
        return 'Y';
    if (color == "Blue")
        return 'B';
    if (color == "Red")
        return 'R';
    if (color == "Green")
        return 'G';
    if (color == "White")
        return 'W';
    if (color == "Orange")
        return 'O';
    return '-';
}



void Rubik::cube_state()
{
    int w = 0, y = 0, r = 0, o = 0, b = 0, g = 0;

    for (auto child : center->children)
    {
        if (!child->shape)
            continue;

        Point3 pos = child->get_center();
        std::vector<IndicesInfo>& faces = child->shape->info_faces;

        std::cout << std::endl;
        if (error(pos.y, 0.5f))
        {
            white[w++] = get_colors(faces[4].texture_name);
        }

        if (error(pos.y, -0.5f))
        {
            yellow[y++] = get_colors(faces[5].texture_name);
        }
        if (error(pos.x, 0.5f))
        {
            red[r++] = get_colors(faces[3].texture_name);
        }
        if (error(pos.x, -0.5f))
        {
            orange[o++] = get_colors(faces[2].texture_name);
        }
        if (error(pos.z, 0.5f))
        {
            blue[b++] = get_colors(faces[0].texture_name);
        }
        if (error(pos.z, -0.5f))
        {
            green[g++] = get_colors(faces[1].texture_name);
        }

    }
}


