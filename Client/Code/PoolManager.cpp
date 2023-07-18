#include "Export_Function.h"
#include "PoolManager.h"

#include "DungeonWarrior.h"
#include "DungeonSpider.h"
#include "Wizard.h"
#include "Bat.h"
#include "Alien.h"
#include "Slime.h"
#include "Skeleton.h"
#include "Worm.h"
#include "SkullGhost.h"
//#include "Monk.h"

#include "EffectBubble.h"
#include "EffectBrokenbox.h"
#include "EffectDamage.h"
#include "EffectDamageStar.h"
#include "EffectBlood.h"
#include "EffectWand.h"
#include "EffectExplosion.h"
#include "EffectProjectileTrace.h"

IMPLEMENT_SINGLETON(CPoolManager)

CPoolManager::CPoolManager()
	: m_pPool(nullptr)
{
}

CPoolManager::~CPoolManager()
{
	Free();
}

void CPoolManager::Ready_Pool()
{
	m_pPool = new ObjectPool;
}

void CPoolManager::Create_Monster(MONSTERTAG _eMonsterTag, _vec3 _vSpawnPos) // 특정 위치에 소환
{
	CGameObject* pGameObject = m_pPool->GetMonsterPool(_eMonsterTag).front();
	pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vSpawnPos;
	EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

	m_pPool->GetMonsterPool(_eMonsterTag).pop();
}
/*
void CPoolManager::Create_Effect(EFFECTTAG _eEffectTag, _vec3 _vSpawnPos)
{
	CGameObject* pGameObject = m_pPool->GetEffectPool(_eEffectTag).front();
	pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vSpawnPos;
	EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
 
	m_pPool->GetEffectPool(_eEffectTag).pop();
}
*/
void CPoolManager::Delete_Object(CGameObject* _pGameObject)
{
	switch (_pGameObject->Get_ObjectTag())
	{
	case OBJECTTAG::MONSTER:
		_pGameObject->Set_Dead(false);
		static_cast<CMonster*>(_pGameObject)->Init_Stat();

		m_pPool->GetMonsterPool(static_cast<CMonster*>(_pGameObject)->Get_MonsterTag()).push(static_cast<CMonster*>(_pGameObject));
		EventManager()->DeleteObject(_pGameObject);
		break;

	case OBJECTTAG::EFFECT:
		_pGameObject->Set_Dead(false);
		//static_cast<CTempEffect*>(_pGameObject)->Init_Data(); // CTempEffect에 virtual PURE로 순수가상함수 하나 만들어서
		//														// 자식 클래스에 오버라이딩하시면 됩니다
		//m_pPool->GetEffectPool(static_cast<CEffect*>(_pGameObject)->Get_EffectTag()).push(static_cast<CEffect*>(_pGameObject));
		EventManager()->DeleteObject(_pGameObject);
		break;
	}
}

void CPoolManager::Free()
{
	m_pPool->~ObjectPool();
}

CPoolManager::ObjectPool::ObjectPool()
{
	Ready_Pool();
}

CPoolManager::ObjectPool::~ObjectPool()
{
	Free();
}

void CPoolManager::ObjectPool::Ready_Pool()
{
	CMonster* pGameObject = nullptr;
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	for (_uint i = 0; i < (_uint)MONSTERTAG::MONSTER_END; ++i)
	{
		// 가상함수 및 템플릿으로 구현하면 더 좋음...
		for (_uint j = 0; j < iMaxMonster; ++j)
		{
			switch ((MONSTERTAG)i)
			{
			case MONSTERTAG::SPIDER:
				pGameObject = CDungeonSpider::Create(pGraphicDev);
				break;
			case MONSTERTAG::WARRIOR:
				pGameObject = CDungeonWarrior::Create(pGraphicDev);
				break;
			case MONSTERTAG::BAT:
				pGameObject = CBat::Create(pGraphicDev);
				break;
			case MONSTERTAG::WIZARD:
				pGameObject = CWizard::Create(pGraphicDev);
				break;
			case MONSTERTAG::ALIEN:
				pGameObject = CAlien::Create(pGraphicDev);
				break;
			case MONSTERTAG::SLIME:
				pGameObject = CSlime::Create(pGraphicDev);
				break;
			case MONSTERTAG::SKELETON:
				pGameObject = CSkeleton::Create(pGraphicDev);
				break;
			case MONSTERTAG::SKULLGHOST:
				pGameObject = CSkullGhost::Create(pGraphicDev);
				break;
			case MONSTERTAG::WORM:
				pGameObject = CWorm::Create(pGraphicDev);
				break;
			//case MONSTERTAG::MONK:
			//	pGameObject = CMonk::Create(pGraphicDev);
			//	break;
			}
			m_MonsterPool[i].push(pGameObject);
		}
	}

	//for (_uint i = 0; i < (_uint)EFFECTTAG::EFFECT_END; ++i)
	//{
	//	// 가상함수 및 템플릿으로 구현하면 더 좋음...
	//	for (_uint j = 0; j < iMaxEffect; ++j)
	//	{
	//		switch ((EFFECTTAG)i)
	//		{
	//		case EFFECTTAG:::
	//			pGameObject = CDungeonSpider::Create(pGraphicDev);
	//			break;
	//		case EFFECTTAG:::
	//			pGameObject = CDungeonWarrior::Create(pGraphicDev);
	//			break;
	//		case EFFECTTAG:::
	//			pGameObject = CBat::Create(pGraphicDev);
	//			break;
	//		case EFFECTTAG:::
	//			pGameObject = CWizard::Create(pGraphicDev);
	//			break;
	//		case EFFECTTAG:::
	//			pGameObject = CAlien::Create(pGraphicDev);
	//			break;
	//		case EFFECTTAG:::
	//			pGameObject = CSlime::Create(pGraphicDev);
	//			break;
	//		case EFFECTTAG:::
	//			pGameObject = CSkeleton::Create(pGraphicDev);
	//			break;
	//		case EFFECTTAG:::
	//			pGameObject = CSkullGhost::Create(pGraphicDev);
	//			break;
	//		case EFFECTTAG:::
	//			pGameObject = CWorm::Create(pGraphicDev);
	//			break;
	//		//case EFFECTTAG:::
	//		//	pGameObject = CMonk::Create(pGraphicDev);
	//		//	break;
	//		}
	//		m_EffectPool[i].push(pGameObject);
	//	}
	//}
}

void CPoolManager::ObjectPool::Free()
{
	for (_uint i = 0; i < (_uint)MONSTERTAG::MONSTER_END; ++i)
	{
		while (!m_MonsterPool[i].empty())
		{
			CMonster*& iter = m_MonsterPool[i].front();

			_ulong dwRefCnt = 0;
			dwRefCnt = iter->Release();

			if (0 == dwRefCnt)
				iter = NULL;

			m_MonsterPool[i].pop();
		}
	}

	/*for (_uint i = 0; i < (_uint)EFFECTTAG::EFFECT_END; ++i)
	{
		CMonster*& iter = m_EffectPool[i].front();
		while (!m_EffectPool[i].empty())
		{
			_ulong dwRefCnt = 0;
			dwRefCnt = iter->Release();

			if (0 == dwRefCnt)
				iter = NULL;

			m_EffectPool[i].pop();
			iter = m_EffectPool[i].front();
		}
	}*/
}
