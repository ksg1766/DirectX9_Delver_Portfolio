#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CItem : public CGameObject
{
protected:
    explicit CItem(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CItem(const CItem& rhs);
    virtual ~CItem();

public:
    ITEMTYPEID  Get_ItemTag() { return m_ItemID; }
    void        Add_ItemCount(_uint _iCount) {
        m_ItemID.iCount += _iCount;
    }

public:
    HRESULT     Ready_Object();
    _int        Update_Object(const _float& fTimeDelta);

protected:
    ITEMTYPEID  m_ItemID;


    // ITEM_TAG m_eItemTag;
    // ITEM_ID m_eItemID;    // 필요하다면 추가
    // MonsterAI Component 추가해서 사용
    // 
// CGameObject로부터 상속
    //map<COMPONENTTAG, CComponent*>	m_mapComponent[ID_END];
    //LPDIRECT3DDEVICE9					m_pGraphicDev;
    //_bool								m_IsDead;

    //OBJECTTAG							m_eObjectTag;
    //CTransform*                       m_pTransform;	// Transform Attached to this GameObject

protected:
    virtual void	Free(void);
};

END