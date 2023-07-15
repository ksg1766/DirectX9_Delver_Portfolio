#include "..\Header\Magic_Ball.h"
#include "Export_Function.h"
#include "Terrain.h"
#include "Player.h"

CMagic_Ball::CMagic_Ball(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CMonster(pGraphicDev)
{

}

CMagic_Ball::CMagic_Ball(const CMagic_Ball& rhs)
	: Engine::CMonster(rhs)
{
}

CMagic_Ball::~CMagic_Ball()
{
	Free();
}

HRESULT CMagic_Ball::Ready_Object(CTransform* pOwner, _float _fSpeed)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_eObjectTag = OBJECTTAG::MONSTERBULLET;
	Set_State(STATE::ATTACK);

	m_bCheck = false;
	m_bIsAttack = false;
	m_vPrevPos = _vec3(0.f, 0.f, 0.f);
	m_pTransform->Set_Parent(pOwner);
	m_pTransform->Scale(_vec3(0.8f, 0.8f, 0.8f));

	m_pCollider->InitOBB(
		m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
		m_pTransform->LocalScale() * 0.5);
	
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

	m_pBasicStat->Get_Stat()->fAttack = 1.f;

	return S_OK;
}

_int CMagic_Ball::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);



	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->GetInstance()->
		Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());
	
	m_pAnimator->Update_Animator(fTimeDelta);

	if (!m_bCheck)
	{
		if (m_pAnimator->Get_Animation()->Get_Frame() >= 1)
			m_pAnimator->Get_Animation()->Set_Loop(FALSE);
	}

	if (!m_bIsAttack)
	{
		m_vInit = m_pTransform->m_vInfo[INFO_POS];
		m_vPrevPos = pPlayer.m_pTransform->m_vInfo[INFO_POS];
		m_bIsAttack = true;
	}

	_vec3 vDir = m_vPrevPos - m_vInit;
	D3DXVec3Normalize(&vDir, &vDir);
	
	m_pTransform->m_vInfo[INFO_POS] = m_pTransform->m_vInfo[INFO_POS] + vDir * m_fSpeed * fTimeDelta;




	return _int();
}

void CMagic_Ball::LateUpdate_Object()
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
	__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);

}

void CMagic_Ball::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pAnimator->Render_Animator();
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

void CMagic_Ball::ForceHeight(_vec3 _vPos)
{
	_float x = (VTXCNTX * VTXITV / 2.f) + _vPos.x;
	_float z = (VTXCNTZ * VTXITV / 2.f) + _vPos.z;

	x /= (_float)VTXITV;
	z /= (_float)VTXITV;

	_int col = ::floorf(x);
	_int row = ::floorf(z);

	_vec3 A = m_pTerrain->LoadTerrainVertex()[row * VTXCNTX + col];
	_vec3 B = m_pTerrain->LoadTerrainVertex()[row * VTXCNTX + col + 1];
	_vec3 C = m_pTerrain->LoadTerrainVertex()[(row + 1) * VTXCNTX + col];
	_vec3 D = m_pTerrain->LoadTerrainVertex()[(row + 1) * VTXCNTX + col + 1];

	_float dx = x - col;
	_float dz = z - row;

	_float height;
	//c-d b-d cdb 
	if (dz < 1.0f - dx)
	{
		/*
		Lerp(_float _a, _float _b, _float _c)
		{
			return a - (a * t) + (b * t);
		}
		*/

		_vec3 uy = B - A;
		_vec3 vy = C - A;

		height = A.y + (uy.y * dx) + (vy.y * dz) + 1.f;
		m_pTransform->m_vInfo[INFO_POS].y = height;
	}// c-a b-a cba
	else
	{
		_vec3 uy = C - D;
		_vec3 vy = B - D;

		height = D.y + (uy.y * (1.f - dx)) + (vy.y * (1.f - dz)) + 1.f;
		m_pTransform->m_vInfo[INFO_POS].y = height;
	}
}

void CMagic_Ball::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	//if (_pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::ITEM)	
	//	__super::OnCollisionEnter(_pOther);

	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BLOCK && this->Get_State() != STATE::DEAD)
	{
		Set_State(STATE::DEAD);
		m_pAnimator->Set_Animation(STATE::DEAD);
		m_bCheck = true;

		cout << "벽돌충돌 " << endl;
	}
		

	if (_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::PLAYER && this->Get_State() != STATE::DEAD)
	{
		CPlayerStat& PlayerState = *dynamic_cast<CPlayer*>(_pOther->GetHost())->Get_Stat();

		PlayerState.Take_Damage(this->Get_BasicStat()->Get_Stat()->fAttack);
		this->Set_AttackTick(true);

		cout << "마법구 데미지" << endl;

		Set_State(STATE::DEAD);
		m_pAnimator->Set_Animation(STATE::DEAD);

		m_bCheck = true;
	}
}

void CMagic_Ball::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }


	if (!(_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::ITEM))
		__super::OnCollisionStay(_pOther);

	cout << "마법구 데미지" << endl;

	if (m_pAnimator->Get_Animation()->Get_Frame() >= 1)
	{
		m_pAnimator->Get_Animation()->Set_Loop(FALSE);
		EventManager()->GetInstance()->DeleteObject(this);
	}

}

void CMagic_Ball::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }


}

HRESULT CMagic_Ball::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture[(_uint)STATE::ATTACK] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_MonsterBullet"));
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

CMagic_Ball* CMagic_Ball::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pOwner, _float _fSpeed)
{
	CMagic_Ball* pInstance = new CMagic_Ball(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pOwner, _fSpeed)))
	{
		Safe_Release<CMagic_Ball*>(pInstance);

		MSG_BOX("Create MagicBall Failed");
		return nullptr;
	}

	return pInstance;
}

void CMagic_Ball::Free()
{
	__super::Free();

}
