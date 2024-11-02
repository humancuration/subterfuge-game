#ifndef ISYSTEM_HPP
#define ISYSTEM_HPP

#include <Godot.hpp>

namespace Systems {

class ISystem {
public:
    virtual ~ISystem() = default;
    virtual void update(float delta_time) = 0;
};

} // namespace Systems

#endif // ISYSTEM_HPP 