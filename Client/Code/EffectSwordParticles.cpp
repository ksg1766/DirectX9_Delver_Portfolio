#include "stdafx.h"
#include "..\Header\EffectSwordParticles.h"

// 이펙트 테스트
#include "Player.h"

CEffectSwordParticles::CEffectSwordParticles(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectSwordParticles::~CEffectSwordParticles()
{
	Free();
}

HRESULT CEffectSwordParticles::Ready_Object()
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_EffectTag = EFFECTTAG::EFFECT_HEKIREKIISSEN;

	m_bAnimation = true;
	m_bLoop = true;

	m_fFrame = 0.f;

	m_fFrameSpeed = 0.2f;

	m_fTime = 0.f;
	m_fLife = 2.f;

	m_fEffectScale = 40.f;
	m_pTransform->Scale(_vec3(m_fEffectScale, 0.001f, m_fEffectScale));

	return S_OK;
}

_int CEffectSwordParticles::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (m_fTime > m_fLife || m_fFrame == m_fFinal && m_bAnimation && !m_bLoop)
	{
		EventManager()->DeleteObject(this);
	}

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	if (m_fCurScaleRate < m_fMaxScaleRate)
	{
		m_fCurScaleRate += .5f * fTimeDelta;
		m_pTransform->Scale(*D3DXVec3Lerp(&_vec3(), &_vec3(m_fEffectScale, 0.001f, m_fEffectScale), &_vec3(m_fEffectScale, 0.5f * m_fEffectScale, m_fEffectScale), m_fCurScaleRate));
	}

	return iExit;
}

void CEffectSwordParticles::LateUpdate_Object(void)
{
	CTempEffect::LateUpdate_Object();

	//CPlayer* pPlayer = SceneManager()->Get_Scene()->Get_MainPlayer();
	//m_pTransform->m_vInfo[INFO_POS] = pPlayer->m_pTransform->m_vInfo[INFO_POS] + pPlayer->m_pTransform->m_vInfo[INFO_LOOK] * 45.f;
}

void CEffectSwordParticles::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CEffectSwordParticles::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"EffectSwordParticles"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CEffectSwordParticles* CEffectSwordParticles::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectSwordParticles* pInstance = new CEffectSwordParticles(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("EffectSwordParticles Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffectSwordParticles::Free()
{
	CTempEffect::Free();
}
