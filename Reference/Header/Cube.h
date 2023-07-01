#pragma once
#include "GameObject.h"

BEGIN(Engine)
// Static Object
class ENGINE_DLL CCube :
    public CGameObject
{
protected:
    explicit CCube(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CCube(const CCube& rhs);
    virtual ~CCube();

public:
    HRESULT		    Ready_Object();
    _int		    Update_Object(const _float& fTimeDelta);

protected:
    //map<COMPONENTTAG, CComponent*>	m_mapComponent[ID_END];
    //LPDIRECT3DDEVICE9					m_pGraphicDev;
    //_bool								m_IsDead;

    //OBJECTTAG							m_eObjectTag;
    //CTransform*                       m_pTransform;	// Transform Attached to this GameObject

protected:
    virtual void	Free(void);
};

END