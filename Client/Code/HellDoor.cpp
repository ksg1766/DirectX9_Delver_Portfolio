#include "stdafx.h"
#include "SoundManager.h"
#include "Export_Function.h"
#include "Player.h"
#include "HellDoor.h"
#include "DoorCube.h"
#include "DynamicCamera.h"
#include "Scene.h"
#include "Stage.h"
//#include "CameraManager.h"

CHellDoor::CHellDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
	m_vecDoorCube.reserve(40);
}

CHellDoor::CHellDoor(const CHellDoor& rhs)
	: Engine::CGameObject(rhs)
{
}

CHellDoor::~CHellDoor()
{
}

HRESULT CHellDoor::Ready_Object(CLayer* pLayer)
{
	m_eObjectTag = OBJECTTAG::TRIGERBLOCK;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(10.f, 3.f, 1.f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
		m_pTransform->LocalScale());
	m_pTransform->Translate(_vec3(51.f, 75.f,50.f));

	m_bShake = true;
	m_bBgmChange = false;

	return S_OK;
}

_int CHellDoor::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

	_vec3 vDir = rPlayer.m_pTransform->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS];
	_float fDistance = D3DXVec3Length(&vDir);

	_uint iExit = __super::Update_Object(fTimeDelta);

	if (!m_bCreate)
	{
		for (_uint i = 0; i < 4; ++i)
			for (_uint j = 0; j < 10; ++j)
			{
				CGameObject* pDoor = nullptr;
				pDoor = CDoorCube::Create(m_pGraphicDev);
				pDoor->m_pTransform->Translate(_vec3(59 - (j * 2), 72 + (2 * i), 45.f));
				EventManager()->CreateObject(pDoor, LAYERTAG::GAMELOGIC);
				m_vecDoorCube.push_back(dynamic_cast<CDoorCube*>(pDoor));
			}

		m_bCreate = true;
	}

	if (m_bBgmChange)
	{
		CSoundManager::GetInstance()->StopAll();
		CSoundManager::GetInstance()->PlayBGM(L"04_swords_and_skulls.mp3", 0.8f);

		m_bBgmChange = false;
		m_bDoorOpen = false;

	}

	if (m_bDoorOpen)
	{
		CSoundManager::GetInstance()->PlaySoundLoop(L"MyDoor.mp3", CHANNELID::SOUND_DOOR, 0.8f);
	}


	if (m_bTriger)
	{
		CDynamicCamera& rCamera = *dynamic_cast<CDynamicCamera*>(SceneManager()->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::CAMERA).front());

		if (m_bShake)
		{
			rCamera.Set_ShakeForce(0.f, 0.01, 3, 2.f);
			rCamera.Shake_Camera();
		}


		_float fFinalY = m_vecDoorCube[30]->m_pTransform->m_vInfo[INFO_POS].y;


		for (auto& iter = m_vecDoorCube.begin(); iter != m_vecDoorCube.end(); ++iter)
		{
			_uint iCurrentIndex = std::distance(m_vecDoorCube.begin(), iter);
			_float fNormalizeIndex = 0.f;

			if (iCurrentIndex >= 0 && iCurrentIndex < 10)
				fNormalizeIndex = static_cast<_float>(10 - iCurrentIndex) / 10.f;
			else if(iCurrentIndex >= 10 && iCurrentIndex < 20)
				fNormalizeIndex = static_cast<_float>(20 - iCurrentIndex) / 10.f;
			else if (iCurrentIndex >= 20 && iCurrentIndex < 30)
				fNormalizeIndex = static_cast<_float>(30 - iCurrentIndex) / 10.f;


			_float fNewY = (*iter)->m_pTransform->m_vInfo[INFO_POS].y +
				fNormalizeIndex * (fFinalY - m_pTransform->m_vInfo[INFO_POS].y);

			if (iCurrentIndex >= 0 && iCurrentIndex < 10)
				fNewY *= ((_float)10 - iCurrentIndex) + 1;
			else if (iCurrentIndex >= 10 && iCurrentIndex < 20)
				fNewY *= ((_float)20 - iCurrentIndex) + 1;
			else if (iCurrentIndex >= 20 && iCurrentIndex < 30)
				fNewY *= ((_float)30 - iCurrentIndex) + 1;



			if (iCurrentIndex < 30 && fFinalY >= (*iter)->m_pTransform->m_vInfo[INFO_POS].y)
				(*iter)->m_pTransform->m_vInfo[INFO_POS].y += fNewY * fTimeDelta * fTimeDelta * 0.05f;


			if (iCurrentIndex == 17)
			{
				if (fFinalY <= (*iter)->m_pTransform->m_vInfo[INFO_POS].y)
				{
					if (!m_bBgmStop)
					{
						rCamera.Reset_ShakeForce();
						m_bShake = false;
						m_bBgmChange = true;
						m_bBgmStop = true;
					}
				}
			}

			if (iCurrentIndex == 9)
			{
				if (fFinalY <= (*iter)->m_pTransform->m_vInfo[INFO_POS].y)
				{
					m_bTriger = false;
					return iExit;
				}
			}

			if (iCurrentIndex == 1 && !m_bDoorClose)
			{
				m_bDoorClose = true;
				m_bDoorOpen = true;
			}
		
		}

		for (auto iter = m_vecDoorCube.begin(); iter != m_vecDoorCube.end(); ++iter)
			(*iter)->LateUpdate_Object();
	}

	_vec3 vSoundDir = m_vecDoorCube[37]->m_pTransform->m_vInfo[INFO_POS] - m_vecDoorCube[17]->m_pTransform->m_vInfo[INFO_POS];
	_float fSoundDistance = D3DXVec3Length(&vSoundDir);

	_float fVolume = m_fMinVolume - (fSoundDistance / m_fMaxDistance) * (m_fMaxVolume - m_fMinVolume);

	fVolume *= -1.f;

	if (fVolume <= 0.01f)
	{
		m_fSound = 0.f;
		dynamic_cast<CStage*>(CSceneManager::GetInstance()->Get_Scene())->Set_Sound(false);
	}
	else
		m_fSound = fVolume;

	if(m_fSound != 0)
		dynamic_cast<CStage*>(CSceneManager::GetInstance()->Get_Scene())->Set_Sound(m_fSound);

	return iExit;
}

