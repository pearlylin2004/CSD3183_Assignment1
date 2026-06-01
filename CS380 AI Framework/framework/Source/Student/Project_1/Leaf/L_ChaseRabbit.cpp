#include <pch.h>
#include "L_ChaseRabbit.h"
#include "Agent/BehaviorAgent.h"

static bool FindNearestRabbit(Agent* self, Vec3& outPos, float& outDist){
    const auto& allAgents = agents->get_all_agents();
    const Vec3 currPos = self->get_position();
    float best = std::numeric_limits<float>::max();
    bool found = false;

    for (size_t i = 0; i < allAgents.size(); ++i){
        Agent* a = allAgents[i];
        if (a == self) continue;
        if (strcmp(a->get_type(), "Rabbit") != 0) continue;

        const float d = Vec3::Distance(currPos, a->get_position());
        if (d < best){
            best = d;
            outPos = a->get_position();
            outDist = d;
            found = true;
        }
    }
    return found;
}

void L_ChaseRabbit::on_enter(){
    float dist = 0.0f;
    if (FindNearestRabbit(agent, targetPoint, dist))
        BehaviorNode::on_leaf_enter();
    else
        on_failure();
}

void L_ChaseRabbit::on_update(float dt){
    float dist = 0.0f;
    if (!FindNearestRabbit(agent, targetPoint, dist)){
        on_failure();
        display_leaf_text();
        return;
    }

    if (dist < CATCH_DISTANCE){
        on_success();
        display_leaf_text();
        return;
    }

    agent->move_toward_point(targetPoint, dt);
    display_leaf_text();
}
