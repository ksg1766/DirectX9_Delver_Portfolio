#include "stdafx.h"
#include "SoundManager.h"
#include "Puzzle_Part.h"
#include "Export_Function.h"
#include "Player.h"

CPuzzle_Part::CPuzzle_Part(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev)
{
}

CPuzzle_Part::CPuzzle_Part(const CPuzzle_Part& rhs)
	: CItem(rhs)
{
}

CPuzzle_Part::~CPuzzle_Part()
{
	Free();
}

HRESULT CPuzzle_Part::Ready_Object(_uint _PuzzleNumber)
{
	m_eObjectTag = OBJECTTAG::ITEM;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (!Get_WorldItem())
	{
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
		m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
			m_pTransform->LocalScale());

		CGameObject* pPlayer = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front();

		if (pPlayer != nullptr)
		{
#pragma region HpPotion
			m_pBasicStat->Get_Stat()->fMaxHP = dynamic_cast<CPlayer*>(pPlayer)->Get_Stat()->Get_Stat()->fMaxHP;
#pragma endregion
		}
	}
	else
	{
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
	}

	Set_PuzzleNumber(_PuzzleNumber);

	m_ItemID.eItemType = ITEMTYPE::ITEMTYPE_QUEST;
	m_ItemID.eItemID = (ITEMID)(QUEST_PART1 + Get_PuzzleNumber());
	m_ItemID.iCount = 1;

	return S_OK;
}

_int CPuzzle_Part::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);


	return iExit;
}

void CPuzzle_Part::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
	//	__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);
}

void CPuzzle_Part::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(m_iPuzzleNumber);
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif // _DEBUG

}

HRESULT CPuzzle_Part::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_QuestItem"));
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

void CPuzzle_Part::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER) &&
		!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER))
		__super::OnCollisionEnter(_pOther);
}

void CPuzzle_Part::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CPuzzle_Part::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

CPuzzle_Part* CPuzzle_Part::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint _PuzzleNumber)
{
	CPuzzle_Part* pInstance = new CPuzzle_Part(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_PuzzleNumber)))
	{
		Safe_Release<CPuzzle_Part*>(pInstance);

		MSG_BOX("Bear Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPuzzle_Part::Free()
{
	__super::Free();
}
