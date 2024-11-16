#include "ColliderParentIdentification.h"

#include "../Object/Player/Player.h"
#include "../Object/Ground/Ground.h"

MeshObject* ColliderParentIdentification::Execution(ColliderParentObject colliderPartner)
{

    MeshObject* result = nullptr;

    if (std::holds_alternative<Player*>(colliderPartner)) {
        result = std::get<Player*>(colliderPartner);
    }
    else if (std::holds_alternative<Ground*>(colliderPartner)) {
        result = std::get<Ground*>(colliderPartner);
    }

    return result;

}
