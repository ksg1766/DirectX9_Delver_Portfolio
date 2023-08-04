#include "stdafx.h"
#include "..\Header\EffectWaterMove.h"
#include "PoolManager.h"

CEffectWaterMove::CEffectWaterMove(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectWaterMove::~CEffectWaterMove()
{
	Free();
}

HRESULT CEffectWaterMove::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_EffectTag = EFFECTTAG::EFFECT_WATERFALL;

	m_fEffectScale = 1.f;
	m_pTransform->Scale(_vec3(m_fEffectScale, m_fEffectScale, m_fEffectScale));

	m_bAnimation = true;
	m_bLoop = true;

	m_fFrame = 0.f;
	m_fFrist = 0.f;
	m_fFinal = 3.f;
	m_fFrameSpeed = CTempEffect::Get_RandomFloat(.1f, .5f);
	
	return S_OK;
}

Engine::_int CEffectWaterMove::Update_Object(const _float& fTimeDelta)
{
	if (m_RandomSet) {
		m_RandomSet = false;

		m_fSpeed = CTempEffect::Get_RandomFloat(.1f, .5f);

		if (m_iMoveSet == 1) {
			m_fEffectScale *= (m_MaxNumber - (m_PickNumber * 0.5f)) - m_vecMoveScale[1].x;// *(CTempEffect::Get_RandomFloat(m_vecMoveScale[0].x, m_vecMoveScale[1].x));
			m_pTransform->m_vInfo[INFO_POS].z += .1f * m_PickNumber;

			m_pTransform->m_vInfo[INFO_POS].x = CTempEffect::Get_RandomFloat(m_vecOriginPos.x + m_EffectBoundingBox.vMin.x, m_vecOriginPos.x + m_EffectBoundingBox.vMax.x);
		}
		else if (m_iMoveSet == 2) {
			m_fEffectScale *= (m_MaxNumber - (m_PickNumber * 0.5f)) - m_vecMoveScale[1].x;// *(CTempEffect::Get_RandomFloat(m_vecMoveScale[0].x, m_vecMoveScale[1].x));
			m_pTransform->Rotate(ROT_Y, D3DXToRadian(90.f));
			m_pTransform->m_vInfo[INFO_POS].x += .1f * m_PickNumber;

			m_pTransform->m_vInfo[INFO_POS].z = CTempEffect::Get_RandomFloat(m_vecOriginPos.z + m_EffectBoundingBox.vMin.z, m_vecOriginPos.z + m_EffectBoundingBox.vMax.z);
		}
		
		m_pTransform->Scale(_vec3(m_fEffectScale, m_fEffectScale, m_fEffectScale));
		m_pTransform->m_vInfo[INFO_POS].y = (m_fEffectScale - 1.f) + m_EffectBoundingBox.vMin.y; // 4

		m_bRight = true;
	}

	if (m_iMoveSet == 1)
	{
		if (m_bRight && m_pTransform->m_vInfo[INFO_POS].x < m_vecOriginPos.x + m_EffectBoundingBox.vMax.x + (m_fEffectScale - 1.f)) {// - 20.f) {

			m_pTransform->m_vInfo[INFO_POS].x += 1.f * fTimeDelta * m_fSpeed;
			if (m_pTransform->m_vInfo[INFO_POS].x >= m_vecOriginPos.x + m_EffectBoundingBox.vMax.x)
			{
				m_bRight = false;
				m_bLeft = true;
			}
		}

		if (m_bLeft && m_pTransform->m_vInfo[INFO_POS].x > m_vecOriginPos.x + m_EffectBoundingBox.vMin.x - (m_fEffectScale - 1.f)) //- 26.f)
		{
			m_pTransform->m_vInfo[INFO_POS].x -= 1.f * fTimeDelta * m_fSpeed;
			if (m_pTransform->m_vInfo[INFO_POS].x <= m_vecOriginPos.x + m_EffectBoundingBox.vMin.x)
			{
				m_bLeft = false;
				m_bRight = true;
			}
		}
	}
	else if (m_iMoveSet == 2)
	{
		if (m_bRight && m_pTransform->m_vInfo[INFO_POS].z < m_vecOriginPos.z + m_EffectBoundingBox.vMax.z + (m_fEffectScale - 1.f)) {// - 20.f) {

			m_pTransform->m_vInfo[INFO_POS].z += 1.f * fTimeDelta * m_fSpeed;
			if (m_pTransform->m_vInfo[INFO_POS].z >= m_vecOriginPos.z + m_EffectBoundingBox.vMax.z)
			{
				m_bRight = false;
				m_bLeft = true;
			}
		}

		if (m_bLeft && m_pTransform->m_vInfo[INFO_POS].z > m_vecOriginPos.z + m_EffectBoundingBox.vMin.z - (m_fEffectScale - 1.f)) //- 26.f)
		{
			m_pTransform->m_vInfo[INFO_POS].z -= 1.f * fTimeDelta * m_fSpeed;
			if (m_pTransform->m_vInfo[INFO_POS].z <= m_vecOriginPos.z + m_EffectBoundingBox.vMin.z)
			{
				m_bLeft = false;
				m_bRight = true;
			}
		}
	}

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	return iExit;
}

void CEffectWaterMove::LateUpdate_Object(void)
{
	CTempEffect::LateUpdate_Object();
}

void CEffectWaterMove::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, & m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CEffectWaterMove::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EffectWaterMove"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CEffectWaterMove::Free()
{
	CTempEffect::Free();
}

CEffectWaterMove* CEffectWaterMove::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectWaterMove*	pInstance = new CEffectWaterMove(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CEffectWaterMove Create Failed");
		return nullptr;
	}

	return pInstance;
}
