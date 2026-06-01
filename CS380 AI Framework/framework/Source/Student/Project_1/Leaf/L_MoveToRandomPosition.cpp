#include <pch.h>
#include "L_MoveToRandomPosition.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToRandomPosition::on_enter(){
    targetPoint = RNG::world_position();
	BehaviorNode::on_leaf_enter();
}

void L_MoveToRandomPosition::on_update(float dt){
    const auto result = agent->move_toward_point(targetPoint, dt);

    if (result == true){
        on_success();
    }
    display_leaf_text();
}
