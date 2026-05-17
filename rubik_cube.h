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

class Rubik
{
public:
	std::vector<Cube*> cubes;
	SceneNode* center;
	
	Rubik()
	{
		center = new SceneNode(-1);
		center->traslate(Vector3(0.0, 0.0, 0.0f), true);

		cubes.reserve(27);
		Color pink(255.0f, 0.0f, 255.0f, true);
		
		float size = 0.5;
		
		for (int i = 0; i < 27; i++)
		{
			Cube* cube_to_push = new Cube(size);
			cube_to_push->add_faces();
			
			
			cubes.push_back(cube_to_push);
			SceneNode *to_push = new SceneNode(i, cube_to_push);
			center->add_children(to_push);
		}
		
		
		
		// Move
		float possible_moves[3] = {0.5f, 0.0f, -0.5f};
		
		int cur_idx = 0;
		for (auto &z: possible_moves)
			for (auto &y: possible_moves)
				for (auto &x: possible_moves)
				{
					center->children[cur_idx]->traslate(Vector3(x, y, z), true);
					cur_idx++;
				}
		
	}
	
};

#endif