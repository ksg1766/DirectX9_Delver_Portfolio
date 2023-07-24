#include "StrikeDown_Trap_Body.h"
#include "Export_Function.h"
#include "Player.h"
CStrikeDown_Trap::CStrikeDown_Trap(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CTrap(pGraphicDev)
{
}

CStrikeDown_Trap::CStrikeDown_Trap(const CStrikeDown_Trap& rhs)
	:Engine::CTrap(rhs)
{
}

CStrikeDown_Trap::~CStrikeDown_Trap()
{
}

HRESULT CStrikeDown_Trap::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::TRAP;
	m_eTrapTag = TRAPTAG::STRIKEDOWN;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(2.f, 2.f, 2.f));
	m_fTime = 0.f;
	m_bAttack = false;
	m_bCollisonBlock = false;
	m_fInitialHeight = 0.f;
	m_bPlayerHit = false;
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale() * 1.1f);

	__super::Ready_Object();
	//m_pTransform->Translate(_vec3(0.f, 1.5f, 0.f));

	return S_OK;
}

_int CStrikeDown_Trap::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);
	_uint iExit = 0;
	if (SCENETAG::EDITOR == SceneManager()->Get_Scene()->Get_SceneTag())
		return iExit;

	if (SceneManager()->Get_GameStop()) { return 0; }
	iExit = __super::Update_Object(fTimeDelta);
	m_fTime += fTimeDelta;
	Ground_Pounding(fTimeDelta);
	return iExit;
}

void CStrikeDown_Trap::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
}

void CStrikeDown_Trap::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture();
	m_pCubeBf->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

void CStrikeDown_Trap::Ground_Pounding(const _float& fTimeDelta)
{
	if ((!m_bAttack) && (5.f < m_fTime))
		m_pTransform->Translate(_vec3(0.f, -0.3f, 0.f));
	else if (m_bCollisonBlock)
	{
		m_bPlayerHit = true;
		m_pTransform->Translate(_vec3(0.f, (2.5f * fTimeDelta), 0.f));
	}
	if (m_fInitialHeight < m_pTransform->m_vInfo[INFO_POS].y)
	{
		m_fTime = 0.f;
		m_pTransform->m_vInfo[INFO_POS].y = m_fInitialHeight;
		m_bCollisonBlock = false;
		m_bAttack = false;
		m_bPlayerHit = false;
	}
}

void CStrikeDown_Trap::Set_InitailHeight(_float _Height)
{
	m_fInitialHeight = _Height + m_pTransform->m_vInfo[INFO_POS].y;
}

void CStrikeDown_Trap::Set_MinHeight(_float _Height)
{
	m_fMinHeight = m_pTransform->m_vInfo[INFO_POS].y - _Height;
}

void CStrikeDown_Trap::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if (m_bCollisonBlock) { return; }
	m_pOtherObj = _pOther->GetHost();

	if (OBJECTTAG::PLAYER == m_pOtherObj->Get_ObjectTag())
	{
		if (!m_bPlayerHit) {return;}
		CPlayerStat& PlayerState = *static_cast<CPlayer*>(_pOther->GetHost())->Get_Stat();
		PlayerState.Take_Damage(10.f);
		m_bPlayerHit = false;

		_vec3	vDir = _pOther->GetHost()->m_pTransform->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS];
		(dynamic_cast<CPlayer*>(_pOther->GetHost())->Get_RigidBody()->Add_Force(_vec3(vDir.x, 1.1f * 5.f, vDir.z)));
		(dynamic_cast<CPlayer*>(_pOther->GetHost())->Get_RigidBody()->UseGravity(true));
		(dynamic_cast<CPlayer*>(_pOther->GetHost())->Set_JumpState(true));
	}
}

void CStrikeDown_Trap::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	//__super::OnCollisionStay(_pOther);
}

void CStrikeDown_Trap::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

HRESULT CStrikeDown_Trap::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	pComponent = m_pCubeBf = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_ProjectileTrapBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = dynamic_cast<CRigidBody*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::RIGIDBODY, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CStrikeDown_Trap* CStrikeDown_Trap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStrikeDown_Trap* pInstance = new CStrikeDown_Trap(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Projectile_Trap_Body Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CStrikeDown_Trap::Free()
{
	__super::Free();
}
