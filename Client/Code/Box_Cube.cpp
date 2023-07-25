#include "stdafx.h"
#include "Box_Cube.h"
#include "Export_Function.h"
#include "Player.h"
#include "EffectBrokenbox.h"
#include "EffectSquare.h"
#include "Itemgroup.h"
#include "SoundManager.h"

CBox_Cube::CBox_Cube(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev)
{
}

CBox_Cube::CBox_Cube(const CBox_Cube& rhs)
    : CGameObject(rhs)
{
}

CBox_Cube::~CBox_Cube()
{
	Free();
}

HRESULT CBox_Cube::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::FRAGILE;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fHitCool = 0.f;
	m_fShakeDelay = 0.f;
	m_bHit = false;
	m_bShake = false;
	m_iHP = 2;
	m_pTransform->Scale(_vec3 (0.5f, 0.5f, 0.5f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale() * 1.2f);


	m_vOriginUp = _vec3(0.f, 0.5f, 0.f);
	m_vOriginLook = _vec3(0.f, 0.f, 0.5f);
	m_vOriginRight = _vec3(0.5f, 0.f, 0.f);
	m_fLerpTime = 0.f;

	return S_OK;
}

_int CBox_Cube::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);


	if (m_bHit)
	{
		m_fHitCool += fTimeDelta;
		Shake_Box(fTimeDelta);

		if (m_fHitCool > 1.f)
		{
			m_fHitCool = 0;
			m_bHit = false;
		}

		// Shake
	}

	//m_fHitCool += fTimeDelta;
	//if (1.5f < m_fHitCool)
	//	m_bHit = false;
	//if (0 >= m_iHP)
	//	Drop_RandomItem();
	return iExit;
}

void CBox_Cube::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
}

void CBox_Cube::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	
	m_pTexture->Render_Texture(m_iTextureNumber);
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

void CBox_Cube::Drop_RandomItem()
{
	CreateConsum();

	// 이펙트 생성
	CGameObject* pGameObject = CEffectBrokenbox::Create(m_pGraphicDev);
	dynamic_cast<CEffectBrokenbox*>(pGameObject)->m_pTransform->
		Translate(_vec3(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y + 0.5f, m_pTransform->m_vInfo[INFO_POS].z));
	dynamic_cast<CEffectBrokenbox*>(pGameObject)->Set_EffectType(0);
	Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

	Engine::EventManager()->DeleteObject(this);
}

void CBox_Cube::CreateConsum()
{
	CItem* pItem = nullptr;

	_int iRamdom = rand() % 9;
	const _int iApple = 0, iBread = 1, iCheese = 2, iMeat = 3,
		iRoastMeat = 4, iBeer = 5, iHolyWater = 6, iHpPotion = 7, iRandomPotion = 8;

	switch (iRamdom)
	{
	case iApple:
		pItem = CApple::Create(m_pGraphicDev, true);
		break;
	case iBread:
		pItem = CBread::Create(m_pGraphicDev, true);
		break;
	case iCheese:
		pItem = CCheese::Create(m_pGraphicDev, true);
		break;
	case iMeat:
		pItem = CMeat::Create(m_pGraphicDev, true);
		break;
	case iRoastMeat:
		pItem = CRoastmeat::Create(m_pGraphicDev, true);
		break;
	case iBeer:
		pItem = CBeer::Create(m_pGraphicDev, true);
		break;
	case iHolyWater:
		pItem = CHolyWater::Create(m_pGraphicDev, true);
		break;
	case iHpPotion:
		pItem = CHpPotion::Create(m_pGraphicDev, true);
		break;
	case iRandomPotion:
		pItem = CRandomPotion::Create(m_pGraphicDev, true);
		break;
	}



	pItem->m_pTransform->Copy_RUL_AddPos(m_pTransform->m_vInfo);
	EventManager()->CreateObject(pItem, LAYERTAG::GAMELOGIC);
}

void CBox_Cube::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	
	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYERBULLET && !IsDead())
	{
			m_iHP -= 1.f;
			if (m_iHP <= 0)
			{
				Drop_RandomItem();
				EventManager()->DeleteObject(this);
			}
			m_bHit = true;

			CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_EFFECT);
			CSoundManager::GetInstance()->PlaySound(L"break_wood_01.mp3", CHANNELID::SOUND_EFFECT, 1.f);
	}
}

