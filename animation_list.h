#ifndef ANIMATION_LIST_H
#define ANIMATION_LIST_H

#define CAMERA_ONLY -1

#include <queue>
#include <string>


#include "camera.h"
#include "shape.h"
#include "utils.h"
#include "scene.h"


class AnimationInfo
{
public:
    int id;
    float move;
    std::string type;
    std::string visibility;

    AnimationInfo(const int& in_id, const float& in_move, const std::string& in_type, const std::string& in_visibility);
};

class AnimationBlock
{
public:
    std::vector<AnimationInfo> transforms;
    float remaining_time;

    AnimationBlock(const std::vector<AnimationInfo>& in_transforms,const float& in_remaining_time);
};

class AnimationList
{
public:

    void add_animation(const std::vector<AnimationInfo>& animation_sequence, const float& seconds);

    void process_animations(const std::vector<SceneNode*>& in_info, const float& in_delta_time);

    void process_animations_camera(Camera& in_camera, std::vector<SceneNode*>& in_info, const float& in_delta_time);

    std::queue<AnimationBlock> animation_queue;
};

#endif