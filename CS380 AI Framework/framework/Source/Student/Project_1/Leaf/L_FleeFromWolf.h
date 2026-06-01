#pragma once
#include "BehaviorTrees/BehaviorNode.h"

class L_FleeFromWolf : public BaseNode<L_FleeFromWolf>{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
private:
    Vec3 fleeTarget;
    static constexpr float FLEE_DISTANCE = 60.0f;
};
