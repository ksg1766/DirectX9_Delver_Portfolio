#include "stdafx.h"
#include "..\Header\EffectBrokenbox.h"
#include "PoolManager.h"

CEffectBrokenbox::CEffectBrokenbox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectBrokenbox::~CEffectBrokenbox()
{
	Free();
}

HRESULT CEffectBrokenbox::Ready_Object()
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_EffectTag = EFFECTTAG::EFFECT_BROKENBOX;


	_float fRandom = CTempEffect::Get_RandomFloat(.0f, .4f);
	if (fRandom > .2f)
		m_fFrame = 0.f;
	else
		m_fFrame = 1.f;
	
	m_fTime			= 0.f;
	m_fLife			= 7.f;
	m_fEffectScale	= .5f;

	m_fDownSpeed = 7.f;

	return S_OK;
}

Engine::_int CEffectBrokenbox::Update_Object(const _float& fTimeDelta)
{
	// 이미지 랜덤 1 택 
	if (m_bBoxSet)
	{
		m_bBoxSet = false;

		_float fRandom = CTempEffect::Get_RandomFloat(.0f, .4f);
		if (m_iType == 0)
		{
			if (fRandom > .2f)
				m_fFrame = 0.f;
			else
				m_fFrame = 1.f;
		}
		else if (m_iType == 1)
		{
			if (fRandom > .2f)
				m_fFrame = 2.f;
			else
				m_fFrame = 3.f;
		}

	}

	if (m_RandomSet)
	{
		m_RandomSet = false;
		_int iNum = 0.f;

		_float fRandom = CTempEffect::Get_RandomFloat(.0f, .4f);
		if (fRandom > .2f)
			iNum = 3.f;
		else
			iNum = 4.f;

		for (_int i = 0; i < iNum; ++i)
		{
			CGameObject* pGameObject = CEffectBrokenbox::Create(m_pGraphicDev);
			pGameObject->m_pTransform->Translate(_vec3(m_pTransform->m_vInfo[INFO_POS].x + CTempEffect::Get_RandomFloat(-.25f, .25f),
				m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vInfo[INFO_POS].z + CTempEffect::Get_RandomFloat(-.25f, .25f)));
			dynamic_cast<CEffectBrokenbox*>(pGameObject)->Set_EffectType(m_iType);
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

	if (m_fTime < 0.5f)
	{
		m_pTransform->m_vInfo[INFO_POS].y -= 0.15f * fTimeDelta * m_fDownSpeed;
	}

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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EffectBrokenbox"));
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
