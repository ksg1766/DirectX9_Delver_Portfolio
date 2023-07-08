#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CCollider;
class ENGINE_DLL CMonster : public CGameObject
{
protected:
    explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CMonster(const CMonster& rhs);
    virtual ~CMonster();

public:
    HRESULT     Ready_Object();
    _int        Update_Object(const _float& fTimeDelta);

public:
    virtual void		OnCollisionEnter(CCollider* _pOther);
    virtual void		OnCollisionStay(CCollider* _pOther);
    virtual void		OnCollisionExit(CCollider* _pOther);

protected:
    // MONSTER_ID m_eMonserID;
    // MonsterAI Component 추가해서 사용

// CGameObject로부터 상속
    //map<COMPONENTTAG, CComponent*>	m_mapComponent[ID_END];
    //LPDIRECT3DDEVICE9				  m_pGraphicDev;
    //_bool								m_IsDead;

   // OBJECTTAG						  m_eObjectTag;
    //CTransform*                       m_pTransform;	// Transform Attached to this GameObject

    

protected:
    virtual void	Free(void);
};

END