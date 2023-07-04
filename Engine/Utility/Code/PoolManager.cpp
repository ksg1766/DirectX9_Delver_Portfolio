#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CPoolManager)

CPoolManager::CPoolManager()
{
}

CPoolManager::~CPoolManager()
{
}

void CPoolManager::Ready_Pool()
{
}

void CPoolManager::TakeOut_Pool(_vec3 _vSpawnPos)
{
}

void CPoolManager::Return_Pool(CGameObject* _pGameObject)
{
}

void CPoolManager::Free()
{
}

CPoolManager::ObjectPool::ObjectPool()
{
}

CPoolManager::ObjectPool::~ObjectPool()
{
}

void CPoolManager::ObjectPool::Ready_Pool()
{
}

void CPoolManager::ObjectPool::Free()
{
}
