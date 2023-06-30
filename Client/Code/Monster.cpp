#include "stdafx.h"
#include "..\Header\Monster.h"

#include "Export_Function.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}
CMonster::CMonster(const CMonster& rhs)
	: Engine::CGameObject(rhs)
{

}
CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CMonster::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	/*CTransform*	pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(LAYERTAG::ENVIRONMENT, OBJECTTAG::PLAYER, COMPONENTTAG::TRANSFORM, ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransform, -1);
	
	_vec3		vPlayerPos;
	pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);

	m_pTransformCom->Chase_Target(&vPlayerPos, fTimeDelta, m_fSpeed);*/

	return iExit;
}

void CMonster::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CMonster::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMonster::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CTriCol*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_TriCol"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	return S_OK;
}

void CMonster::Free()
{
	__super::Free();
}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster*	pInstance = new CMonster(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Monster Create Failed");
		return nullptr;
	}

	return pInstance;
}
