#include "Boss_BatSwarm.h"
#include "Export_Function.h"
#include "Player.h"

CBoss_BatSwarm::CBoss_BatSwarm(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CMonster(pGraphicDev)
{
}

CBoss_BatSwarm::CBoss_BatSwarm(const CBoss_BatSwarm& rhs)
	: Engine::CMonster(rhs)
{
}

CBoss_BatSwarm::~CBoss_BatSwarm()
{
	Free();
}

HRESULT CBoss_BatSwarm::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::MONSTERBULLET;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fFrame = 0.f;
	m_bTest = false;

	m_fDirSpeed = 1.f;
	m_fMoveSpeed = 5.f;
	m_fMaxSpeed = 30.f;
	return S_OK;
}

_int CBoss_BatSwarm::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_uint iExit = __super::Update_Object(fTimeDelta);

	m_fTime += fTimeDelta*5;
	m_fFrame += 2.f * fTimeDelta * 5;
	if (2.f < m_fFrame)
		m_fFrame = 0.f;
	if (m_fMaxSpeed >= m_fDirSpeed)
		m_fDirSpeed*= m_fTime;
	m_pTransform->Translate(-m_vInitialDir * m_fMoveSpeed * fTimeDelta);
	m_pTransform->Translate(m_vDir * m_fDirSpeed * fTimeDelta);

	return iExit;
}

void CBoss_BatSwarm::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
	
}

void CBoss_BatSwarm::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(_uint(m_fFrame));
	m_pBuffer->Render_Buffer();
}

void CBoss_BatSwarm::Set_TargetPos(_vec3 vTargetPos)
{
	m_vTargetPos = vTargetPos;
	m_vDir = m_vTargetPos - m_pTransform->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&m_vDir, &m_vDir);
}

void CBoss_BatSwarm::Set_InitialDir(_vec3 vInitialPos)
{
	m_vInitialDir = vInitialPos - m_pTransform->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&m_vInitialDir, &m_vInitialDir);
}

void CBoss_BatSwarm::Init_Stat()
{
}

void CBoss_BatSwarm::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

}

void CBoss_BatSwarm::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CBoss_BatSwarm::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

}

HRESULT CBoss_BatSwarm::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;
	
	pComponent = m_pBuffer = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossBatSwarm"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = static_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	//pComponent = m_pRigidBody = static_cast<CRigidBody*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RigidBody"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::RIGIDBODY, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CBoss_BatSwarm* CBoss_BatSwarm::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss_BatSwarm* pInstance = new CBoss_BatSwarm(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Boss_BatSwarm Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBoss_BatSwarm::Free()
{
	__super::Free();
}
