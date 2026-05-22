#ifndef RUBIK_CUBE_H
#define RUBIK_CUBE_H

#include <vector>

#include "shape.h"
#include "shader_list.h"
#include "matrix.h"
#include "scene.h"
#include "vertex.h"
#include "texture_list.h"
#include "color.h"


#include <algorithm>

class Rubik
{
public:
	std::vector<Cube*> cubes;
	SceneNode* center;
	TextureList textures;
	char white[9], yellow[9], blue[9], red[9], green[9], orange[9];

	Rubik();
	SceneNode* find_layer(float x, float y, float z, bool x_use = true, bool y_use = true, bool z_use = true);
	void destroy_temp_pivot(SceneNode* pivot);

	void draw(ShaderList& shaders);
	char get_colors(std::string color);
	void cube_state();
};

#endif