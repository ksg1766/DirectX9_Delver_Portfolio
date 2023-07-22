#include "..\Header\UIShop.h"
#include "Export_Function.h"
#include "ShopItem.h"

CUIShop::CUIShop(LPDIRECT3DDEVICE9 pGraphicDev)
    : CTempUI(pGraphicDev)
{
}

CUIShop::~CUIShop()
{
}

HRESULT CUIShop::Ready_Object()
{
    m_eObjectTag = OBJECTTAG::UI;
    FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL);

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransform->m_vInfo[INFO_POS].x = WINCX * 0.5f;
    m_pTransform->m_vInfo[INFO_POS].y = WINCY * 0.5f;

    m_pTransform->m_vLocalScale.x = 420;
    m_pTransform->m_vLocalScale.y = 320;

    WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y,
        m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

    for (_uint i = 0; i < ITEMID::ITEMID_END; ++i)
    {

        CTempUI* pUI = CShopItem::Create(m_pGraphicDev);
        dynamic_cast<CShopItem*>(pUI)->Set_ItemTag((ITEMID)i);
        dynamic_cast<CShopItem*>(pUI)->Set_FontTag((ITEMID)i);

        vecShopItem.push_back(pUI);
    }

    return S_OK;
}

_int CUIShop::Update_Object(const _float& fTimeDelta)
{
    if (m_IsDead)
        return 0;

    for (auto iter : vecShopItem)
        iter->Update_Object(fTimeDelta);

    _int iExit = CTempUI::Update_Object(fTimeDelta);


    return iExit;
}

void CUIShop::LateUpdate_Object()
{
    if (m_IsDead)
        return;

    for (auto iter : vecShopItem)
        iter->LateUpdate_Object();

    CTempUI::LateUpdate_Object();
}

void CUIShop::Render_Object()
{
    if (m_IsDead)
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

    m_pTextureCom->Render_Texture(0);
    m_pBufferCom->Render_Buffer();

    for (auto iter : vecShopItem)
        iter->Render_Object();

}

HRESULT CUIShop::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTexBf"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

    pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_ShopUI"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

    for (int i = 0; i < ID_END; ++i)
        for (auto& iter : m_mapComponent[i])
            iter.second->Init_Property(this);

    return S_OK;
}

CUIShop* CUIShop::Create(LPDIRECT3DDEVICE9 pGrahpicDev)
{
    CUIShop* pInstance = new CUIShop(pGrahpicDev);

    if (FAILED(pInstance->Ready_Object()))
    {
        Safe_Release<CUIShop*>(pInstance);

        MSG_BOX("Create Shop Failed");
        return nullptr;
    }

    return pInstance;
}

void CUIShop::Free()
{
    __super::Free();

    for_each(vecShopItem.begin(), vecShopItem.end(), CDeleteObj());
    vecShopItem.clear();
}
