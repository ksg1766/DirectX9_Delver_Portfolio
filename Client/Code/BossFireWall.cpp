#include "stdafx.h"
#include "..\Header\BossFireWall.h"
#include "Export_Function.h"
#include "Player.h"
CBossFireWall::CBossFireWall(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CMonster(pGraphicDev), m_fFrame(0.f)
{
}

CBossFireWall::CBossFireWall(const CBossFireWall& rhs)
	:	Engine::CMonster(rhs), m_fFrame(rhs.m_fFrame)
{
}

CBossFireWall::~CBossFireWall()
{
	Free();
}

HRESULT CBossFireWall::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::MONSTERBULLET;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->Scale(_vec3(2.f, 2.f, 2.f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale()*0.5f);
	m_pBasicStat->Get_Stat()->fAttack = 2.f;
	m_fDuration = 0.f;
	m_fSpeed = 20.f;
	m_fAngle = 0.f;
	m_fHitCool = 0.f;
	m_fDuration = 0.f;
	return S_OK;
}

_int CBossFireWall::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);
	if ((1.f < m_fHitCool)&&(m_bHit))
		m_bHit = false;

	m_fDuration += fTimeDelta;
	m_fFrame += 8.f * fTimeDelta * 2;
	m_pTransform->RotateAround(m_vCenter, _vec3(0.f, 1.f, 0.f),1.f* fTimeDelta);
	if (8.f < m_fFrame)
		m_fFrame = 0.f;
	if (10.f < m_fDuration)
		EventManager()->DeleteObject(this);
	return iExit;
}

void CBossFireWall::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	m_pBillBoard->LateUpdate_Component();
	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(2.f, 2.f, 2.f));
}

void CBossFireWall::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pTexture->Render_Texture((_uint)m_fFrame);
	m_pBuffer->Render_Buffer();
#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

void CBossFireWall::Init_Stat()
{
}

void CBossFireWall::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CBossFireWall::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if (m_bHit) { return; }
	if (_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		CPlayerStat& PlayerState = *(dynamic_cast<CPlayer*>(_pOther->GetHost())->Get_Stat());
		PlayerState.Take_Damage(this->Get_BasicStat()->Get_Stat()->fAttack);
		this->Set_AttackTick(true);
		m_bHit = true;
		//Engine::EventManager()->DeleteObject(this);
	}
}

void CBossFireWall::OnCollisionExit(CCollider* _pOther)
{

}

HRESULT CBossFireWall::Add_Component(void)
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

	pComponent = m_pBillBoard = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

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

CBossFireWall* CBossFireWall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossFireWall* pInstance = new CBossFireWall(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BossProjectitle Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBossFireWall::Free()
{
	__super::Free();
}
