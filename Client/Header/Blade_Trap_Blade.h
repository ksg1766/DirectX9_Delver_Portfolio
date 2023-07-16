#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CStateMachine;
END

class CBlade_Trap_Blade :
    public CGameObject
{
private:
	explicit	CBlade_Trap_Blade(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CBlade_Trap_Blade(const CBlade_Trap_Blade& rhs);
	virtual		~CBlade_Trap_Blade();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture[(_uint)STATE::STATE_END] = {};
	CStateMachine* m_pStateMachine = nullptr;

	_vec3			m_vPlayerPos;
	_vec3			m_vDir;

	_float			m_fDistance;
	_float			m_fCooltime;

private:
	HRESULT		Add_Component(void);

public:
	static CBlade_Trap_Blade* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;


};

