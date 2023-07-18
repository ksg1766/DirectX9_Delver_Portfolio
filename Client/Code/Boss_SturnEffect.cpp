#include "stdafx.h"
#include "Boss_SturnEffect.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
CBoss_SturnEffect::CBoss_SturnEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CMonster(pGraphicDev), m_fFrame(0.f)
{
}

CBoss_SturnEffect::CBoss_SturnEffect(const CBoss_SturnEffect& rhs)
	:	Engine::CMonster(rhs), m_fFrame(rhs.m_fFrame)
{
}

CBoss_SturnEffect::~CBoss_SturnEffect()
{
	Free();
}

HRESULT CBoss_SturnEffect::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::MONSTERBULLET;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fTime = 0.f;
	return S_OK;
}

_int CBoss_SturnEffect::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);
	
	m_pTransform->m_vInfo[INFO_POS]= Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front()->m_pTransform->m_vInfo[INFO_POS];
	m_pTransform->m_vInfo[INFO_POS].y = Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front()->m_pTransform->m_vInfo[INFO_POS].y + 1.5f;
	m_pTransform->m_vInfo[INFO_POS].z = Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front()->m_pTransform->m_vInfo[INFO_POS].z - 0.1f;

	m_fTime += fTimeDelta;
	m_fFrame += 9.f * fTimeDelta * 2;
	if (9.f < m_fFrame)
		m_fFrame = 0.f;

	if (3.f < m_fTime)
	{
		m_fTime = 0.f;
		Engine::EventManager()->DeleteObject(this);
	}

	return iExit;
}

void CBoss_SturnEffect::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	m_pBillBoard->LateUpdate_Component();
	__super::LateUpdate_Object();
	__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);
}

void CBoss_SturnEffect::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pTexture->Render_Texture((_uint)m_fFrame);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pBuffer->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBoss_SturnEffect::Init_Stat()
{
}

HRESULT CBoss_SturnEffect::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Sturn"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pBillBoard = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CBoss_SturnEffect* CBoss_SturnEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss_SturnEffect* pInstance = new CBoss_SturnEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Sturn Star Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBoss_SturnEffect::Free()
{
	__super::Free();
}
