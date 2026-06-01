#pragma once
#include "BehaviorTrees/BehaviorNode.h"

class L_FleeFromNearestAgent : public BaseNode<L_FleeFromNearestAgent>{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
private:
    Vec3 fleeTarget;
};
