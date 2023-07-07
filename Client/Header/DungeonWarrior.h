#pragma once

#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CWarriorAI;
class CTexture;

END

class CTerrain;

class CDungeonWarrior : public CMonster
{
	enum class MonsterState
	{
		Idle,
		Run,
		Roaming
		
	};


private:
	explicit CDungeonWarrior(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDungeonWarrior(const CDungeonWarrior& rhs);
	virtual ~CDungeonWarrior();

public:
	virtual HRESULT	Ready_Object() override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object() override;
	virtual void	Render_Object() override;

public:
	void		ForceHeight(_vec3 _vPos);
	void		Set_Terrain(CTerrain* _pCurrentTerrain) { m_pTerrain = _pCurrentTerrain; }

	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

private:
	HRESULT	Add_Component();

private:
	CRcTex*			m_pBuffer = nullptr;
	CWarriorAI*		m_pAI = nullptr;
	CTexture*		m_pTexture = nullptr;
	CTerrain*		m_pTerrain = nullptr;

	_float			m_fFrame;

	MonsterState	m_eMonster;


public:
	static CDungeonWarrior* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

