#include "Export_Utility.h"
#include "Monster.h"
#include "PlayerStat.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_vCenterPos(_vec3(-50.f, 0.f, 20.f)), m_bKnockback(false)
	, m_fDeadCoolTime(0.f)
{
}

CMonster::CMonster(const CMonster& rhs)
	: CGameObject(rhs), m_vCenterPos(rhs.m_vCenterPos), m_bKnockback(rhs.m_bKnockback)
	, m_fDeadCoolTime(rhs.m_fDeadCoolTime)
{
}

CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_Object()
{
	
	return S_OK;
}

_int CMonster::Update_Object(const _float& fTimeDelta)
{
	if(m_pRigidBody)
		m_pRigidBody->Update_RigidBody(fTimeDelta);
	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CMonster::OnCollisionEnter(CCollider* _pOther)
{
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

	if (fRadiusY == fMinAxis)
	{
		if (vOtherPos.y < vThisPos.y)
		{
			if (m_pRigidBody)
			m_pRigidBody->Set_Force(_vec3(0.f, 0.f, 0.f));

			m_pTransform->Translate(_vec3(0.f, fRadiusY - 0.000001f, 0.f));
		}
		else
			m_pTransform->Translate(_vec3(0.f, -fRadiusY, 0.f));
	}
	else if (fRadiusZ == fMinAxis)
	{
		if (vOtherPos.z < vThisPos.z)
			m_pTransform->Translate(_vec3(0.f, 0.f, fRadiusZ));
		else
			m_pTransform->Translate(_vec3(0.f, 0.f, -fRadiusZ));
	}
	else //(fRadiusX == fMinAxis)
	{
		if (vOtherPos.x < vThisPos.x)
			m_pTransform->Translate(_vec3(fRadiusX, 0.f, 0.f));
		else
			m_pTransform->Translate(_vec3(-fRadiusX, 0.f, 0.f));
	}
}

void CMonster::OnCollisionStay(CCollider* _pOther)
{
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

	if (fRadiusY == fMinAxis)
	{
		if (vOtherPos.y < vThisPos.y)
		{
			if (m_pRigidBody)
				m_pRigidBody->Set_Force(_vec3(0.f, 0.f, 0.f));

			m_pTransform->Translate(_vec3(0.f, fRadiusY - 0.000001f, 0.f));
		}
		else
			m_pTransform->Translate(_vec3(0.f, -fRadiusY, 0.f));
	}
	else if (fRadiusZ == fMinAxis)
	{
		if (vOtherPos.z < vThisPos.z)
			m_pTransform->Translate(_vec3(0.f, 0.f, fRadiusZ));
		else
			m_pTransform->Translate(_vec3(0.f, 0.f, -fRadiusZ));
	}
	else //(fRadiusX == fMinAxis)
	{
		if (vOtherPos.x < vThisPos.x)
			m_pTransform->Translate(_vec3(fRadiusX, 0.f, 0.f));
		else
			m_pTransform->Translate(_vec3(-fRadiusX, 0.f, 0.f));
	}
}

void CMonster::OnCollisionExit(CCollider* _pOther)
{

}

void CMonster::IsAttack(CBasicStat* _PlayerStat)
{
	_int iDamage = 1 + rand() % (m_pBasicStat->Get_Stat()->iDamageMin +
		(m_pBasicStat->Get_Stat()->iDamageMax - m_pBasicStat->Get_Stat()->iDamageMin));

	_int iDeffence = 1 + rand() % (_PlayerStat->Get_Stat()->iArmorMin + (_PlayerStat->Get_Stat()->iArmorMax - _PlayerStat->Get_Stat()->iArmorMin));

	_int iResultDamage = iDamage - iDeffence;

	if (iResultDamage <= 0)
		iResultDamage = 1;

	_PlayerStat->Take_Damage(iResultDamage);
}


void CMonster::Free(void)
{
	__super::Free();
}
