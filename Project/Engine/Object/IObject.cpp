#include "IObject.h"

ID3D12GraphicsCommandList* IObject::commandList_ = nullptr;
uint32_t IObject::newSerialNumber_ = 0;

void IObject::StaticInitialize(ID3D12GraphicsCommandList* commandList)
{

	commandList_ = commandList;

	newSerialNumber_ = 0;

}

void IObject::ResetNewSerialNumber()
{

	newSerialNumber_ = 0;

}
