#include "Boss_Lightning.h"
#include "Export_Function.h"
#include "Player.h"

CBoss_Lightning::CBoss_Lightning(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CMonster(pGraphicDev)
{
}

CBoss_Lightning::CBoss_Lightning(const CBoss_Lightning& rhs)
	:	Engine::CMonster(rhs)
{
}

CBoss_Lightning::~CBoss_Lightning()
{
	Free();
}

HRESULT CBoss_Lightning::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::MONSTERBULLET;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS]
		, &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());
	m_fTime = 0.f;
	m_bHit = false;
	m_fScale = 1.;
	m_pBasicStat->Get_Stat()->fAttack = 3.f;
	return S_OK;

}

_int CBoss_Lightning::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_int iExit = __super::Update_Object(fTimeDelta);
	m_fScale -= fTimeDelta;
	m_fTime += fTimeDelta;
	if ((1.f< m_fTime))
		EventManager()->DeleteObject(this);
	m_pTransform->Scale(_vec3(m_fScale, 1.f, m_fScale));

	return iExit;
}

void CBoss_Lightning::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
}

void CBoss_Lightning::Render_Object(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();

	m_pTransform->Rotate(ROT_Y, D3DXToRadian(90.f));
	_matrix matWorld = m_pTransform->WorldMatrix();
	m_pTransform->Rotate(ROT_Y, D3DXToRadian(-90.f));

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

void CBoss_Lightning::Init_Stat()
{
}

void CBoss_Lightning::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CBoss_Lightning::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if (m_bHit) { return; }
	if (OBJECTTAG::PLAYER == _pOther->Get_Host()->Get_ObjectTag())
	{
		CPlayerStat& PlayerState = *(dynamic_cast<CPlayer*>(_pOther->Get_Host())->Get_Stat());
		PlayerState.Take_Damage(this->Get_BasicStat()->Get_Stat()->fAttack);
		this->Set_AttackTick(true);

		_vec3 m_fDis = (dynamic_cast<CPlayer*>(_pOther->Get_Host())->m_pTransform->m_vInfo[INFO_LOOK] * 2.f);
		(dynamic_cast<CPlayer*>(_pOther->Get_Host())->Get_RigidBody()->Add_Force(_vec3(-m_fDis.x, 3.f, -m_fDis.z)));
		(dynamic_cast<CPlayer*>(_pOther->Get_Host())->Get_RigidBody()->UseGravity(true));
		(dynamic_cast<CPlayer*>(_pOther->Get_Host())->Set_JumpState(true));
		m_bHit = true;
	}
}

void CBoss_Lightning::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

HRESULT CBoss_Lightning::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossLightning"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	//pComponent = m_pBillBoard = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

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

CBoss_Lightning* CBoss_Lightning::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss_Lightning* pInstance = new CBoss_Lightning(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BossLightning Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBoss_Lightning::Free()
{
	__super::Free();
}
