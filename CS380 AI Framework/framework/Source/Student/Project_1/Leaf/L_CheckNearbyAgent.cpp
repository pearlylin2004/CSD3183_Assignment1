#include <pch.h>
#include "L_CheckNearbyAgent.h"
#include "Agent/BehaviorAgent.h"

void L_CheckNearbyAgent::on_update(float dt){
    const auto& allAgents = agents->get_all_agents();
    const Vec3 currPos = agent->get_position();

    for (const auto& a : allAgents){
        if (a == agent)
            continue;

        const float distance = Vec3::Distance(currPos, a->get_position());

        if (distance < PROXIMITY_THRESHOLD){
            on_success();
            display_leaf_text();
            return;
        }
    }
    on_failure();
    display_leaf_text();
}
