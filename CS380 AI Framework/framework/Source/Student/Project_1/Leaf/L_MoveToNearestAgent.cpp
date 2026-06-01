#include <pch.h>
#include "L_MoveToNearestAgent.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToNearestAgent::on_enter(){
    float shortestDistance = std::numeric_limits<float>::max();
    Vec3 nearestPoint;
    bool targetFound = false;

    const auto& allAgents = agents->get_all_agents();
    const auto& currPos = agent->get_position();

    for (const auto& a : allAgents){
        if (a != agent){
            const auto& agentPos = a->get_position();
            const float distance = Vec3::Distance(currPos, agentPos);
            if (distance < shortestDistance){
                shortestDistance = distance;
                nearestPoint = agentPos;
                targetFound = true;
            }
        }
    }

    if (targetFound){
        targetPoint = nearestPoint;
        BehaviorNode::on_leaf_enter();
    }
    else{
        on_failure();
    }
}

void L_MoveToNearestAgent::on_update(float dt){
    const auto result = agent->move_toward_point(targetPoint, dt);
    if (result) on_success();
    display_leaf_text();
}
