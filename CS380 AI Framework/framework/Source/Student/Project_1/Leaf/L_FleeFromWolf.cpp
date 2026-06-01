#include <pch.h>
#include "L_FleeFromWolf.h"
#include "Agent/BehaviorAgent.h"

void L_FleeFromWolf::on_enter(){
    float shortestDistance = std::numeric_limits<float>::max();
    Vec3 wolfPos;
    bool found = false;

    const auto& allAgents = agents->get_all_agents();
    const Vec3 currPos = agent->get_position();

    for (const auto& a : allAgents){
        if (a == agent) continue;
        if (strcmp(a->get_type(), "Wolf") != 0) continue;

        const float distance = Vec3::Distance(currPos, a->get_position());
        if (distance < shortestDistance){
            shortestDistance = distance;
            wolfPos = a->get_position();
            found = true;
        }
    }

    if (!found){
        on_failure();
        return;
    }

    Vec3 awayDir = currPos - wolfPos;
    if (awayDir.LengthSquared() < 0.0001f)
        awayDir = Vec3(1.0f, 0.0f, 0.0f);

    awayDir.Normalize();
    fleeTarget = currPos + awayDir * FLEE_DISTANCE;

    const float mapSize = terrain->mapSizeInWorld;
    fleeTarget.x = std::max(0.0f, std::min(fleeTarget.x, mapSize));
    fleeTarget.z = std::max(0.0f, std::min(fleeTarget.z, mapSize));

    BehaviorNode::on_leaf_enter();
}

void L_FleeFromWolf::on_update(float dt){
    if (agent->move_toward_point(fleeTarget, dt))
        on_success();
    display_leaf_text();
}
