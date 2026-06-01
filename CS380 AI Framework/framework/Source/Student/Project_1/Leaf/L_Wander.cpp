#include <pch.h>
#include "L_Wander.h"
#include "Agent/BehaviorAgent.h"

static Vec3 ClampToTerrain(const Vec3& pos, float mapSize){
    Vec3 p = pos;
    p.x = std::max(0.0f, std::min(p.x, mapSize));
    p.z = std::max(0.0f, std::min(p.z, mapSize));

    return p;
}

//to avoid crowding
static Vec3 AvoidCrowding(BehaviorAgent* self, Vec3 target, float strength = 0.2f){
    const auto& allAgents = agents->get_all_agents();
    Vec3 pos = self->get_position();
    Vec3 push(0.0f, 0.0f, 0.0f);

    for (auto& a : allAgents){
        if (a == self) continue;

        float dist = Vec3::Distance(pos, a->get_position());

        if (dist < 5.0f){
            push += (pos - a->get_position());
        }
    }
    return target + push * strength;
}

void L_Wander::on_enter(){
    const auto& currPos = agent->get_position();
    float offsetX = RNG::range(-WANDER_RADIUS, WANDER_RADIUS);
    float offsetZ = RNG::range(-WANDER_RADIUS, WANDER_RADIUS);

    wanderTarget = Vec3(
        currPos.x + offsetX,
        currPos.y,
        currPos.z + offsetZ
    );

    wanderTarget = ClampToTerrain(wanderTarget, terrain->mapSizeInWorld);
    BehaviorNode::on_leaf_enter();
}

//this is to update every moment of frame
void L_Wander::on_update(float dt){
    const float mapSize = terrain->mapSizeInWorld;

    if (agent->move_toward_point(wanderTarget, dt)){
        const auto& currPos = agent->get_position();
        float offsetX = RNG::range(-WANDER_RADIUS, WANDER_RADIUS);
        float offsetZ = RNG::range(-WANDER_RADIUS, WANDER_RADIUS);

        wanderTarget = Vec3(
            currPos.x + offsetX,
            currPos.y,
            currPos.z + offsetZ
        );
        wanderTarget = AvoidCrowding(agent, wanderTarget);
        wanderTarget = ClampToTerrain(wanderTarget, mapSize);
    }
    display_leaf_text();
}