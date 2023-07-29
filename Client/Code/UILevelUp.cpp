#include "stdafx.h"
#include "..\Header\UILevelUp.h"
#include "UILevelUpCard.h"
#include "Player.h"

CUILevelUp::CUILevelUp(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}


CUILevelUp::~CUILevelUp()
{

}

HRESULT CUILevelUp::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // 초기화
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_pTransform->m_vInfo[INFO_POS].x = WINCX / 2;
	m_pTransform->m_vInfo[INFO_POS].y = WINCY / 2 + 1.5f;
	m_pTransform->m_vLocalScale.x = 300;
	m_pTransform->m_vLocalScale.y = 280;
	m_FontWorld = WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

	m_pTransform->m_vInfo[INFO_POS].x = WINCX / 2;
	m_pTransform->m_vInfo[INFO_POS].y = WINCY / 2;
	m_pTransform->m_vLocalScale.x = 650;
	m_pTransform->m_vLocalScale.y = 370;
	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

	// 카드 6개 생성 후 보유
	CGameObject* pGameObject = nullptr;
	for (_uint i = 0; i < 6; ++i)
	{
		pGameObject = CUILevelUpCard::Create(m_pGraphicDev);
		dynamic_cast<CUILevelUpCard*>(pGameObject)->Set_CardType(i);

		m_vecCardObject.push_back(pGameObject);
	}

	return S_OK;
}

_int CUILevelUp::Update_Object(const _float& fTimeDelta)
{
	if (m_bNotUseLevelUpUI) {
		m_bNotUseLevelUpUI = false;
		SceneManager()->Set_GameStop(false);


		m_vecPickObject.clear();

		m_bPickRender = false;
		m_IsDead = true;
	}

	if (m_IsDead)
		return 0;

	if (m_bUseLevelUpUI) {
		m_bUseLevelUpUI = false;
		SceneManager()->Set_GameStop(true);

		// 랜덤으로 카드 3개 활성화 -> 골라진 순서대로 1번째에서 보이고 1 / 2 / 3번째 위치로 이동함

		dynamic_cast<CUILevelUpCard*>(m_vecCardObject[1])->Set_PickNumber(0);
		dynamic_cast<CUILevelUpCard*>(m_vecCardObject[1])->Set_UseCard(true);
		m_vecPickObject.push_back(m_vecCardObject[1]);
		m_bPickRender = true;
	}

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUILevelUp::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();
}

void CUILevelUp::Render_Object()
{
	if (m_IsDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_FontWorld);

	m_pTextureCom->Render_Texture(1);
	m_pBufferCom->Render_Buffer();

	// 선택된 카드 3개 렌더링
	if (m_bPickRender) {
		for (auto& iter : m_vecPickObject) {
			iter->Render_Object();
		}
	}
}

HRESULT CUILevelUp::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_LevelUpBackground"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUILevelUp::Key_Input(void)
{
}

CUILevelUp* CUILevelUp::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUILevelUp* pInstance = new CUILevelUp(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CUILevelUp Failed");
		return nullptr;
	}

	return pInstance;
}

void CUILevelUp::Free()
{
	CTempUI::Free();
}
