#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CTriCol;
class CTransform;

END

class CMonster : public Engine::CGameObject
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

private:
	HRESULT		Add_Component(void);

private:
	CTriCol*			m_pBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	_float				m_fSpeed = 5.f;
	
public:
	static CMonster*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

