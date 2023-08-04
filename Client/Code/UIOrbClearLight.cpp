#include "stdafx.h"
#include "..\Header\UIOrbClearLight.h"
#include "SkyBoxVillage.h"
#include "TempEffect.h"
#include "EffectFallingleaves.h"
#include "EffectFirefly.h"
#include "EffectTwinkle.h"
#include <Orb.h>
#include <OrbBlockLight.h>
#include <UIOrbClearLight.h>
#include <CubeBlock.h>
#include <Altar.h>
#include "FlyingCamera.h"
#include "CameraManager.h"
#include "GameManager.h"
#include "Player.h"
#include <EffectFirework.h>
#include "WhiteOutIn.h"

CUIOrbClearLight::CUIOrbClearLight(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}


CUIOrbClearLight::~CUIOrbClearLight()
{

}

HRESULT CUIOrbClearLight::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // 초기화

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = WINCX / 2;
	m_pTransform->m_vInfo[INFO_POS].y = WINCY / 2;

	m_pTransform->m_vLocalScale.x = 1000.f;
	m_pTransform->m_vLocalScale.y = 500.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

	m_fFrame = 0.f;
	m_fFrist = 0.f;
	m_fFinal = 14.f;
	m_fFrameSpeed = 3.5f;

	return S_OK;
}

