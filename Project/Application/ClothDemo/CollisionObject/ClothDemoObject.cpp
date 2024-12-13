#include "ClothDemoObject.h"
#include "../../../Engine/3D/Model/ModelDraw.h"

// 次のギズモID
uint32_t ClothDemoObject::nextGuizmoID_ = 0;

void ClothDemoObject::Draw(BaseCamera& camera)
{

    // 出現してないなら描画しない
    if (!exist_) {
        return;
    }

    // 描画引数
    ModelDraw::NormalObjectDesc desc;
    // テクスチャハンドル
    std::vector<UINT> textureHandles;
    textureHandles.push_back(textureHandle_);

    desc.model = model_.get();
    desc.material = material_.get();
    desc.camera = &camera;
    desc.worldTransform = &worldTransform_;
    desc.textureHandles = textureHandles;

    // 描画
    ModelDraw::NormalObjectDraw(desc);

}
