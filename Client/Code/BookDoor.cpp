#include "stdafx.h"
#include "SoundManager.h"
#include "Export_Function.h"
#include "Player.h"
#include "BookDoor.h"
#include "BookCube.h"

#include "CameraManager.h"
#include "FlyingCamera.h"
#include "Scene.h"
#include "Stage.h"
//#include "CameraManager.h"

CBookDoor::CBookDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
	m_vecFrontDoorCube.reserve(16);
	m_vecBackDoorCube.reserve(16);
}

CBookDoor::CBookDoor(const CBookDoor& rhs)
	: Engine::CGameObject(rhs)
{
}

CBookDoor::~CBookDoor()
{
}

HRESULT CBookDoor::Ready_Object(CLayer* pLayer)
{
	m_eObjectTag = OBJECTTAG::TRIGERBLOCK;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(1.f, 1.f, 5.f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
		m_pTransform->LocalScale());
	m_pTransform->Translate(_vec3(-69.f, 12.f, -9.f));

	m_bShake = true;
	m_bBgmChange = false;

	return S_OK;
}

_int CBookDoor::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

	_vec3 vDir = rPlayer.m_pTransform->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS];
	_float fDistance = D3DXVec3Length(&vDir);

	_uint iExit = __super::Update_Object(fTimeDelta);

	if (!m_bCreate)
	{
		for (_uint i = 0; i < 4; ++i)
			for (_uint j = 0; j < 4; ++j)
			{
				CGameObject* pFrontDoor = nullptr;
				pFrontDoor = CBookCube::Create(m_pGraphicDev);
				pFrontDoor->m_pTransform->Translate(_vec3(-67.f, 12 + (2 * i), -13.f + (j * 2)));
				EventManager()->CreateObject(pFrontDoor, LAYERTAG::GAMELOGIC);
				m_vecFrontDoorCube.push_back(dynamic_cast<CBookCube*>(pFrontDoor));

				CGameObject* pBackDoor = nullptr;
				pBackDoor = CBookCube::Create(m_pGraphicDev);
				pBackDoor->m_pTransform->Translate(_vec3(-65.f, 12 + (2 * i), -13.f + (j * 2)));
				EventManager()->CreateObject(pBackDoor, LAYERTAG::GAMELOGIC);
				m_vecBackDoorCube.push_back(dynamic_cast<CBookCube*>(pBackDoor));
			}

		m_bCreate = true;
		m_fFinalRight = m_vecFrontDoorCube[0]->m_pTransform->m_vInfo[INFO_POS].z - 2.f;
		m_fFinalLeft = m_vecFrontDoorCube[3]->m_pTransform->m_vInfo[INFO_POS].z + 2.f;

		m_fFinalUp = m_vecBackDoorCube[15]->m_pTransform->m_vInfo[INFO_POS].y + 2.f;
		m_fFinalDown = m_vecBackDoorCube[0]->m_pTransform->m_vInfo[INFO_POS].y - 2.f;

	}

	if (m_bBgmChange)
	{
		CSoundManager::GetInstance()->StopAll();
		CSoundManager::GetInstance()->PlayBGM(L"DK-5", 0.8f);

		m_bBgmChange = false;
		m_bDoorOpen = false;

	}

	//if (m_bDoorOpen)
	//{
	//	CSoundManager::GetInstance()->PlaySoundLoop(L"MyDoor.mp3", CHANNELID::SOUND_DOOR, 0.8f);
	//}

	if (m_bTriger)
	{

	//	if (m_bShake)
	//	{
	//		pCamera->Set_ShakeForce(0.f, 0.01f, 3, 2.f);
	//		pCamera->Shake_Camera();
	//	}

		for (auto& iter = m_vecFrontDoorCube.begin(); iter != m_vecFrontDoorCube.end(); ++iter)
		{
			_uint iCurrentIndex = std::distance(m_vecFrontDoorCube.begin(), iter);


			if (iCurrentIndex == 0 || iCurrentIndex == 4 || iCurrentIndex == 8 || iCurrentIndex == 12
				|| iCurrentIndex == 1 || iCurrentIndex == 5 || iCurrentIndex == 9 || iCurrentIndex == 13)
			{
				_float fNewRightZ = (*iter)->m_pTransform->m_vInfo[INFO_POS].z + 1 *
					(m_fFinalRight - (*iter)->m_pTransform->m_vInfo[INFO_POS].z);

				if (m_fFinalRight <= (*iter)->m_pTransform->m_vInfo[INFO_POS].z)
					(*iter)->m_pTransform->m_vInfo[INFO_POS].z += fNewRightZ * fTimeDelta * 0.05f;


				if (iCurrentIndex == 13)
				{
					if (m_fFinalRight <= (*iter)->m_pTransform->m_vInfo[INFO_POS].z)
						m_bFrontOpen = true;
				}
			}

			if (iCurrentIndex == 2 || iCurrentIndex == 6 || iCurrentIndex == 10 || iCurrentIndex == 14
				|| iCurrentIndex == 3 || iCurrentIndex == 7 || iCurrentIndex == 11 || iCurrentIndex == 15)
			{
				_float fNewLeftZ = (*iter)->m_pTransform->m_vInfo[INFO_POS].z - 1 *
					(m_fFinalLeft - (*iter)->m_pTransform->m_vInfo[INFO_POS].z);

				if (m_fFinalLeft >= (*iter)->m_pTransform->m_vInfo[INFO_POS].z)
					(*iter)->m_pTransform->m_vInfo[INFO_POS].z -= fNewLeftZ * fTimeDelta * 0.05f;
			}


			if (iCurrentIndex == 2)
			{
				if (m_fFinalLeft <= (*iter)->m_pTransform->m_vInfo[INFO_POS].z)
					m_bTriger = false;
			}
		}


		if (m_bFrontOpen)
		{
			for (auto& iter = m_vecBackDoorCube.begin(); iter != m_vecBackDoorCube.end(); ++iter)
			{
				_uint iCurrentIndex = std::distance(m_vecBackDoorCube.begin(), iter);

				if (iCurrentIndex == 0 || iCurrentIndex == 1 || iCurrentIndex == 2 || iCurrentIndex == 3
					|| iCurrentIndex == 4 || iCurrentIndex == 5 || iCurrentIndex == 6 || iCurrentIndex == 7)
				{
					// TODO
					// : Y축 하강

					_float fNewDownPos = (*iter)->m_pTransform->m_vInfo[INFO_POS].y - 1 *
						(m_fFinalDown - (*iter)->m_pTransform->m_vInfo[INFO_POS].y);

					if (m_fFinalDown <= (*iter)->m_pTransform->m_vInfo[INFO_POS].y)
						(*iter)->m_pTransform->m_vInfo[INFO_POS].y -= fNewDownPos * fTimeDelta * 0.05f;

				}

				if (iCurrentIndex == 8 || iCurrentIndex == 9 || iCurrentIndex == 10 || iCurrentIndex == 11
					|| iCurrentIndex == 12 || iCurrentIndex == 13 || iCurrentIndex == 14 || iCurrentIndex == 15)
				{
					// TODO
					// : Y축 상승 

					_float fNewUpPos = (*iter)->m_pTransform->m_vInfo[INFO_POS].y + 1 *
						(m_fFinalUp - (*iter)->m_pTransform->m_vInfo[INFO_POS].y);

					if (m_fFinalUp >= (*iter)->m_pTransform->m_vInfo[INFO_POS].y)
						(*iter)->m_pTransform->m_vInfo[INFO_POS].y += fNewUpPos * fTimeDelta * 0.05f;
				}
			}
		}

		for (auto iter = m_vecFrontDoorCube.begin(); iter != m_vecFrontDoorCube.end(); ++iter)
			(*iter)->LateUpdate_Object();

		for (auto iter = m_vecBackDoorCube.begin(); iter != m_vecBackDoorCube.end(); ++iter)
			(*iter)->LateUpdate_Object();
	}
	
	return iExit;
}

