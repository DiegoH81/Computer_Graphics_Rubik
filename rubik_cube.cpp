#include "rubik_cube.h"


Color white(255.0f, 255.0f, 255.0f, true);
Color yellow(255.0f, 255.0f, 0.0f, true);

Color red(255.0f, 0.0f, 0.0f, true);
Color orange(255.0f, 128.0f, 0.0f, true);

Color blue(0.0f, 70.0f, 255.0f, true);
Color green(0.0f, 255.0f, 0.0f, true);

Color black(0.0f, 0.0f, 0.0f, true);

bool error(float a, float b){
	return std::abs(a-b) < 0.05f;
}

Rubik::Rubik(){
		center = new SceneNode(1);
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

SceneNode* Rubik::find_layer(float x, float y, float z, bool x_use, bool y_use, bool z_use){
	SceneNode* pivot = new SceneNode(-1,nullptr);
	center->add_children(pivot);
	
	std::vector<SceneNode*> cubes;
	for(auto child :center->children){
		if(child == pivot) continue;
		Point3 centerPos = center->get_center();
		Point3 childPos = child->get_center();
		Point3 relativePos = childPos - centerPos;
		if((!x_use || error(relativePos.x,x)) && (!y_use || error(relativePos.y,y)) && (!z_use || error(relativePos.z,z))){
			cubes.push_back(child);
		}
	}
	
	for(auto cube: cubes){
		for(auto it = center->children.begin(); it != center->children.end(); it++){
			if(*it == cube){
				center->children.erase(it);
				break;
			}
		}
		pivot->add_children(cube);
	}
	
	return pivot;
}


void Rubik::destroy_temp_pivot(SceneNode* pivot){
	for(auto cube : pivot->children){
		cube->private_transform = pivot->public_transform * cube->private_transform;
		cube->public_transform = pivot->public_transform * cube->public_transform;
		center->add_children(cube);
	}

	pivot->children.clear();

	for(auto cube: cubes){
		for(auto it = center->children.begin(); it != center->children.end(); it++){
			if(*it == pivot){
				center->children.erase(it);
				break;
			}
		}
	}

	delete pivot;
}