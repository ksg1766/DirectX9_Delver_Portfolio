#include "stdafx.h"
#include "..\Header\Player.h"

#include "Export_Function.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}
CPlayer::CPlayer(const CPlayer& rhs)
	: Engine::CGameObject(rhs)
{

}
CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eObjectTag = OBJECTTAG::PLAYER;

	//m_pTransformCom->m_vScale = { 2.f, 1.f, 1.f };
	m_fSpeed = 10.f;

	return S_OK;
}

Engine::_int CPlayer::Update_Object(const _float& fTimeDelta)
{

	_int iExit = __super::Update_Object(fTimeDelta);

	Key_Input(fTimeDelta);

	return iExit;
}

void CPlayer::LateUpdate_Object(void)
{

	__super::LateUpdate_Object();
}

void CPlayer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayer::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

/*	pComponent = m_pBufferCom = dynamic_cast<CTriCol*>(Engine::Clone_Proto(L"Proto_TriCol"));*/
	pComponent = m_pBufferCom = dynamic_cast<CRcCol*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcCol"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	m_vDir = m_pTransformCom->m_vInfo[INFO_UP];

	if (GetAsyncKeyState(VK_UP))
	{
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransformCom->Translate(m_vDir);
		//m_pTransformCom->Move_Pos(&m_vDir, fTimeDelta, m_fSpeed);
	}

	if (GetAsyncKeyState(VK_DOWN))
	{
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransformCom->Translate(-m_vDir);
	}

	if (GetAsyncKeyState('Q'))
		m_pTransformCom->Rotate(ROT_X, D3DXToRadian(180.f * fTimeDelta));

	if (GetAsyncKeyState('A'))
		m_pTransformCom->Rotate(ROT_X, D3DXToRadian(-180.f * fTimeDelta));
	
	if (GetAsyncKeyState('W'))
		m_pTransformCom->Rotate(ROT_Y, D3DXToRadian(180.f * fTimeDelta));

	if (GetAsyncKeyState('S'))
		m_pTransformCom->Rotate(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));

	if (GetAsyncKeyState('E'))
		m_pTransformCom->Rotate(ROT_Z, D3DXToRadian(180.f * fTimeDelta));

	if (GetAsyncKeyState('D'))
		m_pTransformCom->Rotate(ROT_Z, D3DXToRadian(-180.f * fTimeDelta));

}

void CPlayer::Free()
{
	__super::Free();
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pInstance;
}
