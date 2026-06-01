#pragma once
#include "BehaviorTrees/BehaviorNode.h"

class L_ChaseRabbit : public BaseNode<L_ChaseRabbit>{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
private:
    Vec3 targetPoint;
    static constexpr float CATCH_DISTANCE = 5.0f;
};
