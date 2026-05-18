#ifndef RUBIK_CUBE_H
#define RUBIK_CUBE_H

#include <vector>

#include "shape.h"
#include "shader_list.h"
#include "shape.h"
#include "matrix.h"
#include "scene.h"
#include "vertex.h"
#include "texture_list.h"
#include "color.h"

class Rubik
{
public:
	std::vector<Cube*> cubes;
	SceneNode* center;
	
	Rubik();
	
};

#endif