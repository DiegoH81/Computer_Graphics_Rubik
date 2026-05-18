#include "rubik_cube.h"


Color white(255.0f, 255.0f, 255.0f, true);
Color yellow(255.0f, 255.0f, 0.0f, true);

Color red(255.0f, 0.0f, 0.0f, true);
Color orange(255.0f, 128.0f, 0.0f, true);

Color blue(0.0f, 70.0f, 255.0f, true);
Color green(0.0f, 255.0f, 0.0f, true);

Color black(0.0f, 0.0f, 0.0f, true);

Rubik::Rubik(){
	center = new SceneNode(-1);
		center->traslate(Vector3(0.0, 0.0, 0.0f), true);

		cubes.reserve(27);
		Color pink(255.0f, 0.0f, 255.0f, true);
		
		float size = 0.5;
		
		for (int i = 0; i < 27; i++)
		{
			Cube* cube_to_push = new Cube(size);
			cube_to_push->add_faces();
			cube_to_push->add_edges(&black);
			cube_to_push->set_edge_color(ALL_IDs, &black);
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
					
					Shape* cube = center->children[cur_idx]->shape;
					center->children[cur_idx]->traslate(Vector3(x, y, z), true);
					
					if (x == 0.5f)
						cube->set_face_color(3, &red);

					if (x == -0.5f)
						cube->set_face_color(2, &orange);

					if (y == 0.5f)
						cube->set_face_color(4, &white);

					if (y == -0.5f)
						cube->set_face_color(5, &yellow);

					if (z == 0.5f)
						cube->set_face_color(0, &blue);

					if (z == -0.5f)
						cube->set_face_color(1, &green);
					
					cur_idx++;
				}
		
}