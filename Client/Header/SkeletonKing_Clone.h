#pragma once

#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;
class CBossAI;
END
class CTerrain;

class CSkeletonKing_Clone:public Engine::CGameObject
{
private:
	explicit	CSkeletonKing_Clone(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CSkeletonKing_Clone(const CSkeletonKing_Clone& rhs);
	virtual		~CSkeletonKing_Clone();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;
public:
	void		ForceHeight(_vec3 _vPos);
	void		Set_Terrain(CTerrain* _pCurrentTerrain) { m_pTerrain = _pCurrentTerrain; }
private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;
	CBillBoard* m_pBillBoard = nullptr;
	CBossAI* m_pBossAI = nullptr;
	CTerrain* m_pTerrain = nullptr;
	_float	m_fFrame = 0;
	_float	m_fSkillCool = 0;
	_bool	m_bSkill;
private:
	HRESULT		Add_Component(void);
	void		Clone_Pattern();

public:
	static CSkeletonKing_Clone* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

