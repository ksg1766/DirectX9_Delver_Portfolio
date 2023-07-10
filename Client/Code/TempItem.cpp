#include "stdafx.h"
#include "..\Header\Player.h"

#include "..\Header\TempItem.h"
#include "Export_Function.h"

CTempItem::CTempItem(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CItem(pGraphicDev)
{
}

CTempItem::CTempItem(const CTempItem& rhs)
	: Engine::CItem(rhs)
{
}

CTempItem::~CTempItem()
{
}

HRESULT CTempItem::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::ITEM;
	m_eItemTag = ITEMTAG::WEAPON;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
	m_pCollider->
		InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], 2 * m_pTransform->LocalScale());

	m_pBasicStat->Get_Stat()->fAttack = 1.f;
	m_pBasicStat->Get_Stat()->fHealth = 3.f;

	CGameObject* pGamePlayer = SceneManager()->GetInstance()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front();
	m_pTransform->Translate(pGamePlayer->m_pTransform->m_vInfo[INFO_POS] + *dynamic_cast<CPlayer*>(pGamePlayer)->Get_Offset());

	m_AttackInfo.fWeaponSpeed = 0.08f;
	m_AttackInfo.fReturnSpeed = 0.08f;
	m_AttackInfo.fMaxDistance = 0.8f;

	return S_OK;
}

_int CTempItem::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->GetInstance()
		->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	if (pPlayer.Get_Attack())
	{
		if (!m_AttackInfo.bReturn)
		{
		
			m_pTransform->Translate(Get_RotationDir(pPlayer, fTimeDelta) * m_AttackInfo.fReturnSpeed);


		}

		else
		{
			Get_ReturnTrans(pPlayer, fTimeDelta);
		}

		_vec3 vWeaponToPlayer = pPlayer.m_pTransform->m_vInfo[INFO_POS] + *pPlayer.Get_Offset() - m_pTransform->m_vInfo[INFO_POS];
		_float fDistanceToPlayer = D3DXVec3Length(&vWeaponToPlayer);

		if (fDistanceToPlayer >= m_AttackInfo.fMaxDistance) 
			// 끝까지 도달한 지점이 최대 사거리에 온다면 다시 돌아가야함.
		{
			// 무기가 플레이어 위치로 돌아오도록 설정
			m_AttackInfo.bReturn = true;
		}
	}


	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CTempItem::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

}


void CTempItem::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif

	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTempItem::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Sword"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);
	
	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	m_pTransform->Set_Parent(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform);
	m_pTransform->Copy_RUL(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo);


	return S_OK;
}

void CTempItem::OnCollisionEnter(CCollider* _pOther)
{
	if (!(_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::MONSTER) && 
		!(_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::PLAYER))
		__super::OnCollisionEnter(_pOther);
	// 몬스터거나 플레이어면 밀어내지않는다.

	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->GetInstance()
		->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());
	// 플레이어의 정보를 레퍼런스로 얻어옴.

	if (_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::MONSTER)
		// 무기 콜리전에 들어온 타입이 몬스터이면서, 플레이어의 스테이트가 공격이라면
	{
		if (!pPlayer.Get_AttackTick())
			// 공격 하지 않은 상태라면.
		{
			_pOther->Get_Host()->Get_BasicStat()->Take_Damage(1.f);
			// 몬스터의 스텟에 데미지 1을 준다.
			pPlayer.Set_AttackTick(true);
			// 그리고 다시 틱을 true로 해서 때렸다는 것을 알려줌.

			cout << "데미지" << endl;
		}
	}



}

void CTempItem::OnCollisionStay(CCollider* _pOther)
{
}

void CTempItem::OnCollisionExit(CCollider* _pOther)
{
}

_vec3 CTempItem::Get_RotationDir(const CPlayer& pPlayer, const _float& fTimeDelta)
{
	m_AttackInfo.vFoward = pPlayer.m_pTransform->m_vInfo[INFO_LOOK];
	// 무기의 전방 벡터를 지정한다. -> 플레이어가 바라보고 있는 방향

	m_AttackInfo.vRotation = D3DXVECTOR3(0.0f, atan2(m_AttackInfo.vFoward.x, m_AttackInfo.vFoward.z), 0.0f);
	// 내가 회전함에 따라서 회전 값을 계산해야하는데 
	// 트랜스폼엔 앵글값을 불러 올 수 없으므로, 플레이어가 바라보는 방향 벡터로 역삼각함수를 이용해서 각도를 구함
	// 반환 값 Radian이고 atan2를 사용해서 x-z 각도를 구함

	D3DXMatrixRotationYawPitchRoll(&m_AttackInfo.matRot, m_AttackInfo.vRotation.y, m_AttackInfo.vRotation.x, m_AttackInfo.vRotation.z);
	// 매트릭스에 y축 회전값을 담은 Rotation으로 내가 바라보는 방향에서 y,x,z축의 회전을 지정하고 
	// 회전행렬에 사용한다.

	_vec3 vFowardDir = _vec3(0.0f, 0.0f, 1.0f); 
	// 무기의 이동 방향

	D3DXVec3TransformNormal(&vFowardDir, &vFowardDir, &m_AttackInfo.matRot);
	// 내 전방벡터 기준 y축 회전을 기준으로 한 매트릭스로 전방벡터의 방향으로 바꿈

	D3DXVec3Normalize(&vFowardDir, &vFowardDir);
	// 노멀라이즈해서 단위벡터로 만 듦

	return vFowardDir; 
}

void CTempItem::Get_ReturnTrans(CPlayer& pPlayer, const _float& fTimeDelta)
{
	_vec3 vWeaponToPlayer = (pPlayer.m_pTransform->m_vInfo[INFO_POS] + *pPlayer.Get_Offset()) - m_pTransform->m_vInfo[INFO_POS];
	// 끝에 도달했을 경우 무기의 위치에서 내 위치를 빼면 방향벡터가 나옴.

	// 무기가 플레이어 위치에 도달한 경우
	// 방향벡터보다 돌아오는 속도가 크다면 
	if (D3DXVec3Length(&vWeaponToPlayer) <= m_AttackInfo.fReturnSpeed)
	{
	
		m_AttackInfo.bReturn = false;
		m_pTransform->m_vInfo[INFO_POS] = pPlayer.m_pTransform->m_vInfo[INFO_POS] + *pPlayer.Get_Offset();
		// 무기가 돌아온 상태를 표시하고, 위치를 플레이어 위치로 설정

		//m_pTransform->Rotate(_vec3(0.0f, -interpolatedAngle * fTimeDelta, 0.f));
		pPlayer.Set_Attack(false);
		// 돌아왔으므로 공격상태를 false로 만들어서 다시 공격할 수 있게 만든다.
	}
	else
	{
		// 무기를 플레이어 위치로 이동
		D3DXVec3Normalize(&vWeaponToPlayer, &vWeaponToPlayer);
		// 정규화 시켜서 단위벡터로 만든다.
		m_pTransform->Translate(vWeaponToPlayer * m_AttackInfo.fReturnSpeed);

		// 중간 각도를 회전에 적용

		//m_pTransform->Rotate(_vec3(0.0f, -interpolatedAngle * fTimeDelta, 0.f));
	}

	
}

CTempItem* CTempItem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTempItem* pInstance = new CTempItem(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("TempItem Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CTempItem::Free()
{
	__super::Free();
}
