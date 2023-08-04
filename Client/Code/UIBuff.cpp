#include "UIBuff.h"
#include "Export_Function.h"

CUIBuff::CUIBuff(LPDIRECT3DDEVICE9 pGraphicDev)
	:CTempUI(pGraphicDev)
{
}

CUIBuff::~CUIBuff()
{
}

HRESULT CUIBuff::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS].x = WINCX / 2 + WINCX / 2.3;
	m_pTransform->m_vInfo[INFO_POS].y = WINCY - WINCY / 5;

	m_pTransform->m_vLocalScale.x = 80.f;
	m_pTransform->m_vLocalScale.y = 30.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

	m_pFontconfig = static_cast<CFont*>(m_pFont)->Create_3DXFont(32, 13.f, 1000.f, false, TEXT("¸¼Àº °íµñ"), m_pFontconfig);
	static_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig);
	static_cast<CFont*>(m_pFont)->Set_FontColor(_uint(0xffffffff));
	static_cast<CFont*>(m_pFont)->Set_Rect(RECT{ 0, 520, WINCX, WINCY });
	static_cast<CFont*>(m_pFont)->Set_Anchor(DT_CENTER | DT_NOCLIP);

	m_fRemainingTime = 0.f;
	return S_OK;
}

_int CUIBuff::Update_Object(const _float& fTimeDelta)
{
	if (m_IsDead)
		return 0;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;;
}

void CUIBuff::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();
}

void CUIBuff::Render_Object()
{
	if (m_IsDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIBuff::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SpeechBubbleUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pFont = static_cast<CFont*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Font"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::FONT, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CUIBuff* CUIBuff::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIBuff* pInstance = new CUIBuff(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UIQuest Board Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUIBuff::Free()
{
	CTempUI::Free();
}
