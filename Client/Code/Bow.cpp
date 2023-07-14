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
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	if (pPlayer->Get_CurrentEquipRight() == nullptr)
	{
		//m_pTransform->Copy_RUL(pPlayer->m_pTransform->m_vInfo);
		

		m_pTransform->Translate(pPlayer->m_pTransform->m_vInfo[INFO_POS] + *dynamic_cast<CPlayer*>(pPlayer)->Get_Offset());
		m_fSignTime = 1.f;
		m_fChase = 0.f;
		m_fChase2 = 0.f;
		m_fAngle = 0.f;
		m_iCount = 0;
	}
	//CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pTransform->m_pParent->Get_Host());

	// 타입 및 아이디 지정
	m_ItemID.eItemType = ITEMTYPE_WEAPONITEM;
	m_ItemID.eItemID = WEAPON_BOW;
	m_ItemID.iCount = 1;

	return S_OK;
}

_int CBow::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	
	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pTransform->m_pParent->Get_Host());
	_vec3 vUp = *D3DXVec3Cross(&vUp, &m_pTransform->m_pParent->m_vInfo[INFO_LOOK],
		&m_pTransform->m_pParent->m_vInfo[INFO_RIGHT]);
	_matrix matRot;
	_vec3 vLook = *D3DXVec3Cross(&vLook, &m_pTransform->m_pParent->m_vInfo[INFO_UP],
		&m_pTransform->m_pParent->m_vInfo[INFO_RIGHT]);


	if (pPlayer->Get_Attack())
	{
		if (m_iCount < 28.f)
		{
			++m_iCount;
			m_pTransform->RotateAround(m_pTransform->m_pParent->m_vInfo[INFO_POS],
				vUp, -0.01f);	
			m_pTransform->RotateAround(m_pTransform->m_vInfo[INFO_POS], m_pTransform->m_vInfo[INFO_UP],
				0.08f);
		}
		pPlayer->Set_Attack(false);

	}
	else
	{
		if (m_iCount > 0.f)
		{
			--m_iCount;
			m_pTransform->RotateAround(m_pTransform->m_pParent->m_vInfo[INFO_POS],
				vUp, 0.01f);
			m_pTransform->RotateAround(m_pTransform->m_vInfo[INFO_POS], m_pTransform->m_vInfo[INFO_UP],
				-0.08f);

			
		}
	}


	return iExit;
}

void CBow::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

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

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	if (pPlayer->Get_CurrentEquipRight() == nullptr)
	{
		m_pTransform->Set_Parent(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform);
		m_pTransform->Copy_RUL(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo);
	}

	return S_OK;
}

void CBow::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (!(_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::MONSTER) &&
		!(_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::PLAYER))
		__super::OnCollisionEnter(_pOther);
	// 몬스터거나 플레이어면 밀어내지않는다.
}

void CBow::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CBow::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
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
