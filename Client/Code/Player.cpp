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
	m_eObjectTag = OBJECTTAG::PLAYER;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3( 1.f, 2.f, 1.f ));
	//m_fSpeed = 10.f;

	return S_OK;
}

Engine::_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);
	Key_Input(fTimeDelta);

	return iExit;
}

void CPlayer::LateUpdate_Object(void)
{

	__super::LateUpdate_Object();
}

void CPlayer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pBuffer->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayer::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CPlayerBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_PlayerBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);
	
	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);
	
	for(int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->InitProperty(this);

	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	//m_vDir = m_pTransform->m_vInfo[INFO_LOOK];

	if (GetAsyncKeyState('W'))
	{
		//D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransform->Translate(m_fSpeed * fTimeDelta * m_pTransform->m_vInfo[INFO_LOOK]);
		//m_pTransformCom->Move_Pos(&m_vDir, fTimeDelta, m_fSpeed);
	}

	if (GetAsyncKeyState('S'))
	{
		//D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransform->Translate(m_fSpeed * fTimeDelta * -m_pTransform->m_vInfo[INFO_LOOK]);
	}

	if (GetAsyncKeyState('A'))
		m_pTransform->Rotate(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));

	if (GetAsyncKeyState('D'))
		m_pTransform->Rotate(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
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
