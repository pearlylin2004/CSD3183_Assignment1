#pragma once
#include "BehaviorTrees/BehaviorNode.h"
#include <vector>

class C_RandomSequencer : public BaseNode<C_RandomSequencer>
{
public:
    C_RandomSequencer();
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
private:
    std::vector<size_t> order;
    size_t currentIndex;
};
