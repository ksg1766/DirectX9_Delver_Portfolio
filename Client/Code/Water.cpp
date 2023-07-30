#include "stdafx.h"
#include "SoundManager.h"
#include "Water.h"
#include "Player.h"
#include "Export_Function.h"

CWater::CWater(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCubeBlock(pGraphicDev)
{
}

CWater::CWater(const CWater& rhs)
	: CCubeBlock(rhs)
{
}

CWater::~CWater()
{
}

HRESULT CWater::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::BLOCK;
	m_eBlockTag = BLOCKTAG::WATER_BLOCK;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_iTextureNumber = 0;

	//_vec3 vWaterCollider = _vec3(m_pTransform->LocalScale().x, 0.5f * m_pTransform->LocalScale().y, m_pTransform->LocalScale().z)
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS] + _vec3(0.f, -1.f, 0.f), &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

	//m_pTransform->Translate(_vec3(0.f, 2.f, 0.f));

	return S_OK;
}

_int CWater::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	(m_iTextureNumber < 319) ? (++m_iTextureNumber) : (m_iTextureNumber = 0);

	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CWater::LateUpdate_Object(void)
{
	//__super::LateUpdate_Object();
	m_pCollider->SetCenterPos(m_pTransform->m_vInfo[INFO_POS] - _vec3(0.f, 0.3f, 0.f));
}

void CWater::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(m_iTextureNumber / 10);

	m_pBuffer->Render_Buffer();
}

void CWater::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		if (!dynamic_cast<CPlayer*>(_pOther->Get_Host())->DropWater())
		{
			CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_WATER);
			CSoundManager::GetInstance()->PlaySound(L"drop_water_03.mp3", CHANNELID::SOUND_WATER, 1.f);
			
			dynamic_cast<CPlayer*>(_pOther->Get_Host())->Set_DropWather(true);
		}
		else
		{

		}
			//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_WATER);

	}
}

void CWater::OnCollisionStay(CCollider* _pOther)
{
}

void CWater::OnCollisionExit(CCollider* _pOther)
{
}

HRESULT CWater::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Water"));
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

CWater* CWater::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWater* pInstance = new CWater(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Water Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CWater::Free()
{
	__super::Free();
}
