#include <pch.h>
#include "L_MoveToFurthestAgent.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToFurthestAgent::on_enter(){
    float longestDistance = std::numeric_limits<float>().min();
    Vec3 furthestPoint;
    bool targetFound = false;

    const auto &allAgents = agents->get_all_agents();
    const auto &currPos = agent->get_position();

    for (const auto & a : allAgents){
        if (a != agent){
            const auto &agentPos = a->get_position();
            const float distance = Vec3::Distance(currPos, agentPos);

            if (distance > longestDistance){
                longestDistance = distance;
                furthestPoint = agentPos;
                targetFound = true;
            }
        }
    }

    if (targetFound == true){
        targetPoint = furthestPoint;
		BehaviorNode::on_leaf_enter();
    }
    else {
        on_failure();
    }
}

void L_MoveToFurthestAgent::on_update(float dt){
    const auto result = agent->move_toward_point(targetPoint, dt);

    if (result == true){
        on_success();
    }
    display_leaf_text();
}
