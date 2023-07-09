#include "stdafx.h"
#include "..\Header\SkeletonKing.h"
#include "Export_Function.h"
#include "Terrain.h"
#include "BossProjectile.h"
#include "BossExplosion.h"
#include "SkeletonKing_Clone.h"

CSkeletonKing::CSkeletonKing(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CSkeletonKing::CSkeletonKing(const CSkeletonKing& rhs)
	: Engine::CGameObject(rhs)
{
}

CSkeletonKing::~CSkeletonKing()
{
}

HRESULT CSkeletonKing::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::BOSS;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->Scale(_vec3(3.f, 3.f, 1.f));

	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

	m_pTransform->Translate(_vec3(5.f, 0.f, 5.f));

	return S_OK;
}

_int CSkeletonKing::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);
	m_fFrame += 8.f * fTimeDelta;

	if (8.f < m_fFrame)
		m_fFrame = 0.f;
	ForceHeight(m_pTransform->m_vInfo[INFO_POS]);
	Key_Input(fTimeDelta);
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	return iExit;
}

void CSkeletonKing::LateUpdate_Object(void)
{
	m_pBillBoard->LateUpdate_Component();
	__super::LateUpdate_Object();
}

void CSkeletonKing::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTexture->Render_Texture((_uint)m_fFrame);
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CSkeletonKing::ForceHeight(_vec3 _vPos)
{
	_float x = (VTXCNTX * VTXITV / 2.f) + _vPos.x;
	_float z = (VTXCNTZ * VTXITV / 2.f) + _vPos.z;

	x /= (_float)VTXITV;
	z /= (_float)VTXITV;

	_int col = ::floorf(x);
	_int row = ::floorf(z);

	_vec3 A = m_pTerrain->LoadTerrainVertex()[row * VTXCNTX + col];
	_vec3 B = m_pTerrain->LoadTerrainVertex()[row * VTXCNTX + col + 1];
	_vec3 C = m_pTerrain->LoadTerrainVertex()[(row + 1) * VTXCNTX + col];
	_vec3 D = m_pTerrain->LoadTerrainVertex()[(row + 1) * VTXCNTX + col + 1];

	_float dx = x - col;
	_float dz = z - row;

	_float height;

	if (dz < 1.0f - dx)
	{

		_vec3 uy = B - A;
		_vec3 vy = C - A;

		height = A.y + (uy.y * dx) + (vy.y * dz) + 1.f;
		m_pTransform->m_vInfo[INFO_POS].y = height;
	}
	else
	{
		_vec3 uy = C - D;
		_vec3 vy = B - D;

		height = D.y + (uy.y * (1.f - dx)) + (vy.y * (1.f - dz)) + 1.f;
		m_pTransform->m_vInfo[INFO_POS].y = height+2.f;
	}
}

void CSkeletonKing::OnCollisionEnter(CCollider* _pOther)
{
#pragma region 밀어내기
	CTransform* pOtherTransform = _pOther->Get_Transform();

	_vec3	vOtherPos = _pOther->GetCenterPos();
	_float* fOtherAxis = _pOther->GetAxisLen();

	_vec3	vThisPos = m_pCollider->GetCenterPos();
	_float* fThisAxis = m_pCollider->GetAxisLen();

	// OBJECTTAG에 따른 예외 처리 가능성
	_float fWidth = fabs(vOtherPos.x - vThisPos.x);
	_float fHeight = fabs(vOtherPos.y - vThisPos.y);
	_float fDepth = fabs(vOtherPos.z - vThisPos.z);

	_float fRadiusX = (fOtherAxis[0] + fThisAxis[0]) - fWidth;
	_float fRadiusY = (fOtherAxis[1] + fThisAxis[1]) - fHeight;
	_float fRadiusZ = (fOtherAxis[2] + fThisAxis[2]) - fDepth;

	_float fMinAxis = min(min(fRadiusX, fRadiusY), fRadiusZ);	// 가장 작은 값이 가장 얕게 충돌한 축. 이 축을 밀어내야 함.

	if (fRadiusX == fMinAxis)
	{
		if (vOtherPos.x < vThisPos.x)
		{
			pOtherTransform->Translate(_vec3(-fRadiusX, 0.f, 0.f));
		}
		else
		{
			pOtherTransform->Translate(_vec3(fRadiusX, 0.f, 0.f));
		}
	}
	else if (fRadiusZ == fMinAxis)
	{
		if (vOtherPos.z < vThisPos.z)
		{
			pOtherTransform->Translate(_vec3(0.f, 0.f, -fRadiusZ));
		}
		else
		{
			pOtherTransform->Translate(_vec3(0.f, 0.f, fRadiusZ));
		}
	}
	else //(fRadiusY == fMinAxis)
	{
		if (vOtherPos.y < vThisPos.y)
		{
			pOtherTransform->Translate(_vec3(0.f, -fRadiusY, 0.f));
		}
		else
		{
			pOtherTransform->Translate(_vec3(0.f, fRadiusY, 0.f));
		}
	}
#pragma endregion 밀어내기
}

