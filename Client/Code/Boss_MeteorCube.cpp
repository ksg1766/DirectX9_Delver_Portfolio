#include "Boss_MeteorCube.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "Player.h"
CBoss_MeteorCube::CBoss_MeteorCube(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBoss_MeteorCube::CBoss_MeteorCube(const CBoss_MeteorCube& rhs)
	:CGameObject(rhs)
{
}

CBoss_MeteorCube::~CBoss_MeteorCube()
{
	Free();
}

HRESULT CBoss_MeteorCube::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::MONSTERBULLET;
	FAILED_CHECK_RETURN(Add_Component(),E_FAIL);
	m_bChanneling_End = false;
	m_bMaxHeight = 12.f;
	m_fScale = 0;
	m_pCollider->InitOBB(
		m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
		m_pTransform->LocalScale() * 0.5);
	m_pBasicStat->Get_Stat()->fAttack = 15.0;
	m_pTransform->Scale(_vec3(0.6f, 0.6f, 0.6f));
	return S_OK;
}

_int CBoss_MeteorCube::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_PRIORITY, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_uint iExit = __super::Update_Object(fTimeDelta);
	
	if(STATE::BOSS_STURN == dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_StatMachine()->Get_State())
		Engine::EventManager()->DeleteObject(this);
	m_fScale += fTimeDelta/4.f;
	Channeling_End(fTimeDelta);
	Channeling_Now(fTimeDelta);
	m_pTransform->Translate(_vec3(-fTimeDelta / 4.f, 0.f, 0.f));
	if (4.f > m_fScale)
	{
		m_pTransform->Scale(_vec3(m_fScale, m_fScale, m_fScale));
	}
	return iExit;
}

void CBoss_MeteorCube::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
	__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);
}

void CBoss_MeteorCube::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pCubeBf->Render_Buffer();//큐브버퍼

#if _DEBUG
	m_pCollider->Render_Collider();
#endif

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBoss_MeteorCube::Channeling_Begin()
{
	m_vCenter = Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front()->m_pTransform->m_vInfo[INFO_POS];
	m_pTransform->m_vInfo[INFO_POS] = Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front()->m_pTransform->m_vInfo[INFO_POS];
	m_pTransform->Translate(_vec3 (4.f, 0.f, 0.f));
}

void CBoss_MeteorCube::Channeling_Now(const _float& fTimeDelta)
{
	if (m_bChanneling_End)
		return;
		m_pTransform->Translate(_vec3(0.f, 1.f * fTimeDelta, 0.f));
		m_pTransform->Rotate(_vec3(0.f, 0.f, 3.f));
		m_pTransform->RotateAround(m_vCenter, _vec3(0.f, 3.f, 0.f), 3.f * fTimeDelta/2.f);
		m_pBasicStat->Get_Stat()->fAttack *= m_fScale;
}

void CBoss_MeteorCube::Channeling_End(const _float& fTimeDelta)
{
	if (!m_bChanneling_End)
		return;
		_vec3 vTargetPos = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
		_vec3 vDir = vTargetPos - m_pTransform->m_vInfo[INFO_POS];
		m_pTransform->Translate(vDir * fTimeDelta);
		if (2.f > m_pTransform->m_vInfo[INFO_POS].y)
			Engine::EventManager()->DeleteObject(this);

}

void CBoss_MeteorCube::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		CPlayerStat& PlayerState = *(dynamic_cast<CPlayer*>(_pOther->GetHost())->Get_Stat());
		PlayerState.Take_Damage(this->Get_BasicStat()->Get_Stat()->fAttack);
		this->Set_AttackTick(true);
		//Engine::EventManager()->DeleteObject(this);
	}
}

void CBoss_MeteorCube::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

}

void CBoss_MeteorCube::OnCollisionExit(CCollider* _pOther)
{
	m_pBasicStat->Get_Stat()->fAttack = 15.f;
}

void CBoss_MeteorCube::Set_Center(_vec3 _CenterPos)
{
	m_pTransform->m_vInfo[INFO_POS] = _CenterPos;
}

HRESULT CBoss_MeteorCube::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pCubeBf = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);
	
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

CBoss_MeteorCube* CBoss_MeteorCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss_MeteorCube* pInstance = new CBoss_MeteorCube(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Meteor Cube Failed");
		return nullptr;
	}
	return pInstance;
}

void CBoss_MeteorCube::Free()
{
	__super::Free();
}
