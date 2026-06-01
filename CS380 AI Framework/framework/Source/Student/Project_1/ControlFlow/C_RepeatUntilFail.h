#pragma once
#include "BehaviorTrees/BehaviorNode.h"

class C_RepeatUntilFail : public BaseNode<C_RepeatUntilFail>{
protected:
    virtual void on_update(float dt) override;
};
