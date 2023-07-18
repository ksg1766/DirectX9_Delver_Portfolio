#include "stdafx.h"
#include "..\Header\EffectBlood.h"

CEffectBlood::CEffectBlood(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectBlood::~CEffectBlood()
{
	Free();
}

HRESULT CEffectBlood::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fLife  = 10.f;
	m_pTransform->Rotate(ROT_X, D3DXToRadian(90.f));
	m_pTransform->Rotate(ROT_Y, D3DXToRadian(CTempEffect::Get_RandomFloat(.0f, 180.f)));

	m_bParent = true;

	return S_OK;
}

Engine::_int CEffectBlood::Update_Object(const _float& fTimeDelta)
{
	if (m_bParent && !m_bScaleSet) {
		m_bScaleSet = true;
		switch (m_EffectColor)
		{
		case Engine::ECOLOR_PINK:
			m_fFrame = 0.f;
			break;
		case Engine::ECOLOR_RED:
			m_fFrame = 0.f;
			break;
		case Engine::ECOLOR_ORANGE:
			m_fFrame = 0.f;
			break;
		case Engine::ECOLOR_YELLOW:
			m_fFrame = 4.f;
			break;
		case Engine::ECOLOR_LIGHTGREEN:
			m_fFrame = 0.f;
			break;
		case Engine::ECOLOR_GREEN:
			m_fFrame = 2.f;
			break;
		case Engine::ECOLOR_SKYBLUE:
			m_fFrame = 0.f;
			break;
		case Engine::ECOLOR_BLUE:
			m_fFrame = 0.f;
			break;
		case Engine::ECOLOR_INDIGO:
			m_fFrame = 0.f;
			break;
		case Engine::ECOLOR_PURPLE:
			m_fFrame = 0.f;
			break;
		case Engine::ECOLOR_WHITE:
			m_fFrame = 0.f;
			break;
		case Engine::ECOLOR_RAINBOW:
			m_fFrame = 0.f;
			break;
		default:
			m_fFrame = 0.f;
			break;
		}

		m_fEffectScale = CTempEffect::Get_RandomFloat(.7f, 1.2f);
		m_pTransform->Scale(_vec3(m_fEffectScale, m_fEffectScale, m_fEffectScale));
	}
	else if (m_bChild && !m_bScaleSet) {
		m_bScaleSet = true;
		switch (m_EffectColor)
		{
		case Engine::ECOLOR_PINK:
			m_fFrame = 1.f;
			break;
		case Engine::ECOLOR_RED:
			m_fFrame = 1.f;
			break;
		case Engine::ECOLOR_ORANGE:
			m_fFrame = 1.f;
			break;
		case Engine::ECOLOR_YELLOW:
			m_fFrame = 5.f;
			break;
		case Engine::ECOLOR_LIGHTGREEN:
			m_fFrame = 1.f;
			break;
		case Engine::ECOLOR_GREEN:
			m_fFrame = 3.f;
			break;
		case Engine::ECOLOR_SKYBLUE:
			m_fFrame = 1.f;
			break;
		case Engine::ECOLOR_BLUE:
			m_fFrame = 1.f;
			break;
		case Engine::ECOLOR_INDIGO:
			m_fFrame = 1.f;
			break;
		case Engine::ECOLOR_PURPLE:
			m_fFrame = 1.f;
			break;
		case Engine::ECOLOR_WHITE:
			m_fFrame = 1.f;
			break;
		case Engine::ECOLOR_RAINBOW:
			m_fFrame = 0.f;
			break;
		default:
			m_fFrame = 0.f;
			break;
		}
		m_fEffectScale = CTempEffect::Get_RandomFloat(.5f, .8f);
		m_pTransform->Scale(_vec3(m_fEffectScale, m_fEffectScale, m_fEffectScale));
	}

	if (m_RandomSet)
	{
		m_RandomSet = false;
		CGameObject* pGameObject = CEffectBlood::Create(m_pGraphicDev);
		pGameObject->m_pTransform->Translate(_vec3(m_pTransform->m_vInfo[INFO_POS].x + CTempEffect::Get_RandomFloat(-1.f, 1.f), m_pTransform->m_vInfo[INFO_POS].y + 0.01f, m_pTransform->m_vInfo[INFO_POS].z + CTempEffect::Get_RandomFloat(-1.f, 1.f)));
		dynamic_cast<CTempEffect*>(pGameObject)->Set_Parent(false);
		dynamic_cast<CTempEffect*>(pGameObject)->Set_Child(true);
		dynamic_cast<CTempEffect*>(pGameObject)->Set_RandomSet(false);
		dynamic_cast<CTempEffect*>(pGameObject)->Set_EffectColor(m_EffectColor);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
	}

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	return iExit;
}

void CEffectBlood::LateUpdate_Object(void)
{
	CTempEffect::LateUpdate_Object();
}

void CEffectBlood::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, & m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CEffectBlood::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EffectBlood"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CEffectBlood::Free()
{
	CTempEffect::Free();
}

CEffectBlood* CEffectBlood::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectBlood*	pInstance = new CEffectBlood(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CEffectBlood Create Failed");
		return nullptr;
	}

	return pInstance;
}
