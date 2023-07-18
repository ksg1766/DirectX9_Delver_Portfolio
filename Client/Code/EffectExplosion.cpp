#include "stdafx.h"
#include "..\Header\EffectExplosion.h"
#include "EffectExplosionChild.h"

CEffectExplosion::CEffectExplosion(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectExplosion::~CEffectExplosion()
{
	Free();
}

HRESULT CEffectExplosion::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_EffectTag = EFFECTTAG::EFFECT_EXPLOSION;

	m_bAnimation = true;

	m_fFrame = 0.f;
	m_fFrist = 0.f;
	m_fFinal = 8.f;
	m_fFrameSpeed = 2.3f;

	m_fLife = 5.f;

	m_fEffectScale = 2.f;

	return S_OK;
}

Engine::_int CEffectExplosion::Update_Object(const _float& fTimeDelta)
{
	if (m_RandomSet && m_fFrame >= m_fFinal / 2)
	{
		m_RandomSet = false;

		for (int i = 0; i < 6; ++i)
		{
			float angle = i * 60.0f * (3.14159f / 180.0f); 

			float posX = m_pTransform->m_vInfo[INFO_POS].x + 2.0f * cos(angle);
			float posY = m_pTransform->m_vInfo[INFO_POS].y - .5f;
			float posZ = m_pTransform->m_vInfo[INFO_POS].z + 2.0f * sin(angle);

			CGameObject* pGameObject = CEffectExplosionChild::Create(m_pGraphicDev);
			pGameObject->m_pTransform->Translate(_vec3(posX, posY, posZ));
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			//CPoolManager::GetInstance()->Create_Effect(EFFECTTAG::, TargetPos);
		}
	}

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	return iExit;
}

void CEffectExplosion::LateUpdate_Object(void)
{
	CTempEffect::LateUpdate_Object();

	m_pBillBoardCom->LateUpdate_Component();
	m_pTransform->Scale(_vec3(m_fEffectScale, m_fEffectScale, m_fEffectScale));
}

void CEffectExplosion::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, & m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CEffectExplosion::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EffectExplosion"));
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

void CEffectExplosion::Free()
{
	CTempEffect::Free();
}

CEffectExplosion* CEffectExplosion::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectExplosion*	pInstance = new CEffectExplosion(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CEffectExplosion Create Failed");
		return nullptr;
	}

	return pInstance;
}
