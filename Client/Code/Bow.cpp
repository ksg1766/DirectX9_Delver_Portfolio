#include "stdafx.h"
#include "..\Header\Player.h"

#include "..\Header\Bow.h"
#include "Export_Function.h"

static _int iCount = 0;

CBow::CBow(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CItem(pGraphicDev)
{
}

CBow::CBow(const CBow& rhs)
	: Engine::CItem(rhs)
{
}

CBow::~CBow()
{
	Free();
}

HRESULT CBow::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::ITEM;
	m_eItemTag = ITEMTAG::WEAPON;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pTransform->m_pParent->Get_Host());
	m_pTransform->Copy_RUL(pPlayer->m_pTransform->m_vInfo);
	m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));

	m_pTransform->Translate(pPlayer->m_pTransform->m_vInfo[INFO_POS] + *dynamic_cast<CPlayer*>(pPlayer)->Get_Offset());
	m_fSignTime = 1.f;
	m_fChase = 0.f;
	m_fChase2 = 0.f;
	m_fAngle = 0.f;
	m_iCount = 0;
	// 타입 및 아이디 지정
	m_ItemID.eItemType = ITEMTYPE_WEAPONITEM;
	m_ItemID.eItemID = WEAPON_BOW;
	m_ItemID.iCount = 1;

	return S_OK;
}

_int CBow::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pTransform->m_pParent->Get_Host());

	if (pPlayer->Get_Attack())
	{
		pPlayer->Set_Attack(false);

		m_fChase += 0.08 * fTimeDelta;

		if (m_fChase >= 1.f)
			m_fChase = 0.f;

		if (m_iCount < 55)
		{
			//m_pTransform->Rotate(ROT_Y, 0.04f);
			++m_iCount;

			_vec3 vLerp;
			D3DXVec3Lerp(&vLerp, &m_pTransform->m_vInfo[INFO_POS],
				&(m_pTransform->m_pParent->m_vInfo[INFO_POS] + 2.f * m_pTransform->m_pParent->m_vInfo[INFO_LOOK]),
				m_fChase);
			m_pTransform->m_vInfo[INFO_POS] = vLerp;
		}
	}
	else
	{
		m_fChase2 += 0.08 * fTimeDelta;

		if (m_fChase2 >= 1.f)
			m_fChase2 = 0.f;


		if (m_iCount > 0)
		{
			//m_pTransform->Rotate(ROT_Y, -0.04f);
			++m_iCount;

			_vec3 vLerp;
			D3DXVec3Lerp(&vLerp, &m_pTransform->m_vInfo[INFO_POS],
				&(m_pTransform->m_pParent->m_vInfo[INFO_POS] + *pPlayer->Get_Offset()),
				m_fChase2);
			m_pTransform->m_vInfo[INFO_POS] = vLerp;
		}
	}



	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CBow::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
	__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);
}


void CBow::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

//
//#if _DEBUG
//	m_pCollider->Render_Collider();
//#endif

	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBow::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Bow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	m_pTransform->Set_Parent(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform);
	m_pTransform->Copy_RUL(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo);

	return S_OK;
}

void CBow::OnCollisionEnter(CCollider* _pOther)
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
				&& iCount > 4)
			{
				iCount = 0;
				_pOther->Get_Host()->Get_StateMachine()->Set_State(STATE::HIT);
			}


			cout << "데미지" << endl;
		}
	}
}

void CBow::OnCollisionStay(CCollider* _pOther)
{
}

void CBow::OnCollisionExit(CCollider* _pOther)
{
}

CBow* CBow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBow* pInstance = new CBow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("TempItem Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBow::Free()
{
	__super::Free();
}