void CHellDoor::LateUpdate_Object()
{
	__super::LateUpdate_Object();


}

void CHellDoor::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	//m_pTexture->Render_Texture(39);
	//m_pCubeBf->Render_Buffer();


#if _DEBUG
	m_pCollider->Render_Collider();
#endif // _DEBUG

}

void CHellDoor::OnCollisionEnter(CCollider* _pOther)
{
	__super::OnCollisionEnter(_pOther);

	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		m_bTriger = true;
	}
}

void CHellDoor::OnCollisionStay(CCollider* _pOther)
{
	__super::OnCollisionStay(_pOther);
}

void CHellDoor::OnCollisionExit(CCollider* _pOther)
{
	__super::OnCollisionExit(_pOther);
}

HRESULT CHellDoor::Add_Component()
{
	CComponent* pComponent = nullptr;
	pComponent = m_pCubeBf = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Cube"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = static_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);


	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CHellDoor* CHellDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev, CLayer* pLayer)
{
	CHellDoor* pInstance = new CHellDoor(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pLayer)))
	{
		Safe_Release<CHellDoor*>(pInstance);

		MSG_BOX("Create HellDoor FAILED");
		return nullptr;
	}

	return pInstance;
}

void CHellDoor::Free()
{
	__super::Free();
	
	//for_each(m_vecDoorCube.begin(), m_vecDoorCube.end(), CDeleteObj());
	//m_vecDoorCube.clear();

}
