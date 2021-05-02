#ifndef POSITION_COMPONENT_H
#define POSITION_COMPONENT_H

#include "EntityComponentSystem.h"

class PositionComponent : public Component {
public:
    PositionComponent(float x, float y, float z);
    virtual ~PositionComponent() = default;
    
    virtual void Init() override;
    virtual void Draw() override;
    virtual void Update() override;

    inline float GetX() const { return x; }
    inline float GetY() const { return y; }
    inline float GetZ() const { return z; }
private:
    float x, y, z;
};


#endif //!POSITION_COMPONENT_H