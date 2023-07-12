#include "stdafx.h"
#include "..\Header\BossExplosion.h"
#include "Export_Function.h"
#include "Terrain.h"
#include "Player.h"
CBossExplosion::CBossExplosion(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CBossExplosion::CBossExplosion(const CBossExplosion& rhs)
	:	Engine::CGameObject(rhs)
{
}

CBossExplosion::~CBossExplosion()
{
	Free();
}
//if (STATE::BOSS_METEORREADY != Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front()->Get_State())
//Engine::EventManager()->DeleteObject(this);
HRESULT CBossExplosion::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::MONSTERBULLET;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fFrame = 0.f;
	m_iCount = 0.f;
	m_pCollider->InitOBB(
		m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
		m_pTransform->LocalScale());
	m_pBasicStat->Get_Stat()->fAttack = 15.0;
	return S_OK;
}

_int CBossExplosion::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);

	m_fFrame += 16.f * fTimeDelta;

	if (8.f < m_fFrame)
	{
		Engine::EventManager()->DeleteObject(this);
		m_fFrame = 0.f;
	}

	return iExit;
}

void CBossExplosion::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	m_pBillBoard->LateUpdate_Component();
	__super::LateUpdate_Object();
	__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);
}

void CBossExplosion::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTexture->Render_Texture((_uint)m_fFrame);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBossExplosion::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		CPlayerStat& PlayerState = *(dynamic_cast<CPlayer*>(_pOther->GetHost())->Get_Stat());
		PlayerState.Take_Damage(this->Get_BasicStat()->Get_Stat()->fAttack);
		this->Set_AttackTick(true);

		Engine::EventManager()->DeleteObject(this);
	}
}

void CBossExplosion::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

#pragma region 밀어내기-> 플레이어 더 밀어낼 예정
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
			m_pTransform->Translate(_vec3(fRadiusX, 0.f, 0.f));
		else
			m_pTransform->Translate(_vec3(-fRadiusX, 0.f, 0.f));
	}
	else if (fRadiusZ == fMinAxis)
	{
		if (vOtherPos.z < vThisPos.z)
			m_pTransform->Translate(_vec3(0.f, 0.f, fRadiusZ));
		else
			m_pTransform->Translate(_vec3(0.f, 0.f, -fRadiusZ));
	}
	else //(fRadiusY == fMinAxis)
	{
		if (vOtherPos.y < vThisPos.y)
			m_pTransform->Translate(_vec3(0.f, fRadiusY, 0.f));
		else
			m_pTransform->Translate(_vec3(0.f, -fRadiusY, 0.f));
	}
#pragma endregion 밀어내기
}

void CBossExplosion::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CBossExplosion::Set_StartPos(_vec3 _vec)
{
	m_pTransform->m_vInfo[INFO_POS] = _vec;
}

void CBossExplosion::Set_StartPosY(float _fY)
{
	m_pTransform->m_vInfo[INFO_POS].y += _fY;
}

HRESULT CBossExplosion::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossExplosion"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pBillBoard = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CBossExplosion* CBossExplosion::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossExplosion* pInstance = new CBossExplosion(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BossProjectitle Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBossExplosion::Free()
{
	__super::Free();
}
