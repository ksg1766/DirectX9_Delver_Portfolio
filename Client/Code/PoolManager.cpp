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
#include "SpiderRay.h"
//#include "Monk.h"

#include "EffectBubble.h"
#include "EffectBrokenbox.h"
#include "EffectDamage.h"
#include "EffectDamageStar.h"
#include "EffectBlood.h"
#include "EffectWand.h"
#include "EffectExplosion.h"
#include "EffectExplosionChild.h"
#include "EffectTrace.h"
#include "EffectProjectileTrace.h"
#include "EffectTwinkle.h"
#include "EffectPastTrace.h"

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
	if(!m_pPool)
		m_pPool = new ObjectPool;
}

CMonster* CPoolManager::Create_Monster(MONSTERTAG _eMonsterTag, _vec3 _vSpawnPos) // 특정 위치에 소환
{
	CGameObject* pGameObject = m_pPool->GetMonsterPool(_eMonsterTag).front();
	pGameObject->Set_Dead(false);
	pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vSpawnPos;

	if (_eMonsterTag == MONSTERTAG::SPIDER || _eMonsterTag == MONSTERTAG::WORM)
	{
		CGameObject* pRay = nullptr;

		pRay = CSpiderRay::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
		EventManager()->CreateObject(pRay, LAYERTAG::GAMELOGIC);
		static_cast<CSpiderRay*>(pRay)->Set_Host(pGameObject);
	}

	EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

	m_pPool->GetMonsterPool(_eMonsterTag).pop();
	return static_cast<CMonster*>(pGameObject);
}

CTempEffect* CPoolManager::Create_Effect(EFFECTTAG _eEffectTag, _vec3 _vSpawnPos)
{
	CGameObject* pGameObject = m_pPool->GetEffectPool(_eEffectTag).front();
	pGameObject->Set_Dead(false);
	pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vSpawnPos;
	EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
 

	m_pPool->GetEffectPool(_eEffectTag).pop();
	return static_cast<CTempEffect*>(pGameObject);
}

void CPoolManager::Delete_Object(CGameObject* _pGameObject)
{
	switch (_pGameObject->Get_ObjectTag())
	{
	case OBJECTTAG::MONSTER:
		_pGameObject->Set_Dead(true);
		static_cast<CMonster*>(_pGameObject)->Init_Stat();
		m_pPool->GetMonsterPool(static_cast<CMonster*>(_pGameObject)->Get_MonsterTag()).push(static_cast<CMonster*>(_pGameObject));
		//EventManager()->DeleteObject(_pGameObject);
		break;

	case OBJECTTAG::EFFECT:
		_pGameObject->Set_Dead(true);
		//static_cast<CTempEffect*>(_pGameObject)->Init_Data(); // CTempEffect에 virtual PURE로 순수가상함수 하나 만들어서
		//														// 자식 클래스에 오버라이딩하시면 됩니다
		m_pPool->GetEffectPool(static_cast<CTempEffect*>(_pGameObject)->Get_EffectTag()).push(static_cast<CTempEffect*>(_pGameObject));
		break;
	}
}

void CPoolManager::Free()
{
	m_pPool->~ObjectPool();
	Safe_Delete<ObjectPool*>(m_pPool);
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
	CMonster* pMonster = nullptr;
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	for (_uint i = 0; i < (_uint)MONSTERTAG::MONSTER_END; ++i)
	{
		// 가상함수 및 템플릿으로 구현하면 더 좋음...
		for (_uint j = 0; j < iMaxMonster; ++j)
		{
			switch ((MONSTERTAG)i)
			{
			case MONSTERTAG::SPIDER:
				pMonster = CDungeonSpider::Create(pGraphicDev);
				break;
			case MONSTERTAG::WARRIOR:
				pMonster = CDungeonWarrior::Create(pGraphicDev);
				break;
			case MONSTERTAG::BAT:
				pMonster = CBat::Create(pGraphicDev);
				break;
			case MONSTERTAG::WIZARD:
				pMonster = CWizard::Create(pGraphicDev);
				break;
			case MONSTERTAG::ALIEN:
				pMonster = CAlien::Create(pGraphicDev);
				break;
			case MONSTERTAG::SLIME:
				pMonster = CSlime::Create(pGraphicDev);
				break;
			case MONSTERTAG::SKELETON:
				pMonster = CSkeleton::Create(pGraphicDev);
				break;
			case MONSTERTAG::SKULLGHOST:
				pMonster = CSkullGhost::Create(pGraphicDev);
				break;
			case MONSTERTAG::WORM:
				pMonster = CWorm::Create(pGraphicDev);
				break;
			//case MONSTERTAG::MONK:
			//	pGameObject = CMonk::Create(pGraphicDev);
			//	break;
			}
			m_MonsterPool[i].push(pMonster);
		}
	}

	CTempEffect* pEffect = nullptr;
	for (_uint i = 0; i < EFFECTTAG::EFFECT_END; ++i)
	{
		// 가상함수 및 템플릿으로 구현하면 더 좋음...
		for (_uint j = 0; j < iMaxEffect; ++j)
		{
			switch ((EFFECTTAG)i)
			{
			case EFFECTTAG::EFFECT_BUBBLE:
				pEffect = CEffectBubble::Create(pGraphicDev);
				break;
			case EFFECTTAG::EFFECT_BROKENBOX:
				pEffect = CEffectBrokenbox::Create(pGraphicDev);
				break;
			case EFFECTTAG::EFFECT_DAMAGE:
				pEffect = CEffectDamage::Create(pGraphicDev);
				break;
			case EFFECTTAG::EFFECT_DAMAGESTAR:
				pEffect = CEffectDamageStar::Create(pGraphicDev);
				break;
			case EFFECTTAG::EFFECT_BLOOD:
				pEffect = CEffectBlood::Create(pGraphicDev);
				break;
			case EFFECTTAG::EFFECT_WAND:
				pEffect = CEffectWand::Create(pGraphicDev);
				break;
			case EFFECTTAG::EFFECT_EXPLOSION:
				pEffect = CEffectExplosion::Create(pGraphicDev);
				break;
			case EFFECTTAG::EFFECT_EXPLOSIONCHILD:
				pEffect = CEffectExplosionChild::Create(pGraphicDev);
				break;
			case EFFECTTAG::EFFECT_PROJECTILETRACE:
				pEffect = CEffectProjectileTrace::Create(pGraphicDev);
				break;
			case EFFECTTAG::EFFECT_TRACE:
				pEffect = CEffectTrace::Create(pGraphicDev);
				break;
			case EFFECTTAG::EFFECT_TWINKLE:
				pEffect = CEffectTwinkle::Create(pGraphicDev);
				break;
			case EFFECTTAG::EFFECT_PASTTRACE:
				pEffect = CEffectPastTrace::Create(pGraphicDev);
				break;
			}
			m_EffectPool[i].push(pEffect);
		}
	}
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
	int a = 0;		

	for (_uint i = 0; i < EFFECTTAG::EFFECT_END; ++i)
	{
		while (!m_EffectPool[i].empty())
		{
			CTempEffect*& iter = m_EffectPool[i].front();

			_ulong dwRefCnt = 0;
			dwRefCnt = iter->Release();

			if (0 == dwRefCnt)
				iter = NULL;

			m_EffectPool[i].pop();
		}
	}


}
