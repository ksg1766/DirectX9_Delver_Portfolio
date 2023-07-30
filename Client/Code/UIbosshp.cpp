#include "stdafx.h"
#include "..\Header\UIbosshp.h"
#include "Player.h"

CUIbosshp::CUIbosshp(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}


CUIbosshp::~CUIbosshp()
{

}

HRESULT CUIbosshp::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // 초기화

	m_pNumberTextureCom = nullptr;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = 1044.f;
	m_pTransform->m_vInfo[INFO_POS].y = 685.f;
	m_pTransform->m_vLocalScale.x = 400.f;
	m_pTransform->m_vLocalScale.y = 50.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

	m_fMaxHp = 12;
	m_fCurrentHp = 12;
	Update_NumverUI();

	return S_OK;
}

_int CUIbosshp::Update_Object(const _float& fTimeDelta)
{
	if (m_IsDead)
		return 0;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUIbosshp::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();

	// 보스 hp 가져와서 할당
	//CGameObject* pObject = nullptr;// SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front();
	//if (pObject != nullptr)
	//{
	//	//CPlayer& rPlayer = *dynamic_cast<CPlayer*>(pObject);

	//	//m_iMaxHp = rPlayer.Get_Stat()->Get_Stat()->fMaxHP;
	//	//m_iCurrentHp = rPlayer.Get_Stat()->Get_Stat()->fHP;

	//	Update_NumverUI();
	//}
}

void CUIbosshp::Render_Object()
{
	if (m_IsDead)
		return;

	// 내부 이미지
	m_pTransform->m_vInfo[INFO_POS].x = m_fPosition;
	m_pTransform->m_vInfo[INFO_POS].y = 685.f;
	m_pTransform->m_vLocalScale.x = 400.f - m_flength;
	m_pTransform->m_vLocalScale.y = 50.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(1);
	m_pBufferCom->Render_Buffer();

	// 외부 이미지
	m_pTransform->m_vInfo[INFO_POS].x = 1044.f;
	m_pTransform->m_vInfo[INFO_POS].y = 685.f;
	m_pTransform->m_vLocalScale.x = 400.f;
	m_pTransform->m_vLocalScale.y = 50.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	// 물음표 3개
	m_pTransform->m_vInfo[INFO_POS].x = 1024.f;
	m_pTransform->m_vInfo[INFO_POS].y = 685.f;
	m_pTransform->m_vLocalScale.x = 6.f;
	m_pTransform->m_vLocalScale.y = 7.5f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pNumberTextureCom->Render_Texture(17);
	m_pBufferCom->Render_Buffer();

	m_pTransform->m_vInfo[INFO_POS].x = 1044.f;
	m_pTransform->m_vInfo[INFO_POS].y = 685.f;
	m_pTransform->m_vLocalScale.x = 6.f;
	m_pTransform->m_vLocalScale.y = 7.5f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pNumberTextureCom->Render_Texture(17);
	m_pBufferCom->Render_Buffer();

	m_pTransform->m_vInfo[INFO_POS].x = 1064.f;
	m_pTransform->m_vInfo[INFO_POS].y = 685.f;
	m_pTransform->m_vLocalScale.x = 6.f;
	m_pTransform->m_vLocalScale.y = 7.5f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pNumberTextureCom->Render_Texture(17);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIbosshp::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossHpUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pNumberTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_NumberUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE1, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUIbosshp::Key_Input(void)
{
}

void CUIbosshp::Update_NumverUI(void)
{
	if (m_fCurrentHp > m_fMaxHp)
		m_fCurrentHp = m_fMaxHp;
    else if (m_fCurrentHp < 0)
		m_fCurrentHp = 0;

	m_flength   = (400.f / m_fMaxHp) * (m_fMaxHp - m_fCurrentHp);
	m_fPosition = (1044.f - (m_flength / 2.f)) + ((m_fMaxHp - m_fCurrentHp) * (30.f / m_fMaxHp));
}

CUIbosshp* CUIbosshp::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIbosshp* pInstance = new CUIbosshp(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CUIbosshp Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUIbosshp::Free()
{
	CTempUI::Free();
}
