#include "..\Header\Ghost_IDLE.h"
#include "Export_Function.h"
#include "Player.h"

CGhost_IDLE::CGhost_IDLE()
{
}

CGhost_IDLE::CGhost_IDLE(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CGhost_IDLE::~CGhost_IDLE()
{
}

HRESULT CGhost_IDLE::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fChase = 0.f;

	return S_OK;
}

STATE CGhost_IDLE::Update_State(const _float& fTimeDelta)
{

	CPlayer& pPlayer =
		*static_cast<CPlayer*>
		(SceneManager()->GetInstance()->Get_ObjectList
		(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	_vec3 fDistance = m_pOwner->Get_Host()->m_pTransform->m_vInfo[INFO_POS] - pPlayer.m_pTransform->m_vInfo[INFO_POS];
	_float fLength = D3DXVec3Length(&fDistance);

	if (fLength < 10.f)
	{
		return STATE::ATTACK;
	}

	return STATE::IDLE;
}

void CGhost_IDLE::LateUpdate_State()
{
}

void CGhost_IDLE::Render_State()
{
}

CGhost_IDLE* CGhost_IDLE::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CGhost_IDLE* pState = new CGhost_IDLE(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CGhost_IDLE*>(pState);

		MSG_BOX("Create GhostIDLE Failed");
		return nullptr;
	}

	return pState;
}

void CGhost_IDLE::Free()
{
	__super::Free();
}
