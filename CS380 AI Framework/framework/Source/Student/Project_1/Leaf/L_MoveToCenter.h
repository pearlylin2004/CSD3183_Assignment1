#pragma once
#include "BehaviorTrees/BehaviorNode.h"

class L_MoveToCenter : public BaseNode<L_MoveToCenter>{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
private:
    Vec3 centerPoint;
};
