#include "stdafx.h"
#include "WaterFall.h"
#include "Player.h"
#include "Export_Function.h"
#include "SoundManager.h"
#include "FootRay.h"

CWaterFall::CWaterFall(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCubeBlock(pGraphicDev)
{
}

CWaterFall::CWaterFall(const CWaterFall& rhs)
	: CCubeBlock(rhs)
{
}

CWaterFall::~CWaterFall()
{
}

HRESULT CWaterFall::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::BLOCK;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_byTextureNumber = 0;

	//_vec3 vWaterCollider = _vec3(m_pTransform->LocalScale().x, 0.5f * m_pTransform->LocalScale().y, m_pTransform->LocalScale().z)
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS] + _vec3(0.f, -1.f, 0.f), &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

	//m_pTransform->Translate(_vec3(0.f, 2.f, 0.f));

	return S_OK;
}

_int CWaterFall::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	(m_byTextureNumber < 31) ? (++m_byTextureNumber) : (m_byTextureNumber = 0);

	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CWaterFall::LateUpdate_Object(void)
{
	//__super::LateUpdate_Object();
	m_pCollider->SetCenterPos(m_pTransform->m_vInfo[INFO_POS] - _vec3(0.f, 0.1f, 0.f));
}

void CWaterFall::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(m_byTextureNumber);

	m_pBuffer->Render_Buffer();
}

void CWaterFall::OnCollisionEnter(CCollider* _pOther)
{

	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

		if (!rPlayer.DropWater())
			rPlayer.Set_DropWather(true);
		else
		{

		}
	}
}

void CWaterFall::OnCollisionStay(CCollider* _pOther)
{
}

void CWaterFall::OnCollisionExit(CCollider* _pOther)
{
}

HRESULT CWaterFall::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_WaterFall"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CWaterFall* CWaterFall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWaterFall* pInstance = new CWaterFall(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("WaterFall Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CWaterFall::Free()
{
	__super::Free();
}
