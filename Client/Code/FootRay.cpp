#include "stdafx.h"
#include "SoundManager.h"
#include "FootRay.h"
#include "Export_Function.h"
#include "Player.h"
#include "Scene.h"
#include "CubeBlock.h"

CFootRay::CFootRay(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CFootRay::CFootRay(const CFootRay& rhs)
	: CGameObject(rhs)
{
}

CFootRay::~CFootRay()
{
	Free();
}

HRESULT CFootRay::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::RAY;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(0.000001f, 0.2f, 0.000001f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());
	//m_pTransform->Translate(m_pHost->m_pTransform->m_vInfo[INFO_POS] + _vec3(0.f, -1.25f, 0.f));
	//m_pTransform->Set_Parent(m_pHost->m_pTransform);

	return S_OK;
}

_int CFootRay::Update_Object(const _float& fTimeDelta)
{
	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);

	m_pTransform->m_vInfo[INFO_POS] = m_pHost->m_pTransform->m_vInfo[INFO_POS] + _vec3(0.f, -1.1f, 0.f);

	return iExit;
}

void CFootRay::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();

	if (!m_pColTarget)
	{
		if (!static_cast<CPlayer*>(m_pHost)->IsJump())
		{
			static_cast<CPlayer*>(m_pHost)->Set_JumpState(true);
			static_cast<CPlayer*>(m_pHost)->Get_RigidBody()->UseGravity(true);
		}
	}
}

void CFootRay::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
}

void CFootRay::OnCollisionEnter(CCollider* _pOther)
{

	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

	CGameObject* pOtherObj = _pOther->Get_Host();
	if (OBJECTTAG::BLOCK == pOtherObj->Get_ObjectTag())
	{
		// 새로 충돌한 지형은 타겟에 저장
		m_pColTarget = pOtherObj;

		if (dynamic_cast<CCubeBlock*>(pOtherObj)->Get_BlockTag() == BLOCKTAG::WATER_BLOCK)
			rPlayer.Set_InWater(true);

		// (dynamic_cast<CCubeBlock*>(pOtherObj)->Get_BlockTag() != BLOCKTAG::NORMAL_BLOCK)
		//Player.Set_DropWather(false);
	}
}

void CFootRay::OnCollisionStay(CCollider* _pOther)
{
	CGameObject* pOtherObj = _pOther->Get_Host();
	if (OBJECTTAG::BLOCK == pOtherObj->Get_ObjectTag())
	{
		// 계속 충돌 중이라면 Host가 블럭 위에 서 있는 것. 따라서 점프 ㄱㄴ
		static_cast<CPlayer*>(m_pHost)->Set_JumpState(false);
		static_cast<CPlayer*>(m_pHost)->Get_RigidBody()->UseGravity(false);
	}
}

void CFootRay::OnCollisionExit(CCollider* _pOther)
{
	CGameObject* pOtherObj = _pOther->Get_Host();
	if (OBJECTTAG::BLOCK == pOtherObj->Get_ObjectTag())
	{
		CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

		// 충돌 해제된 대상이 타겟과 일치한다면 블럭에서 벗어남. 따라서 공중에 떠 있는 상태
		// 일치하지 않는다면 다른 블럭위로 이동하여 Enter가 먼저호출된 것이므로 nullptr초기화 필요없음
		/*if (pOtherObj == m_pColTarget)
		{
			m_pColTarget = nullptr;
			static_cast<CPlayer*>(m_pHost)->Set_JumpState(true);
			static_cast<CPlayer*>(m_pHost)->Get_RigidBody()->UseGravity(true);
		}*/

		m_pColTarget = nullptr;
		static_cast<CPlayer*>(m_pHost)->Set_JumpState(true);
		static_cast<CPlayer*>(m_pHost)->Get_RigidBody()->UseGravity(true);

		if (dynamic_cast<CCubeBlock*>(pOtherObj)->Get_BlockTag() == BLOCKTAG::WATER_BLOCK)
			rPlayer.Set_InWater(false);
	}
}

HRESULT CFootRay::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCollider = static_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CFootRay* CFootRay::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFootRay* pInstance = new CFootRay(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("FootRay Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CFootRay::Free()
{
	__super::Free();
}