_int CUIOrbClearLight::Update_Object(const _float & fTimeDelta)
{
	if (m_IsDead)
		return 0;

	m_fLifeTime += 1.f * fTimeDelta * m_fFrameSpeed;

	if (m_bAnimation){
		m_fFrame += 1.f * fTimeDelta * m_fFrameSpeed;
		if (m_fFrame > m_fFinal)
		{
			m_fFrame = m_fFinal;
			m_bAnimation = false;
		}
	}

	if (!m_bPadeOutIn && m_fLifeTime > 12.f)
	{
		// 흰색 페이드 아웃 인
		CGameObject* pGameObject = CWhiteOutIn::Create(m_pGraphicDev);
		static_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_BASIC, 7);
		Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_BLACK, Engine::UILAYER::UI_DOWN, pGameObject);
	
		m_bPadeOutIn = true;
	}
	if (!m_bClear && m_fLifeTime > 14.f)
	{
		m_bClear = true;

		// 오브에 반짝이는 이펙트 생성
		CGameObject* pGameObject = nullptr;
		for (_uint i = 0; i < 3; ++i)
		{
			pGameObject = CEffectTwinkle::Create(m_pGraphicDev);
			pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_pAltar->m_pTransform->m_vInfo[INFO_POS].x, m_pAltar->m_pTransform->m_vInfo[INFO_POS].y + 1.f, m_pAltar->m_pTransform->m_vInfo[INFO_POS].z);
			static_cast<CEffectTwinkle*>(pGameObject)->Set_Distance(.1f);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}

		// 오브 색상 밝게 변경
		if(m_pOrb != nullptr)
			static_cast<COrb*>(m_pOrb)->Set_CurrentImage(43);
		// 오브 제단 색상 밝게 변경
		if(m_pAltar != nullptr)
			static_cast<CAltar*>(m_pAltar)->Set_ImageMode(52);

		// 제단 주변 땅 색상 밝게 변경 : 원 이미지 출력
		CGameObject* pAltarLight = COrbBlockLight::Create(m_pGraphicDev);
		pAltarLight->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_pAltar->m_pTransform->m_vInfo[INFO_POS].x, m_pAltar->m_pTransform->m_vInfo[INFO_POS].y + 0.001f, m_pAltar->m_pTransform->m_vInfo[INFO_POS].z);
		Engine::EventManager()->CreateObject(pAltarLight, LAYERTAG::GAMELOGIC);

		// 나무 블럭 이미지 교체
		vector<CGameObject*> pEffectBlock = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BLOCK);
		for (auto& iter : pEffectBlock)
		{
			if (static_cast<CCubeBlock*>(iter)->Get_TextureNumber() == 53)
			{
				static_cast<CCubeBlock*>(iter)->Set_TextureNumber(51);
			}
			else if (iter->m_pTransform->m_vInfo[INFO_POS].y > 50.f)
			{
				static_cast<CCubeBlock*>(iter)->Set_TextureNumber(15);
			}
		}

		// 스카이 박스 색상 이미지 교체
		CGameObject* pSkyObject = SceneManager()->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::SKYBOX).front();
		if (pSkyObject != nullptr)
			static_cast<CSkyBoxVillage*>(pSkyObject)->Set_SkyMode(1);

		// 안개 색상 교체
		Engine::Renderer()->Set_FogColor(100, 255, 170, 150);
		Engine::Renderer()->Set_FogDistance(1.f, 200.0f);

		vector<CGameObject*> pEffectList = SceneManager()->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::EFFECT);
		for (auto& iter : pEffectList)
		{
			// 나뭇잎 이펙트 -> 꽃 이미지로 교체
			if (static_cast<CTempEffect*>(iter)->Get_EffectTag() == EFFECTTAG::EFFECT_LEAVES)
			{
				static_cast<CEffectFallingleaves*>(iter)->Set_ChangeMode(true);
			}
			// 반딧불이 이펙트 -> 나비 이미지로 교체
			else if (static_cast<CTempEffect*>(iter)->Get_EffectTag() == EFFECTTAG::EFFECT_FIREFLY)
			{
				static_cast<CEffectFirefly*>(iter)->Set_ChangeMode(true);
			}
		}

		// 플레이어 움직임 및 카메라 고정 해제
		CGameObject* pPlayerObject = SceneManager()->Get_Scene()->Get_MainPlayer();
		if (pPlayerObject != nullptr)
		{
			CPlayer& rPlayer = *static_cast<CPlayer*>(pPlayerObject);
			rPlayer.Set_UseUI(false);
		}
		CGameObject* pCameraGameObject = CCameraManager::GetInstance()->Get_CurrentCam();
		if (pCameraGameObject != nullptr && m_pOrb != nullptr)
		{
			_vec3 vOrbPos = m_pOrb->m_pTransform->m_vInfo[INFO_POS] - pCameraGameObject->m_pTransform->m_vInfo[INFO_POS];
			D3DXVec3Normalize(&pCameraGameObject->m_pTransform->m_vInfo[INFO_LOOK], &vOrbPos);
			D3DXVec3Normalize(&pCameraGameObject->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &pCameraGameObject->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&pCameraGameObject->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &pCameraGameObject->m_pTransform->m_vInfo[INFO_LOOK], &pCameraGameObject->m_pTransform->m_vInfo[INFO_RIGHT]));

			static_cast<CFlyingCamera*>(pCameraGameObject)->Set_MouseFix(false);
		}

		// 몇초마다 하늘에 폭죽 이펙트 발사
		CGameObject* pFirework = nullptr;
		for (_uint i = 0; i < 7; ++i) {
			pFirework = CEffectFirework::Create(m_pGraphicDev);
			pFirework->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_pAltar->m_pTransform->m_vInfo[INFO_POS].x, m_pAltar->m_pTransform->m_vInfo[INFO_POS].y + 0.0001f, m_pAltar->m_pTransform->m_vInfo[INFO_POS].z);
			Engine::EventManager()->CreateObject(pFirework, LAYERTAG::GAMELOGIC);
		}

		m_IsDead = true;
	}

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUIOrbClearLight::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	if (m_pOrb != nullptr) {
		_vec3 vOrbpos = m_pOrb->m_pTransform->m_vInfo[INFO_POS];

		// 월드 -> 뷰 스페이스
		_matrix		matView;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		D3DXVec3TransformCoord(&vOrbpos, &vOrbpos, &matView);

		// 뷰 스페이스 -> 투영
		_matrix		matProj;
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
		D3DXVec3TransformCoord(&vOrbpos, &vOrbpos, &matProj);

		// 투영 -> 뷰 포트
		D3DVIEWPORT9 viewport;
		m_pGraphicDev->GetViewport(&viewport);
		float width = (float)viewport.Width;
		float height = (float)viewport.Height;
		float minZ = viewport.MinZ;
		float maxZ = viewport.MaxZ;

		vOrbpos.x = (vOrbpos.x + 1.0f) * 0.5f * width + viewport.X;
		vOrbpos.y = WINCY - ((-vOrbpos.y + 1.0f) * 0.5f * height + viewport.Y);
		vOrbpos.z = vOrbpos.z * (maxZ - minZ) + minZ;

		m_pTransform->m_vInfo[INFO_POS] = vOrbpos;
		WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	}

	CTempUI::LateUpdate_Object();
}

void CUIOrbClearLight::Render_Object()
{
	if (m_IsDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();

}

HRESULT CUIOrbClearLight::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_UIOrbLight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUIOrbClearLight::Key_Input(void)
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	CGameObject* Obj = nullptr;

	if (OnCollision(pt, m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y))
	{
		if (Engine::InputDev()->Mouse_Down(DIM_LB))
		{
		}
	}
}

CUIOrbClearLight* CUIOrbClearLight::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIOrbClearLight*	pInstance = new CUIOrbClearLight(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CUIOrbClearLight Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CUIOrbClearLight::Free()
{
	CTempUI::Free();
}
