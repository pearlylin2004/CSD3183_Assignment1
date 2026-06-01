#include <pch.h>
#include "L_CheckFairyNearby.h"
#include "Agent/BehaviorAgent.h"

void L_CheckFairyNearby::on_update(float dt){
    const Vec3 currPos = agent->get_position();
    const auto& allAgents = agents->get_all_agents();

    for (const auto& a : allAgents){
        if (a == agent) continue;
        if (strcmp(a->get_type(), "Fairy") != 0) continue;
        if (Vec3::Distance(currPos, a->get_position()) < DETECTION_RANGE){
            on_success();
            display_leaf_text();
            return;
        }
    }
    on_failure();
    display_leaf_text();
}
