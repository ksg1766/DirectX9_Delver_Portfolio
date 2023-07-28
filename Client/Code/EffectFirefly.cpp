#include "stdafx.h"
#include "..\Header\EffectFirefly.h"
#include "PoolManager.h"

CEffectFirefly::CEffectFirefly(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectFirefly::~CEffectFirefly()
{
	Free();
}

HRESULT CEffectFirefly::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_EffectTag = EFFECTTAG::EFFECT_FIREFLY;

	CTempEffect::Get_RandomVector(&m_pTransform->m_vInfo[INFO_POS], &_vec3(-40.f, 4.f, -40.f), &_vec3(40.f, 10.f, 40.f));
	m_fEffectScale = CTempEffect::Get_RandomFloat(0.05f, 0.15f);

	CTempEffect::Get_RandomVector(&m_vecMoveDir, &_vec3(-1.f, -1.f, -1.f), &_vec3(1.f, 1.f, 1.f));
	m_fMoveSpeed = CTempEffect::Get_RandomFloat(2.f, 5.f);

	m_fChangeTime = CTempEffect::Get_RandomFloat(5.f, 10.f);
	m_fLifeTime = 0.f;

	return S_OK;
}

Engine::_int CEffectFirefly::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	m_fLifeTime += 1.f * fTimeDelta;
	m_fMoveTime += 1.f * fTimeDelta;

	if (m_fMoveTime > m_fChangeTime)
	{
		m_fMoveTime = 0.f;
		m_fChangeTime = CTempEffect::Get_RandomFloat(5.f, 10.f);

		CTempEffect::Get_RandomVector(&m_vecMoveDir, &_vec3(-.5f, -.5f, -.5f), &_vec3(.5f, .5f, .5f));
		m_fMoveSpeed = CTempEffect::Get_RandomFloat(10.f, 20.f);
	}

	if (m_fLifeTime > 3.f) {
		m_fEffectScale -= 0.05f * fTimeDelta * m_fMoveSpeed;
		if (m_fEffectScale <= 0.f)
		{
			m_fLifeTime = 0.f;
			CTempEffect::Get_RandomVector(&m_pTransform->m_vInfo[INFO_POS], &_vec3(-50.f, 4.f, -50.f), &_vec3(50.f, 10.f, 50.f));
			m_fEffectScale = CTempEffect::Get_RandomFloat(0.05f, 0.15f);
			CTempEffect::Get_RandomVector(&m_vecMoveDir, &_vec3(-1.f, -1.f, -1.f), &_vec3(1.f, 1.f, 1.f));
			m_fMoveSpeed = CTempEffect::Get_RandomFloat(2.f, 5.f);
		}
	}

	m_pTransform->m_vInfo[INFO_POS] += m_vecMoveDir * fTimeDelta * m_fMoveSpeed;

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	return iExit;
}

void CEffectFirefly::LateUpdate_Object(void)
{
	if ( m_pTransform->m_vInfo[INFO_POS].x < - 100.f || m_pTransform->m_vInfo[INFO_POS].x > 100.f ||
		m_pTransform->m_vInfo[INFO_POS].y < 3.f || m_pTransform->m_vInfo[INFO_POS].y > 20.f ||
		m_pTransform->m_vInfo[INFO_POS].z < -100.f || m_pTransform->m_vInfo[INFO_POS].z > 100.f ) 
	{
		m_fLifeTime = 0.f;
		CTempEffect::Get_RandomVector(&m_pTransform->m_vInfo[INFO_POS], &_vec3(-50.f, 4.f, -50.f), &_vec3(50.f, 10.f, 50.f));
		m_fEffectScale = CTempEffect::Get_RandomFloat(0.05f, 0.15f);
		CTempEffect::Get_RandomVector(&m_vecMoveDir, &_vec3(-1.f, -1.f, -1.f), &_vec3(1.f, 1.f, 1.f));
		m_fMoveSpeed = CTempEffect::Get_RandomFloat(2.f, 5.f);
	}

	CTempEffect::LateUpdate_Object();

	m_pBillBoardCom->LateUpdate_Component();
	m_pTransform->Scale(_vec3(m_fEffectScale, m_fEffectScale, m_fEffectScale));
}

void CEffectFirefly::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, & m_pTransform->WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_FOGENABLE, FALSE);

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FOGENABLE, TRUE);
	// 추후 조명의 영향도 안받게 추가
}

HRESULT CEffectFirefly::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Firefly"));
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

void CEffectFirefly::Free()
{
	CTempEffect::Free();
}

CEffectFirefly* CEffectFirefly::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectFirefly*	pInstance = new CEffectFirefly(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CEffectFirefly Create Failed");
		return nullptr;
	}

	return pInstance;
}
