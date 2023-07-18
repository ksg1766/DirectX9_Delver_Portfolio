#include "..\Header\Monster_Hit.h"
#include "Export_Function.h"
#include "Player.h"

CMonster_Hit::CMonster_Hit()
{
}

CMonster_Hit::CMonster_Hit(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CMonster_Hit::~CMonster_Hit()
{
}

HRESULT CMonster_Hit::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fChase = 0.f;


	m_bCanHitState = true;
	m_fHitCoolDown = 0.f;
	m_iHitCount = 0;
	return S_OK;
}

STATE CMonster_Hit::Update_State(const _float& fTimeDelta)
{
	
	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->GetInstance()
		->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	CItem* ItemType = dynamic_cast<CItem*>(pPlayer.Get_CurrentEquipRight());
	ITEMTYPEID ItemID = {};

	m_fChase += fTimeDelta;


	if (m_fChase <= 0.5f)
	{
		if(ItemID.eItemID == ITEMID::WEAPON_SWORD)
		m_pOwner->Get_Host()->m_pTransform->m_vInfo[INFO_POS] += 
			pPlayer.m_pTransform->m_vInfo[INFO_LOOK] * 5 * fTimeDelta;

		if (ItemID.eItemID == ITEMID::WEAPON_BOW)
			m_pOwner->Get_Host()->m_pTransform->m_vInfo[INFO_POS] +=
			pPlayer.m_pTransform->m_vInfo[INFO_LOOK] * 0.5 * fTimeDelta;

	
	}
	else
	{
		m_fChase = 0.f;
		return STATE::ROMIMG;
	}




}

void CMonster_Hit::LateUpdate_State()
{
}

void CMonster_Hit::Render_State()
{
}

CMonster_Hit* CMonster_Hit::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CMonster_Hit* pState = new CMonster_Hit(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CMonster_Hit*>(pState);

		MSG_BOX("Create Monster Hit Failed");
		return nullptr;
	}

	return pState;
}

void CMonster_Hit::Free()
{
	__super::Free();
}
