#include "DataBank.h"

DataBank* DataBank::instance_ = nullptr;

void DataBank::CreateInstance(void)
{
}

DataBank& DataBank::GetInstance(void)
{
	return *instance_;
}

void DataBank::Init(void)
{
}

void DataBank::Update(void)
{
}

void DataBank::Draw(void)
{
}

void DataBank::Destroy(void)
{
}
