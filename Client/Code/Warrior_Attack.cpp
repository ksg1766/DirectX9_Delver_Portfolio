#include "..\Header\Warrior_Attack.h"
#include "Export_Function.h"
#include "DungeonSpider.h"
#include "Player.h"

CWarror_Attack::CWarror_Attack()
{
}

CWarror_Attack::CWarror_Attack(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CWarror_Attack::~CWarror_Attack()
{
}

HRESULT CWarror_Attack::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fChase = 0.f;
	m_vPrevPos = _vec3(0.f, 0.f, 0.f);
	m_bIsAttack = false;
	m_fSpeed = 10.f;

	m_bAttackTick  = false;
	m_bAttackTick2 = false;

	return S_OK;
}

STATE CWarror_Attack::Update_State(const _float& fTimeDelta)
{
	CPlayer& pPlayer = 
		*dynamic_cast<CPlayer*>
		(SceneManager()->GetInstance()->Get_ObjectList
		(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());
	
	if (!m_bIsAttack)
	{
		m_vPrevPos = pPlayer.m_pTransform->m_vInfo[INFO_POS];
		m_bIsAttack = true;
	}

	_vec3 vDir = 
		m_vPrevPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&vDir, &vDir);

	_float fAttackDistance = m_fSpeed * fTimeDelta;
	_vec3 vAttackPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + vDir * fAttackDistance;

	m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = vAttackPos;

	_float fRange = D3DXVec3LengthSq(&(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]
		- m_vPrevPos));

	if (fRange <= 4.8f && m_pOwner->Get_Animator()->Get_Animation()->Get_Frame() > 4.8)
	{
		m_bIsAttack = false;
		m_bAttackTick = false;
		m_pOwner->Set_State(STATE::ROMIMG);
		return STATE::ROMIMG;
	}


	m_pOwner->Set_State(STATE::ATTACK);

	return STATE::ATTACK;
}

void CWarror_Attack::LateUpdate_State()
{
}

void CWarror_Attack::Render_State()
{
	//cout << "Spider Jump" << endl;
}


CWarror_Attack* CWarror_Attack::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CWarror_Attack* pState = new CWarror_Attack(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CWarror_Attack*>(pState);

		MSG_BOX("Create Jump Failed");
		return nullptr;
	}

	return pState;
}

void CWarror_Attack::Free()
{
	__super::Free();
}
