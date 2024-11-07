#include "ModelManager.h"

ModelManager* ModelManager::GetInstance()
{
    static ModelManager instance;
    return &instance;
}

void ModelManager::Initialize(DirectXCommon* dxCommon)
{

    dxCommon_ = dxCommon;

}

void ModelManager::Finalize()
{

    for (std::vector<std::unique_ptr<Model>>::iterator it = models_.begin();
        it != models_.end(); ++it) {

        it->reset(nullptr);

    }

    models_.clear();

}

Model* ModelManager::GetModel(
    const std::string& directoryPath,
    const std::string& fileName)
{

    Model* model = nullptr;

    // モデルがすでにあるか
    model = FindModel(fileName);

    // なかったので作る
    if (!model) {
        std::unique_ptr<Model> newModel;
        newModel.reset(Model::Create(directoryPath, fileName, dxCommon_));
        models_.push_back(std::move(newModel));
        model = models_.back().get();
    }

    return model;

}

Model* ModelManager::FindModel(const std::string& fileName)
{

    Model* model = nullptr;

    for (std::vector<std::unique_ptr<Model>>::iterator it = models_.begin();
        it != models_.end(); ++it) {

        if (it->get()->GetFileName() == fileName) {
            model = it->get();
            break;
        }

    }

    return model;

}
