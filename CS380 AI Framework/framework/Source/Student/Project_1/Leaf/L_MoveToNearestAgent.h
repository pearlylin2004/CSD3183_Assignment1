#pragma once
#include "BehaviorTrees/BehaviorNode.h"

class L_MoveToNearestAgent : public BaseNode<L_MoveToNearestAgent>{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
private:
    Vec3 targetPoint;
};
