#include "stdafx.h"
#include "SoundManager.h"
#include "Puzzle.h"
#include "Export_Function.h"
#include "Player.h"
#include "Puzzle_Part.h"

CPuzzle::CPuzzle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev)
{
}

CPuzzle::CPuzzle(const CPuzzle& rhs)
	: CItem(rhs)
{
}

CPuzzle::~CPuzzle()
{
	Free();
}

HRESULT CPuzzle::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::ITEM;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_ItemID.eItemType = ITEMTYPE::ITEMTYPE_POTIONITEM;
	m_ItemID.eItemID = ITEMID::EAT_POTION7;
	m_ItemID.iCount = 1;

	return S_OK;
}

_int CPuzzle::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);

	if (!m_bCreate)
	{
		for (_uint i = 0; i < 6; ++i)
		{
			CGameObject* pPuzzlePart = nullptr;
			pPuzzlePart = CPuzzle_Part::Create(m_pGraphicDev, i);
			pPuzzlePart->m_pTransform->Translate(_vec3(-72.f, 12.f, -9.f + i));
			EventManager()->CreateObject(pPuzzlePart, LAYERTAG::GAMELOGIC);
			m_vecPuzzlePart.push_back(dynamic_cast<CPuzzle_Part*>(pPuzzlePart));
		}

		m_bCreate = true;
	}

	return iExit;
}

void CPuzzle::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
}

void CPuzzle::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();
}

HRESULT CPuzzle::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_QuestItemBackGround"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	if (Get_WorldItem())
	{
		pComponent = m_pBillBoard = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);
	}

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);


	return S_OK;
}

void CPuzzle::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER) &&
		!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER))
		__super::OnCollisionEnter(_pOther);
}

void CPuzzle::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CPuzzle::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}


CPuzzle* CPuzzle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPuzzle* pInstance = new CPuzzle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CPuzzle*>(pInstance);

		MSG_BOX("Bear Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPuzzle::Free()
{
	__super::Free();
}
