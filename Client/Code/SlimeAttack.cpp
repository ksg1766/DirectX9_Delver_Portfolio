#include "..\Header\SlimeAttack.h"
#include "Export_Function.h"
#include "DungeonSpider.h"
#include "Player.h"

CSlimeAttack::CSlimeAttack()
{
}

CSlimeAttack::CSlimeAttack(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CSlimeAttack::~CSlimeAttack()
{
}

HRESULT CSlimeAttack::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fChase = 0.f;
	m_vPrevPos = _vec3(0.f, 0.f, 0.f);
	m_bIsAttack = false;
	m_fSpeed = 5.f;

	m_bAttackTick = false;


	return S_OK;
}

STATE CSlimeAttack::Update_State(const _float& fTimeDelta)
{
	CPlayer& pPlayer =
		*dynamic_cast<CPlayer*>
		(SceneManager()->GetInstance()->Get_ObjectList
		(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	if (!m_bIsAttack)
	{
		m_vPrevPos = pPlayer.m_pTransform->m_vInfo[INFO_POS];
		m_bIsAttack = true;
		m_bAttackTick = false;

	}

	if (m_bIsAttack)
	{
		_vec3 vDir =
			m_vPrevPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
		D3DXVec3Normalize(&vDir, &vDir);

		_float fAttackDistance = m_fSpeed * fTimeDelta;

		_vec3 vAttackPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + vDir * fAttackDistance;

		m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = vAttackPos;
	}


	_float fRange = D3DXVec3Length(&(m_vPrevPos - 
		m_pOwner->Get_Transform()->m_vInfo[INFO_POS]));

	if (fRange < 1.f 
		&& m_pOwner->Get_Animator()->Get_Animation()->Get_Frame() > 4.8f)
	{
		m_bIsAttack = false;
		m_pOwner->Set_State(STATE::ROMIMG);
		return STATE::ROMIMG;
	}

	return STATE::ATTACK;
}

void CSlimeAttack::LateUpdate_State()
{
}

void CSlimeAttack::Render_State()
{
	//cout << "Spider Jump" << endl;
}


CSlimeAttack* CSlimeAttack::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CSlimeAttack* pState = new CSlimeAttack(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CSlimeAttack*>(pState);

		MSG_BOX("Create Jump Failed");
		return nullptr;
	}

	return pState;
}

void CSlimeAttack::Free()
{
	__super::Free();
}
