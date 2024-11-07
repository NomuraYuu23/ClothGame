#pragma once
#include "ColliderParentObject.h"
#include "../../Engine/Object/MeshObject.h"

class ColliderParentIdentification
{

public:

	/// <summary>
	/// ColliderParentObjectをメッシュオブジェクトにする
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <returns></returns>
	static MeshObject* Execution(ColliderParentObject colliderPartner);

};

