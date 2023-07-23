#include "stdafx.h"
#include "..\Header\EffectExplosionChild.h"
#include "PoolManager.h"

CEffectExplosionChild::CEffectExplosionChild(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectExplosionChild::~CEffectExplosionChild()
{
	Free();
}

HRESULT CEffectExplosionChild::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_EffectTag = EFFECTTAG::EFFECT_EXPLOSIONCHILD;

	m_bAnimation = true;

	m_fLife = 5.f;
	m_bParent = true;

	return S_OK;
}

Engine::_int CEffectExplosionChild::Update_Object(const _float& fTimeDelta)
{
	if (m_bParent && !m_bScaleSet)
	{
		m_bScaleSet = true;
		
		m_fFrame = 0.f;
		m_fFrist = 0.f;
		m_fFinal = 8.f;
		m_fFrameSpeed = 2.3f;

		m_fEffectScale = 1.f;
	}
	else if (m_bChild && !m_bScaleSet)
	{
		m_bScaleSet = true;

		m_fFrame = 9.f;
		m_fFrist = 9.f;
		m_fFinal = 15.f;
		m_fFrameSpeed = 2.3f;

		m_fEffectScale = .8f;
	}

	if (m_RandomSet && m_fFrame >= m_fFinal / 2)
	{
		m_RandomSet = false;

		for (int i = 0; i < 6; ++i)
		{
			float angle = i * 60.0f * (3.14159f / 180.0f);

			float posX = m_pTransform->m_vInfo[INFO_POS].x + 1.0f * cos(angle);
			float posY = m_pTransform->m_vInfo[INFO_POS].y - .5f;
			float posZ = m_pTransform->m_vInfo[INFO_POS].z + 1.0f * sin(angle);

			CGameObject* pGameObject = CEffectExplosionChild::Create(m_pGraphicDev);
			pGameObject->m_pTransform->Translate(_vec3(posX, posY, posZ));
			dynamic_cast<CTempEffect*>(pGameObject)->Set_Parent(false);
			dynamic_cast<CTempEffect*>(pGameObject)->Set_Child(true);
			dynamic_cast<CTempEffect*>(pGameObject)->Set_RandomSet(false);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			//CPoolManager::GetInstance()->Create_Effect(EFFECTTAG::, TargetPos);

		}
	}

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (m_fTime > m_fLife || m_fFrame == m_fFinal && m_bAnimation && !m_bLoop)
	{
		CPoolManager::GetInstance()->Delete_Object(this);
	}

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	return iExit;
}

void CEffectExplosionChild::LateUpdate_Object(void)
{
	CTempEffect::LateUpdate_Object();

	m_pBillBoardCom->LateUpdate_Component();
	m_pTransform->Scale(_vec3(m_fEffectScale, m_fEffectScale, m_fEffectScale));
}

void CEffectExplosionChild::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, & m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CEffectExplosionChild::Add_Component(void)
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

void CEffectExplosionChild::Free()
{
	CTempEffect::Free();
}

CEffectExplosionChild* CEffectExplosionChild::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectExplosionChild*	pInstance = new CEffectExplosionChild(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CEffectExplosionChild Create Failed");
		return nullptr;
	}

	return pInstance;
}
