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
	for (_uint i = 0; i < (_uint)MONSTERTAG::MONSTER_END; ++i)
	{
		CGameObject* pGameObject = nullptr;

		switch ((MONSTERTAG)i)
		{
		case MONSTERTAG::SPIDER:

			break;
		case MONSTERTAG::WARRIOR:
			break;
		case MONSTERTAG::BAT:
			break;
		case MONSTERTAG::WIZARD:
			break;
		case MONSTERTAG::ALIEN:
			break;
		case MONSTERTAG::SLIME:
			break;
		case MONSTERTAG::SKELETON:
			break;
		case MONSTERTAG::SKULLGHOST:
			break;
		case MONSTERTAG::WORM:
			break;
		}
		
		for (_uint j = 0; j < iMaxMonster; ++j)
		{

			m_MonsterPool[i];
		}
	}
}

void CPoolManager::ObjectPool::Free()
{
}
