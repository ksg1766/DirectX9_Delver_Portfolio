#include "Ghost_Bullet.h"
#include "Export_Function.h"
#include "Player.h"

CGhost_Bullet::CGhost_Bullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CMonster(pGraphicDev)
{

}

CGhost_Bullet::CGhost_Bullet(const CGhost_Bullet& rhs)
	: Engine::CMonster(rhs)
{
}

CGhost_Bullet::~CGhost_Bullet()
{
	Free();
}

HRESULT CGhost_Bullet::Ready_Object(CTransform* pOwner, _float _fSpeed, _vec3 _vOffset)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_eObjectTag = OBJECTTAG::MONSTERBULLET;
	Set_State(STATE::ATTACK);

	m_bCheck = false;
	m_bIsAttack = false;
	m_vPrevPos = _vec3(0.f, 0.f, 0.f);
	m_pTransform->Scale(_vec3(0.8f, 0.8f, 0.8f));

	m_pCollider->InitOBB(
		m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
		m_pTransform->LocalScale());


	m_pTransform->Copy_RUL_AddPos(pOwner->Get_Transform()->m_vInfo);

	m_fFrame = 0.f;
	m_fSpeed = _fSpeed;

	CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::ATTACK], STATE::ATTACK, 5.f, TRUE);
	m_pAnimator->Add_Animation(STATE::ATTACK, pAnimation);
	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::DEAD], STATE::DEAD, 5.f, TRUE);
	m_pAnimator->Add_Animation(STATE::DEAD, pAnimation);

	m_pAnimator->Set_Animation(STATE::ATTACK);
	m_pTransform->m_vInfo[INFO_POS] += _vOffset;

	m_pBasicStat->Get_Stat()->iDamageMax = 5.f;
	m_pBasicStat->Get_Stat()->iDamageMin = 2.f;
	m_pBasicStat->Get_Stat()->fHP = 10.f;

	return S_OK;
}

_int CGhost_Bullet::Update_Object(const _float& fTimeDelta)
{
	if (IsDead())
		return 0;

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);



	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->GetInstance()->
		Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	m_pAnimator->Update_Animator(fTimeDelta);

	if (!m_bCheck)
	{
		if (m_pAnimator->Get_Animation()->Get_Frame() >= 2)
			m_pAnimator->Get_Animation()->Set_Loop(FALSE);
	}

	if (!m_bIsAttack)
	{
		m_vInit = m_pTransform->m_vInfo[INFO_POS];
		m_vPrevPos = pPlayer.m_pTransform->m_vInfo[INFO_POS];
		m_bIsAttack = true;
	}

	_float fDistance = D3DXVec3Length(&(m_pTransform->m_vInfo[INFO_POS] - m_vInit));

	if (fDistance > 50.f && Get_State() != STATE::DEAD)
	{
		Set_State(STATE::DEAD);
		EventManager()->DeleteObject(this);
	}


	_vec3 vDir = m_vPrevPos - m_vInit;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Translate(vDir * m_fSpeed * fTimeDelta);

	return _int();
}

void CGhost_Bullet::LateUpdate_Object()
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
	//__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);

}

void CGhost_Bullet::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pAnimator->Render_Animator();
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

void CGhost_Bullet::Init_Stat()
{
}

void CGhost_Bullet::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::ITEM
		&& _pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYER
		&& _pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::MONSTER)
		__super::OnCollisionEnter(_pOther);

	if ((_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BLOCK || _pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
		&& this->Get_State() != STATE::DEAD)
	{
		CPlayerStat& PlayerStat = *static_cast<CPlayer*>(_pOther->Get_Host())->Get_Stat();
		this->Set_AttackTick(true);
		IsAttack(&PlayerStat);

		//cout << "마법구 충돌" << endl;

		Set_State(STATE::DEAD);
		m_pAnimator->Set_Animation(STATE::DEAD);
		m_bCheck = true;
		EventManager()->DeleteObject(this);
	}
}

void CGhost_Bullet::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }


	if (_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::ITEM
		&& _pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYER
		&& _pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::MONSTER)
		__super::OnCollisionStay(_pOther);




}

void CGhost_Bullet::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }


}

HRESULT CGhost_Bullet::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture[(_uint)STATE::ATTACK] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_MonsterLeafBullet"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::DEAD] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_MonsterBulletDelete"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::ANIMATOR, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CGhost_Bullet* CGhost_Bullet::Create(
	LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pOwner, _float _fSpeed, _vec3 _vOffset)
{
	CGhost_Bullet* pInstance = new CGhost_Bullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pOwner, _fSpeed, _vOffset)))
	{
		Safe_Release<CGhost_Bullet*>(pInstance);

		MSG_BOX("Create MagicBall Failed");
		return nullptr;
	}

	return pInstance;
}

void CGhost_Bullet::Free()
{
	__super::Free();

}