void CBox_Cube::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	//if (m_bHit) { return; }

	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

	if ((_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::ITEM || _pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYERBULLET) && !IsDead())
	{
		if ((dynamic_cast<CItem*>(_pOther->Get_Host())->
			Get_ItemTag().eItemType == ITEMTYPE::ITEMTYPE_WEAPONITEM &&
			!m_bHit && rPlayer.Get_Attack()) || dynamic_cast<CItem*>(_pOther->Get_Host())->Get_ObjectTag() == OBJECTTAG::PLAYERBULLET)
		{
			m_iHP -= 1.f;

			if (m_iHP <= 0)
			{
				Drop_RandomItem();
				EventManager()->DeleteObject(this);
			}
		
			CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_EFFECT);
			CSoundManager::GetInstance()->PlaySound(L"break_wood_01.mp3", CHANNELID::SOUND_EFFECT, 1.f);

			m_bHit = true;
		}
	}
}

void CBox_Cube::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

HRESULT CBox_Cube::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_WoodBox"));
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

void CBox_Cube::Shake_Box(const _float& fTimeDelta)
{
	m_fShakeDelay += fTimeDelta;

#pragma region y축으로 x,z 회전
	if (m_fShakeDelay <= 0.1)
	{
		_matrix matRot;

		_vec3 vUp = *D3DXVec3Cross(&vUp, &m_pTransform->m_vInfo[INFO_LOOK], &m_pTransform->m_vInfo[INFO_RIGHT]);
		_vec3 vRight = *D3DXVec3Cross(&vRight, &m_pTransform->m_vInfo[INFO_UP], &m_pTransform->m_vInfo[INFO_LOOK]);
		_vec3 vLook = m_pTransform->m_vInfo[INFO_LOOK];

		D3DXMatrixRotationAxis(&matRot, &vUp, 0.5 * fTimeDelta);
		D3DXVec3TransformCoord(&m_pTransform->m_vInfo[INFO_RIGHT], &m_pTransform->m_vInfo[INFO_RIGHT], &matRot);
		D3DXVec3TransformCoord(&m_pTransform->m_vInfo[INFO_LOOK], &m_pTransform->m_vInfo[INFO_LOOK], &matRot);
	}
	else if (m_fShakeDelay <= 0.3f)
	{
		_matrix matRot;


		_vec3 vUp = *D3DXVec3Cross(&vUp, &m_pTransform->m_vInfo[INFO_LOOK], &m_pTransform->m_vInfo[INFO_RIGHT]);
		_vec3 vRight = *D3DXVec3Cross(&vRight, &m_pTransform->m_vInfo[INFO_UP], &m_pTransform->m_vInfo[INFO_LOOK]);
		_vec3 vLook = m_pTransform->m_vInfo[INFO_LOOK];

		D3DXMatrixRotationAxis(&matRot, &vUp, -(0.5 * fTimeDelta));
		D3DXVec3TransformCoord(&m_pTransform->m_vInfo[INFO_RIGHT], &m_pTransform->m_vInfo[INFO_RIGHT], &matRot);
		D3DXVec3TransformCoord(&m_pTransform->m_vInfo[INFO_LOOK], &m_pTransform->m_vInfo[INFO_LOOK], &matRot);
	}
	else
	{
		//_vec3 vDirUp = m_vOriginUp - m_pTransform->m_vInfo[INFO_UP];
		_vec3 vDirLook = m_vOriginLook - m_pTransform->m_vInfo[INFO_LOOK];
		_vec3 vDirRight = m_vOriginRight - m_pTransform->m_vInfo[INFO_RIGHT];

		m_pTransform->m_vInfo[INFO_RIGHT] += vDirRight * 0.5 * fTimeDelta;
		m_pTransform->m_vInfo[INFO_LOOK] += vDirLook * 0.5 * fTimeDelta;
	}
#pragma region y축으로 x,z 회전

}	

CBox_Cube* CBox_Cube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBox_Cube* pInstance = new CBox_Cube(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CubeBox Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBox_Cube::Free()
{
	__super::Free();
}
