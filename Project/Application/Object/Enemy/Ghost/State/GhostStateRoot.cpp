#include "GhostStateRoot.h"

void GhostStateRoot::Initialize()
{

	ghostStateNo_ = kGhostStateIndexRoot;

}

void GhostStateRoot::Update()
{

	ghostStateNo_ = kGhostStateIndexSideMove;

}