void CBookDoor::LateUpdate_Object()
{
	__super::LateUpdate_Object();


}

void CBookDoor::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	//m_pTexture->Render_Texture(39);
	//m_pCubeBf->Render_Buffer();


#if _DEBUG
	m_pCollider->Render_Collider();
#endif // _DEBUG

}

void CBookDoor::OnCollisionEnter(CCollider* _pOther)
{
	__super::OnCollisionEnter(_pOther);

	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		m_bTriger = true;
	}
}

void CBookDoor::OnCollisionStay(CCollider* _pOther)
{
	__super::OnCollisionStay(_pOther);
}

void CBookDoor::OnCollisionExit(CCollider* _pOther)
{
	__super::OnCollisionExit(_pOther);
}

HRESULT CBookDoor::Add_Component()
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

CBookDoor* CBookDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev, CLayer* pLayer)
{
	CBookDoor* pInstance = new CBookDoor(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pLayer)))
	{
		Safe_Release<CBookDoor*>(pInstance);

		MSG_BOX("Create HellDoor FAILED");
		return nullptr;
	}

	return pInstance;
}

void CBookDoor::Free()
{
	__super::Free();

	//for_each(m_vecDoorCube.begin(), m_vecDoorCube.end(), CDeleteObj());
	//m_vecDoorCube.clear();
}
