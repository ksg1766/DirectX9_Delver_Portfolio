#include "stdafx.h"
#include "Projectile_Trap_Arrow.h"
#include "Export_Function.h"
#include "Terrain.h"
#include "Player.h"
#include "Projectile_Trap_Body.h"
CProjectile_Trap_Arrow::CProjectile_Trap_Arrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CMonster(pGraphicDev)
{
}

CProjectile_Trap_Arrow::CProjectile_Trap_Arrow(const CProjectile_Trap_Arrow& rhs)
	:	Engine::CMonster(rhs)
{
}

CProjectile_Trap_Arrow::~CProjectile_Trap_Arrow()
{
	Free();
}

HRESULT CProjectile_Trap_Arrow::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::MONSTERBULLET;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pBasicStat->Get_Stat()->fAttack = 5.f;
	m_fTime = 0.f;
	m_fFrame = 0.f;
	m_pTransform->Scale(_vec3(0.5f, 0.5f, 0.5f));
	return S_OK;
}

_int CProjectile_Trap_Arrow::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);
	m_fFrame += 8.f * fTimeDelta * 2;
	if (8.f < m_fFrame)
		m_fFrame = 0.f;
	m_fTime += fTimeDelta;

	m_pTransform->Translate(m_vDir* fTimeDelta);

	if (5.f < m_fTime)
	{
		m_fTime = 0.f;
		Engine::EventManager()->DeleteObject(this);
	}
	return iExit;
}

void CProjectile_Trap_Arrow::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
	__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);
}

void CProjectile_Trap_Arrow::Render_Object(void)
{
	m_pTransform->Scale(_vec3(0.5f, 0.5f, 0.5f));
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTexture->Render_Texture(_uint(m_fFrame));
	m_pBuffer->Render_Buffer();
#if _DEBUG
	m_pCollider->Render_Collider();
#endif
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CProjectile_Trap_Arrow::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
	
	}

}

void CProjectile_Trap_Arrow::OnCollisionStay(CCollider* _pOther)
{


}

void CProjectile_Trap_Arrow::OnCollisionExit(CCollider* _pOther)
{

}

void CProjectile_Trap_Arrow::Set_Dir(_vec3 _vDir)
{
	m_vDir = _vDir- m_pTransform->m_vInfo[INFO_POS];
}

HRESULT CProjectile_Trap_Arrow::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossFire"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = static_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CProjectile_Trap_Arrow* CProjectile_Trap_Arrow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CProjectile_Trap_Arrow* pInstance = new CProjectile_Trap_Arrow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Projectitle_Trap_Arrow Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CProjectile_Trap_Arrow::Free()
{
	__super::Free();
}
