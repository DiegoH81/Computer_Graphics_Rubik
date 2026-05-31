#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "animation_list.h"

AnimationInfo::AnimationInfo(const int& in_id, const float& in_move, const std::string& in_type, const std::string& in_visibility):
    id(in_id), move(in_move), type(in_type), visibility(in_visibility)
{}

AnimationBlock::AnimationBlock(const std::vector<AnimationInfo>& in_transforms,
                const float& in_remaining_time):
    transforms(in_transforms), remaining_time(in_remaining_time)
    {}

void AnimationList::add_animation(const std::vector<AnimationInfo>& animation_sequence, const float& seconds)
{
    std::vector<AnimationInfo> transformations;
    for (auto &a: animation_sequence)
    {
        float step = a.move / seconds;
        transformations.push_back(AnimationInfo(a.id, step, a.type, a.visibility));
    }

    animation_queue.push(AnimationBlock(transformations, seconds));
}

void AnimationList::process_animations(const std::vector<SceneNode*>& in_info, const float& in_delta_time)
{
    if (animation_queue.empty()) return;

    auto &top = animation_queue.front();
    float actual_delta = std::min(in_delta_time, top.remaining_time);


    for (auto& t : top.transforms)
    {
        bool is_public = (t.visibility == "PUBLIC")? true : false;
        for (auto& node : in_info)
        {
            if (node->id != t.id && t.id != ALL_IDs)
                continue;

            float move = t.move * actual_delta;
            auto& type = t.type;

            if (type == "MOVE_X")
                node->traslate(Vector3(move, 0, 0), is_public);
            else if (type == "MOVE_Y")
                node->traslate(Vector3(0, move, 0), is_public);
            else if (type == "MOVE_Z")
                node->traslate(Vector3(0, 0, move), is_public);
            else if (type == "ROTATE_C_X")
                node->rotate_x_global(move, is_public);
            else if (type == "ROTATE_C_Y")
                node->rotate_y_global(move, is_public);
            else if (type == "ROTATE_C_Z")
                node->rotate_z_global(move, is_public);
            else if (type == "ROTATE_X")
                node->rotate_x_local(move, is_public);
            else if (type == "ROTATE_Y")
                node->rotate_y_local(move, is_public);
            else if (type == "ROTATE_Z")
                node->rotate_z_local(move, is_public);
            else if (type == "SCALE_X")
                node->scale(Vector3(1.0f + move, 1.0f, 1.0f), is_public);
            else if (type == "SCALE_Y")
                node->scale(Vector3(1.0f, 1.0f + move, 1.0f), is_public);
            else if (type == "SCALE_Z")
                node->scale(Vector3(1.0f, 1.0f, 1.0f + move), is_public);
        }
    }

    top.remaining_time -= in_delta_time;
    if (top.remaining_time <= 0)
        animation_queue.pop();
}

void AnimationList::process_animations_camera(Camera& in_camera, std::vector<SceneNode*>& in_info, const float& in_delta_time)
{
    if (animation_queue.empty())
        return;

    auto &top = animation_queue.front();
    auto &transforms = top.transforms;
    

    for (auto& t: transforms)
    {
        auto &type = t.type;
        float move = t.move * in_delta_time;

        if (type == "MOVE_X")
            in_camera.traslate(Vector3(move, 0.0f, 0.0f));
        else if (type == "MOVE_Y")
            in_camera.traslate(Vector3(0.0f, move, 0.0f));
        else if (type == "MOVE_Z")
            in_camera.traslate(Vector3(0.0f, 0.0f, move));
        else if (type == "ROTATE_X")
            in_camera.rotate_x(move);
        else if (type == "ROTATE_Y")
            in_camera.rotate_y(move);
        else if (type == "ROTATE_Z")
            in_camera.rotate_z(move);

        if (t.id == ALL_IDs)
            continue;

        for (auto &node : in_info)
        {
            if (node->id != t.id)
                continue;
            
            if (type == "FOLLOW")
                in_camera.set_objective(node->get_center());
            else if (type == "ORBIT_X")
            {
                in_camera.set_objective(node->get_center());
                in_camera.orbit_x(move);
            }
            else if (type == "ORBIT_Y")
            {
                in_camera.set_objective(node->get_center());
                in_camera.orbit_y(move);
            }
            else if (type == "ORBIT_Z")
            {
                in_camera.set_objective(node->get_center());
                in_camera.orbit_z(move);
            }

            break;
        }
    }

    top.remaining_time -= in_delta_time;
    if (top.remaining_time <= 0)
        animation_queue.pop();
}

std::queue<AnimationBlock> animation_queue;