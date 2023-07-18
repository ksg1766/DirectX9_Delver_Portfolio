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
    ITEMTYPEID  Get_ItemTag()                   { return m_ItemID; }
    void        Add_ItemCount(_uint _iCount)    { m_ItemID.iCount += _iCount; }
    void        Remove_ItemCount(_uint _iCount) { m_ItemID.iCount -= _iCount; }

    _bool       Get_WorldItem()                 { return m_bWorldItem; }
    void        Set_WorldItem(_bool _WorldItem) { m_bWorldItem = _WorldItem; }

    _int        Get_HitCount()                  { return m_iHitCount; }
    void        Set_HitCount(_int iHit)         { m_iHitCount = iHit; }

    CBasicStat* Get_ItemStat()                  { return m_pBasicStat; }

public:
    HRESULT     Ready_Object();
    _int        Update_Object(const _float& fTimeDelta);

protected:
    ITEMTYPEID  m_ItemID;
    _bool       m_bWorldItem;
    CBasicStat* m_pBasicStat = nullptr;

    _int        m_iHitCount;

#pragma region ksg

public:
    void        Set_Owner(CGameObject* _pOwner) { m_pOwner = _pOwner; }
    CGameObject* Get_Owner() { return m_pOwner; }

private:
    CGameObject* m_pOwner = nullptr;

#pragma endregion ksg

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