void CSkeletonKing::OnCollisionStay(CCollider* _pOther)
{
#pragma region 밀어내기
	CTransform* pOtherTransform = _pOther->Get_Transform();

	_vec3	vOtherPos = _pOther->GetCenterPos();
	_float* fOtherAxis = _pOther->GetAxisLen();

	_vec3	vThisPos = m_pCollider->GetCenterPos();
	_float* fThisAxis = m_pCollider->GetAxisLen();

	// OBJECTTAG에 따른 예외 처리 가능성
	_float fWidth = fabs(vOtherPos.x - vThisPos.x);
	_float fHeight = fabs(vOtherPos.y - vThisPos.y);
	_float fDepth = fabs(vOtherPos.z - vThisPos.z);

	_float fRadiusX = (fOtherAxis[0] + fThisAxis[0]) - fWidth;
	_float fRadiusY = (fOtherAxis[1] + fThisAxis[1]) - fHeight;
	_float fRadiusZ = (fOtherAxis[2] + fThisAxis[2]) - fDepth;

	_float fMinAxis = min(min(fRadiusX, fRadiusY), fRadiusZ);	// 가장 작은 값이 가장 얕게 충돌한 축. 이 축을 밀어내야 함.

	if (fRadiusX == fMinAxis)
	{
		if (vOtherPos.x < vThisPos.x)
		{
			pOtherTransform->Translate(_vec3(-fRadiusX, 0.f, 0.f));
		}
		else
		{
			pOtherTransform->Translate(_vec3(fRadiusX, 0.f, 0.f));
		}
	}
	else if (fRadiusZ == fMinAxis)
	{
		if (vOtherPos.z < vThisPos.z)
		{
			pOtherTransform->Translate(_vec3(0.f, 0.f, -fRadiusZ));
		}
		else
		{
			pOtherTransform->Translate(_vec3(0.f, 0.f, fRadiusZ));
		}
	}
	else //(fRadiusY == fMinAxis)
	{
		if (vOtherPos.y < vThisPos.y)
		{
			pOtherTransform->Translate(_vec3(0.f, -fRadiusY, 0.f));
		}
		else
		{
			pOtherTransform->Translate(_vec3(0.f, fRadiusY, 0.f));
		}
	}
#pragma endregion 밀어내기
}

void CSkeletonKing::OnCollisionExit(CCollider* _pOther)
{

}

HRESULT CSkeletonKing::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Boss"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pBillBoard = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pBossAI = dynamic_cast<CBossAI*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Boss_AI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::MONSTERAI, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

//테스트 용 코드입니다.
void CSkeletonKing::Key_Input(const _float& fTimeDelta)
{
	Engine::CGameObject* pGameObject = nullptr;

	if (Engine::InputDev()->Key_Down(DIK_R))
	{
		//평타
			pGameObject = CBossProjectile::Create(m_pGraphicDev);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			dynamic_cast<CBossProjectile*>(pGameObject)->Set_Terrain(m_pTerrain);
			dynamic_cast<CBossProjectile*>(pGameObject)->Set_Target(m_pTransform->m_vInfo[INFO_POS]);
	}

	if (Engine::InputDev()->Key_Down(DIK_T))
	{
		//보스 위치에서 터지기
			pGameObject = CBossExplosion::Create(m_pGraphicDev);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			dynamic_cast<CBossExplosion*>(pGameObject)->Set_StartPos(m_pTransform->m_vInfo[INFO_POS]);
			dynamic_cast<CBossExplosion*>(pGameObject)->Set_StartPosY(-2.f);
	}

	if (Engine::InputDev()->Key_Down(DIK_Y))
	{
		//플레이어 위치에서 터지기
		pGameObject = CBossExplosion::Create(m_pGraphicDev);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		dynamic_cast<CBossExplosion*>(pGameObject)->Set_StartPos(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS]);
	}

	if (Engine::InputDev()->Key_Down(DIK_U))
	{
		pGameObject = CSkeletonKing_Clone::Create(m_pGraphicDev);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->Set_Terrain(m_pTerrain);
		dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pTransform->m_vInfo[INFO_POS];
		m_pBossAI->Teleport();
	}
}
//테스트 용 코드입니다.


CSkeletonKing* CSkeletonKing::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkeletonKing* pInstance = new CSkeletonKing(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SkeletonKing Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CSkeletonKing::Free()
{
	__super::Free();
}
