#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CMonster;
class CTempEffect;

END

class CPoolManager :
    public CBase
{
    DECLARE_SINGLETON(CPoolManager)

	//TODO : Make Template
private:
	explicit CPoolManager();
	virtual ~CPoolManager();

public:
	void	Ready_Pool();

	void	Create_Monster(MONSTERTAG _eMonsterTag, _vec3 _vSpawnPos);
	void	Create_Effect(EFFECTTAG _eMonsterTag, _vec3 _vSpawnPos);

	void	Delete_Object(CGameObject* _pGameObject);

	virtual void Free() override;

private:
	class ObjectPool
		: public CBase
	{
	public:
		ObjectPool();
		~ObjectPool();

		void Ready_Pool();

		queue<CMonster*>& GetMonsterPool(MONSTERTAG _eMonsterTag) { return m_MonsterPool[(_uint)_eMonsterTag]; }
		queue<CTempEffect*>& GetEffectPool(EFFECTTAG _eEffectTag) { return m_EffectPool[(_uint)_eEffectTag]; }
		
		virtual void Free() override;

	private:
		static const _uint iMaxMonster = 31;
		static const _uint iMaxEffect = 127;
		queue<CMonster*> m_MonsterPool[(_uint)MONSTERTAG::MONSTER_END];
		queue<CTempEffect*> m_EffectPool[(_uint)EFFECTTAG::EFFECT_END];
	};

private:
	ObjectPool* m_pPool;
};