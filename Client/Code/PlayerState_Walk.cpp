#include "..\Header\PlayerState_Walk.h"
#include "Export_Function.h"
#include "DynamicCamera.h"

CPlayerState_Walk::CPlayerState_Walk()
{
}

CPlayerState_Walk::CPlayerState_Walk(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CState(pGraphicDev)
{

}

CPlayerState_Walk::~CPlayerState_Walk()
{
}

HRESULT CPlayerState_Walk::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;

	return S_OK;
}

STATE CPlayerState_Walk::Update_State(const _float& fTimeDelta)
{
	STATE eState = Key_Input(fTimeDelta);



	return eState;
}

void CPlayerState_Walk::LateUpdate_State()
{
}

void CPlayerState_Walk::Render_State()
{
	cout << "Walk" << endl;
}

STATE CPlayerState_Walk::Key_Input(const _float& fTimeDelta)
{
	_vec3 vLook = m_pOwner->Get_Transform()->m_vInfo[INFO_LOOK];
	_vec3 vRight = m_pOwner->Get_Transform()->m_vInfo[INFO_RIGHT];

	STATE	_eState = STATE::IDLE;

	if (Engine::InputDev()->GetInstance()->Key_Pressing(DIK_W))
	{
		m_pOwner->Get_Transform()->Translate(10.f * fTimeDelta * vLook);
		_eState = STATE::ROMIMG;
	}
	if (Engine::InputDev()->GetInstance()->Key_Pressing(DIK_S))
	{
		m_pOwner->Get_Transform()->Translate(10.f * fTimeDelta * -vLook);
		_eState = STATE::ROMIMG;
	}
	if (Engine::InputDev()->GetInstance()->Key_Pressing(DIK_A))
	{
		m_pOwner->Get_Transform()->Translate(10.f * fTimeDelta * -vRight);
		_eState = STATE::ROMIMG;
	}
	if (Engine::InputDev()->GetInstance()->Key_Pressing(DIK_D))
	{
		m_pOwner->Get_Transform()->Translate(10.f * fTimeDelta * vRight);
		_eState = STATE::ROMIMG;
	}
	
	CGameObject* pGameObject = SceneManager()->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::CAMERA).front();

	_bool bCameraOn = static_cast<CDynamicCamera*>(pGameObject)->Get_MouseFix();

	_long dwMouseMove = 0;

	if (0 != (dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_X)) && !bCameraOn)
		m_pOwner->Get_Transform()->Rotate(ROT_Y, D3DXToRadian(dwMouseMove) * fTimeDelta * 3.f);

	if (0 != (dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_Y)) && !bCameraOn)
		m_pOwner->Get_Transform()->Rotate(ROT_X, D3DXToRadian(dwMouseMove) * fTimeDelta * 3.f);


	return _eState;
}

CPlayerState_Walk* CPlayerState_Walk::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CPlayerState_Walk* pState = new CPlayerState_Walk(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CPlayerState_Walk*>(pState);

		MSG_BOX("Create WalkState Failed");
		return nullptr;
	}

	return pState;
}

void CPlayerState_Walk::Free()
{
	__super::Free();
}
