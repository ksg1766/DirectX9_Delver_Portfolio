#include "..\Header\Ghost_Attack.h"
#include "Export_Function.h"
#include "Player.h"
#include "Ghost_Bullet.h"

CGhost_Attack::CGhost_Attack()
{
}

CGhost_Attack::CGhost_Attack(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CGhost_Attack::~CGhost_Attack()
{
}

HRESULT CGhost_Attack::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fChase = 0.f;
	m_iCount = 0;
	m_bIsAttack = false;
	
	
	return S_OK;
}

STATE CGhost_Attack::Update_State(const _float& fTimeDelta)
{
	CPlayer& pPlayer =
		*dynamic_cast<CPlayer*>
		(SceneManager()->GetInstance()->Get_ObjectList
		(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());



	if (!m_bIsAttack)
	{
		if (m_pOwner->Get_Animator()->Get_Animation()->Get_Frame() >= 3.8f)
		{
			_float fSpeed = dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Get_Speed();

			_float fAngle = m_iCount * 30.f;

			_vec3 vDir = _vec3(cos(D3DXToRadian(fAngle)), 0.f, sinf(D3DXToRadian(fAngle)));
			_vec3 vOffset = vDir * fSpeed;

			CGameObject* pGameObject = nullptr;
			pGameObject = CGhost_Bullet::Create(m_pGraphicDev, m_pOwner->Get_Transform(), fSpeed, vOffset);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

			++m_iCount;

			if (m_iCount >= 36 * 2)
			{
				m_bIsAttack = true;
				m_iCount = 0.f;
				return STATE::ROMIMG;
			}
		}

	}
		m_bIsAttack = false;
	


	return STATE::ATTACK;
}

void CGhost_Attack::LateUpdate_State()
{
}

void CGhost_Attack::Render_State()
{
}

CGhost_Attack* CGhost_Attack::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CGhost_Attack* pState = new CGhost_Attack(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CGhost_Attack*>(pState);

		MSG_BOX("Create GhostAttack Failed");
		return nullptr;
	}

	return pState;
}

void CGhost_Attack::Free()
{
	__super::Free();
}
