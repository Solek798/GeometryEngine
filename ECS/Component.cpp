//
// Created by felix on 05.06.20.
//

#include "Component.h"

/*template<typename T>
const uint64_t ecs::Component<T>::ID(((uint64_t)FIRST_COMPONENT) << ecs::ComponentBase::componentCounter);*/


static uint32_t componentID = 0;

uint32_t BaseECSComponent::nextID()
{
    return componentID++;
}
