#include "Outline.h"
#include "../../base/BufferResource.h"
#include "../../base/DxCommon/DirectXCommon.h"


void Outline::Initialize()
{

	outlineDataBuff_ = BufferResource::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), ((sizeof(OutlineData) + 0xff) & ~0xff));
	outlineDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&outlineDataMap_));

	outlineDataMap_->color = { 1.0f,1.0f,1.0f,1.0f };
	outlineDataMap_->width = 1.0f;

}

void Outline::Map(const OutlineData& outlineData)
{

	outlineDataMap_->color = outlineData.color;
	outlineDataMap_->width = outlineData.width;

}
