#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "DataSave.h"

DataSave* DataSave::instance_ = nullptr;

void DataSave::CreateInstance(void)
{
    if (instance_ == nullptr)
    {
        instance_ = new DataSave();
    }
    instance_->Init();
}

DataSave& DataSave::GetInstance(void)
{
    return *instance_;
}

void DataSave::Init(void)
{
    data_ = Data();
}

void DataSave::SetData(DataType dataType, int num)
{
    switch (dataType)
    {
    case DataSave::DataType::PLAYER_NUM:
        data_.playerNum_ = num;
        break;
    }
}

const DataSave::Data& DataSave::GetData(void) const
{
    return data_;
}

void DataSave::ResetData(void)
{
    data_ = {};
}
