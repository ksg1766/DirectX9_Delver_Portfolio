#include "..\Header\Altar.h"
#include "Export_Function.h"
#include "Orb.h"
#include "Player.h"
#include "FlyingCamera.h"
#include "CameraManager.h"
#include "SoundManager.h"
#include <UIOrbClearLight.h>

CAltar::CAltar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCubeBlock(pGraphicDev)
{
}

CAltar::CAltar(const CAltar& rhs)
	: CCubeBlock(rhs)
{
}

CAltar::~CAltar()
{
	
}

HRESULT CAltar::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::BLOCK;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(1.f, 0.3f, 1.f));
	//m_pTransform->Translate(_vec3(0.f, 4.f, -60.f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
		m_pTransform->LocalScale());

	m_pTransform->Translate(_vec3(0.f, 3.f, -10.f));

	return S_OK;
}

_int CAltar::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	_uint iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CAltar::LateUpdate_Object()
{
	__super::LateUpdate_Object();
}

void CAltar::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(m_iCurrentImage);
	m_pCubeBf->Render_Buffer();
}

void CAltar::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::ITEM) {
		if (static_cast<CItem*>(_pOther->Get_Host())->Get_ItemTag().eItemID == ITEMID::QUEST_ORB)
		{
			CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();
			if (m_bOrbCollision || rPlayer.Get_ItemEquipRight())
				return;

			_vec3 vPos = m_pTransform->m_vInfo[INFO_POS] + _vec3(0.f, 1.f, 0.f);
			_pOther->Get_Host()->m_pTransform->m_vInfo[INFO_POS] = vPos;
			static_cast<COrb*>(_pOther->Get_Host())->Set_Altar(true);
			rPlayer.Set_Orb(true);

			// 플레이어 움직임 및 카메라 고정
			CGameObject* pPlayerObject = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front();
			if (pPlayerObject != nullptr) {
				CPlayer& rPlayer = *static_cast<CPlayer*>(pPlayerObject);
				rPlayer.Set_UseUI(true);
			}
			CGameObject* pCameraGameObject = CCameraManager::GetInstance()->Get_CurrentCam();
			if (pCameraGameObject != nullptr) {
				_vec3 vOrbPos = _pOther->Get_Host()->m_pTransform->m_vInfo[INFO_POS] - pCameraGameObject->m_pTransform->m_vInfo[INFO_POS];
				D3DXVec3Normalize(&pCameraGameObject->m_pTransform->m_vInfo[INFO_LOOK], &vOrbPos);
				D3DXVec3Normalize(&pCameraGameObject->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &pCameraGameObject->m_pTransform->m_vInfo[INFO_LOOK]));
				D3DXVec3Normalize(&pCameraGameObject->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &pCameraGameObject->m_pTransform->m_vInfo[INFO_LOOK], &pCameraGameObject->m_pTransform->m_vInfo[INFO_RIGHT]));
				
				static_cast<CFlyingCamera*>(pCameraGameObject)->Set_MouseFix(true);
			}

			// 배경 사운드 변경
			CSoundManager::GetInstance()->StopAll();
			CSoundManager::GetInstance()->PlayBGM(L"win.mp3", 0.5f);

			// 몇초간의 카메라 쉐이킹
			CFlyingCamera* pCamera = static_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam());
			if (pCamera != nullptr) {
				pCamera->Set_ShakeForce(0.f, 0.05, 4.f, 5.f);
				pCamera->Shake_Camera();
			}

			// UI 빛 이미지 출력 (내부에서 변경 효과 적용 및 생성)
			CGameObject* pGameObjectUI = CUIOrbClearLight::Create(m_pGraphicDev);
			static_cast<CUIOrbClearLight*>(pGameObjectUI)->Set_AltarObject(this);
			static_cast<CUIOrbClearLight*>(pGameObjectUI)->Set_OrbObject(_pOther->Get_Host());
			Engine::UIManager()->Add_BasicGameobject(Engine::UILAYER::UI_DOWN, pGameObjectUI);

			m_bOrbCollision = true;
		}
	}
}

void CAltar::OnCollisionStay(CCollider* _pOther)
{
}

void CAltar::OnCollisionExit(CCollider* _pOther)
{
}

HRESULT CAltar::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pCubeBf = static_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Cube"));
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

CAltar* CAltar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAltar* pInstance = new CAltar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CAltar*>(pInstance);

		MSG_BOX("Create Altar FAILED");
		return nullptr;
	}

	return pInstance;
}

void CAltar::Free()
{
	__super::Free();
}
