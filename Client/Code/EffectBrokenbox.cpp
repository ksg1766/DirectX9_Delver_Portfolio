#include "stdafx.h"
#include "..\Header\EffectBrokenbox.h"

CEffectBrokenbox::CEffectBrokenbox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectBrokenbox::~CEffectBrokenbox()
{
	Free();
}

HRESULT CEffectBrokenbox::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fFrame = 0.f;
	m_fFrist = 0.f;
	m_fFinal = 5.f;
	m_fFrameSpeed = CTempEffect::Get_RandomFloat(1.f, 2.f);

	m_fLife       = CTempEffect::Get_RandomFloat(5.f, 10.f);

	m_fEffectScale = CTempEffect::Get_RandomFloat(1.f, 1.5f);

	return S_OK;
}

Engine::_int CEffectBrokenbox::Update_Object(const _float& fTimeDelta)
{
	//if (m_RandomSet)
	//{
	//	m_RandomSet = false;
	//	// 랜덤으로 50%의 확률로 자신 객체 하나 더 생성
	//	if (CTempEffect::Get_RandomFloat(0.0f, 1.f) > 0.3f)
	//	{
	//		CGameObject* pGameObject = CEffectBubble::Create(m_pGraphicDev);
	//		pGameObject->m_pTransform->Translate(_vec3(m_pTransform->m_vInfo[INFO_POS].x + CTempEffect::Get_RandomFloat(.5f, 1.f), m_pTransform->m_vInfo[INFO_POS].y + CTempEffect::Get_RandomFloat(.5f, 1.f), m_pTransform->m_vInfo[INFO_POS].z + CTempEffect::Get_RandomFloat(.5f, 1.f)));
	//		dynamic_cast<CTempEffect*>(pGameObject)->Set_RandomSet(false);
	//		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
	//	}
	//}

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	//m_pTransform->m_vInfo[INFO_POS].y += m_fUpSpeed * fTimeDelta;

	return iExit;
}

void CEffectBrokenbox::LateUpdate_Object(void)
{
	CTempEffect::LateUpdate_Object();

	m_pBillBoardCom->LateUpdate_Component();
	m_pTransform->Scale(_vec3(m_fEffectScale, m_fEffectScale, m_fEffectScale));
}

void CEffectBrokenbox::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, & m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CEffectBrokenbox::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_WhiteBubble"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pBillBoardCom = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CEffectBrokenbox::Free()
{
	CTempEffect::Free();
}

CEffectBrokenbox* CEffectBrokenbox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectBrokenbox*	pInstance = new CEffectBrokenbox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CEffectBrokenbox Create Failed");
		return nullptr;
	}

	return pInstance;
}
