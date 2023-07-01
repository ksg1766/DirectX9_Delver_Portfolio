#include "..\..\Header\Transform.h"

CTransform::CTransform()
	: m_pParent(nullptr)
	, m_vAngle(0.f, 0.f, 0.f)
	, m_vLocalScale(1.f, 1.f, 1.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
}

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
	, m_vAngle(0.f, 0.f, 0.f)
	, m_vLocalScale(1.f, 1.f, 1.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_vAngle(rhs.m_vAngle)
	, m_vLocalScale(rhs.m_vLocalScale)
{
	for (size_t i = 0; i < INFO_END; ++i)
		m_vInfo[i] = rhs.m_vInfo[i];
}

CTransform::~CTransform()
{
}

void CTransform::Translate(_vec3& _vTranslation)
{
	m_vInfo[INFO_POS] += _vTranslation;
}

void CTransform::Translate(const _vec3& _vTranslation)
{
	Translate(const_cast<_vec3&>(_vTranslation));
}

void CTransform::Scale(_vec3& _vScale)
{
	for (_int i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&_vScale) + i);
	}
}

void CTransform::Scale(const _vec3& _vScale)
{
	Scale(const_cast<_vec3&>(_vScale));
}

void CTransform::Rotate(_vec3& _vEulers)
{
	for(int i = 0; i < INFO_POS; ++i)
		*(((_float*)&m_vAngle) + i) += *(((_float*)&_vEulers) + i);

	_matrix matRotate;
	D3DXMatrixRotationQuaternion(&matRotate, D3DXQuaternionRotationYawPitchRoll(&_quat(), _vEulers.y, _vEulers.x, _vEulers.z));

	for(int i = 0; i < INFO_POS; ++i)
		D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRotate);
}

void CTransform::Rotate(const _vec3& _vEulers)
{
	Rotate(const_cast<_vec3&>(_vEulers));
}

void CTransform::Rotate(const _float& _fXangle, const _float& _fYangle, const _float& _fZangle)
{
	Rotate(_vec3(_fXangle, _fYangle, _fZangle));
}

void CTransform::Rotate(ROTATION eType, const _float& fAngle)
{
	_vec3 vAngle = _vec3(0.f, 0.f, 0.f);
	*(((_float*)&vAngle) + eType) += fAngle;
		
	Rotate(_vec3(vAngle[INFO_RIGHT], vAngle[INFO_UP], vAngle[INFO_LOOK]));
}

void CTransform::RotateAround(const _vec3& _vPoint, const _vec3& _vAxis, const _float& _fAngle)
{
	_quat quat;
	_matrix matRotate;
	_vec3 vOut;

	D3DXQuaternionRotationAxis(&quat, &_vAxis, _fAngle);
	D3DXMatrixRotationQuaternion(&matRotate, &quat);
	// D3DXMatrixRotationAxis(&matRotate, &_vAxis, _fAngle);
	D3DXVec3TransformCoord(&vOut, &(m_vInfo[INFO_POS] - _vPoint), &matRotate);

	m_vInfo[INFO_POS] = vOut + _vPoint;
	// 공전은 하지만 자전은 하지 않음.
}

const _matrix CTransform::WorldMatrix()
{
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);

	for (int i = 0; i < 4; ++i)
		::CopyMemory(&matWorld.m[i], m_vInfo[i], sizeof(_vec3));

	return matWorld;
}

//void CTransform::Chase_Target(const _vec3 * pTargetPos, const _float & fTimeDelta, const _float & fSpeed)
//{
//	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];
//	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fTimeDelta * fSpeed;
//
//	_matrix			matRot = *Compute_LookAtTarget(pTargetPos);
//	_matrix			matScale, matTrans;
//
//	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
//	D3DXMatrixTranslation(&matTrans, 
//		m_vInfo[INFO_POS].x,
//		m_vInfo[INFO_POS].y,
//		m_vInfo[INFO_POS].z);
//	
//	m_matWorld = matScale * matRot * matTrans;
//}
//
//const _matrix * CTransform::Compute_LookAtTarget(const _vec3 * pTargetPos)
//{
//	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];
//
//	_matrix		matRot;
//	_vec3		vAxis, vUp;
//
//	/*_vec3		vAxis = *D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir);
//
//	_matrix		matRot;
//	_vec3		vUp = m_vInfo[INFO_UP];
//	D3DXVec3Normalize(&vUp, &vUp);
//	D3DXVec3Normalize(&vDir, &vDir);
//
//	_float		fDot = D3DXVec3Dot(&vUp, &vDir);
//	_float		fAngle = acosf(fDot);
//
//	D3DXMatrixRotationAxis(&matRot, &vAxis, fAngle);*/
//
//	return D3DXMatrixRotationAxis(&matRot, 
//										 D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir),
//										acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir), 
//														 D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]))));
//}

HRESULT CTransform::Ready_Transform()
{
	::ZeroMemory(m_vInfo, sizeof(m_vInfo));
	m_vInfo[INFO_RIGHT].x = 1.f;
	m_vInfo[INFO_UP].y = 1.f;
	m_vInfo[INFO_LOOK].z = 1.f;

	return S_OK;
}

_int CTransform::Update_Component(const _float & fTimeDelta)
{
	//D3DXMatrixIdentity(&m_matWorld);

	//for (_int i = 0; i < INFO_POS; ++i)
	//	memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
	//
	//// 크기 변환
	//for (_int i = 0; i < INFO_POS; ++i)
	//{
	//	D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
	//	m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	//}
	//
	//// 회전 변환
	//_matrix			matRot[ROT_END];

	//D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	//D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	//D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	//for (_int i = 0; i < INFO_POS; ++i)
	//{
	//	for (_int j = 0; j < ROT_END; ++j)
	//	{
	//		D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
	//	}
	//}

	//// 월드 행렬 구성

	//for (_int i = 0; i < INFO_END; ++i)
	//	memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	return 0;
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform*		pInstance = new CTransform(pGraphicDev);

	if (FAILED(pInstance->Ready_Transform()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Transform Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent * CTransform::Clone(void)
{
	return new CTransform(*this);
}

void CTransform::Free()
{
	CComponent::Free();
}
