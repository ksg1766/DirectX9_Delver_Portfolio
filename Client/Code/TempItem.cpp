#include "stdafx.h"
#include "..\Header\Player.h"

#include "..\Header\TempItem.h"
#include "Export_Function.h"

static _int iCount = 0;

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
		InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

	m_pBasicStat->Get_Stat()->fAttack = 1.f;
	m_pBasicStat->Get_Stat()->fHealth = 20.f;

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pTransform->m_pParent->Get_Host());
	m_pTransform->Translate(pPlayer->m_pTransform->m_vInfo[INFO_POS] + *dynamic_cast<CPlayer*>(pPlayer)->Get_Offset());

	m_AttackInfo.fWeaponSpeed = 0.08f;
	m_AttackInfo.fReturnSpeed = 0.08f;
	m_AttackInfo.fMaxDistance = 0.8f;

	m_fSignTime = 1.f;

	return S_OK;
}

_int CTempItem::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pTransform->m_pParent->Get_Host());

#pragma region ksg
	if (pPlayer->Get_Attack())
	{
		if (2.3f < D3DXVec3Length(&(m_pTransform->m_pParent->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS])))
			m_fSignTime = -1.f;
		else if (1.8848f > D3DXVec3Length(&(m_pTransform->m_pParent->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS])))
		{
			m_fSignTime = 1.f;
			pPlayer->Set_Attack(false);
		}
		m_pTransform->Translate(m_pTransform->m_vInfo[INFO_LOOK] * m_fSignTime * 15.f * fTimeDelta);

		// 1.8848은 그냥 D3DXVec3Length(&오프셋) 하셔서 바꿔주시면 돼요. 2.3f는 적당히 사거리 더해서 하심 됩니다.
#pragma endregion ksg
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

		if (!pPlayer.Get_AttackTick() && 
			_pOther->Get_Host()->Get_StateMachine()->Get_State() != STATE::DEAD)
			// 공격 하지 않은 상태라면.
		{
			_pOther->Get_Host()->Get_BasicStat()->Take_Damage(1.f);
			pPlayer.Set_AttackTick(true);

			++iCount;

			if (_pOther->Get_Host()->Get_StateMachine()->Get_PrevState() != STATE::HIT
				&& iCount > 3)
			{
				iCount = 0;
				_pOther->Get_Host()->Get_StateMachine()->Set_State(STATE::HIT);
			}


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
