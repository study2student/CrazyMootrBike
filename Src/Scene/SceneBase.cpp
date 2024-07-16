#include "../Manager/ResourceManager.h"
#include "../Object/Score.h"
#include "../Object/DataSave.h"
#include "SceneBase.h"

SceneBase::SceneBase(void) : resMng_(ResourceManager::GetInstance()) , score_(Score::GetInstance()), data_(DataSave::GetInstance())
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init(void)
{
}

void SceneBase::Update(void)
{
}

void SceneBase::Draw(void)
{
}
