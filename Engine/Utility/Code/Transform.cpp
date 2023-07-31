#include "..\..\Header\Transform.h"
#include "GameObject.h"

CTransform::CTransform()
	: m_pParent(nullptr)
	, m_vLocalScale(1.f, 1.f, 1.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
}

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
	, m_pParent(nullptr)
	, m_vLocalScale(1.f, 1.f, 1.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_pParent(rhs.m_pParent)
	, m_pChild(rhs.m_pChild)
	, m_vLocalScale(rhs.m_vLocalScale)
{
	for (size_t i = 0; i < INFO_END; ++i)
		m_vInfo[i] = rhs.m_vInfo[i];

	m_pChild = rhs.m_pChild;
}

CTransform::~CTransform()
{
}

void CTransform::Translate(_vec3& _vTranslation)
{
	m_vInfo[INFO_POS] += _vTranslation;
	if (!m_pChild.empty())
	{
		for(auto& iter : m_pChild)
			iter->Translate(_vTranslation);
	}
}

void CTransform::Translate(const _vec3& _vTranslation)
{
	Translate(const_cast<_vec3&>(_vTranslation));
}

void CTransform::Scale(_vec3& _vScale)
{
	/*_vec3 vScale;
	vScale.x = D3DXVec3Length(&m_vInfo[INFO_RIGHT]);
	vScale.y = D3DXVec3Length(&m_vInfo[INFO_UP]);
	vScale.z = D3DXVec3Length(&m_vInfo[INFO_LOOK]);*/

	for (_int i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		//m_vInfo[i] *= (*(((_float*)&vScale) + i) * *(((_float*)&_vScale) + i));
		m_vInfo[i] *= *(((_float*)&_vScale) + i);
	}
	if (!m_pChild.empty()) 
	{
		//m_pChild->Scale(_vScale); 
		//for (auto& iter : m_pChild)
			//iter->Scale(_vScale);
	}
}

void CTransform::Scale(const _vec3& _vScale)
{
	Scale(const_cast<_vec3&>(_vScale));
}

void CTransform::Rotate(_vec3& _vEulers)
{
	_matrix matRotate;
	_quat quat;
	_vec3 vRotAxis;

	if (0.f != _vEulers.y)
		D3DXQuaternionRotationAxis(&quat, &_vec3(0.f, 1.f, 0.f), _vEulers.y);
	else if (0.f != _vEulers.x)
		D3DXQuaternionRotationAxis(&quat, &m_vInfo[INFO_RIGHT], _vEulers.x);
	else if (0.f != _vEulers.z)
		D3DXQuaternionRotationAxis(&quat, &m_vInfo[INFO_LOOK], _vEulers.z);
	else
		return;

	D3DXMatrixRotationQuaternion(&matRotate, &quat);

	for(int i = 0; i < INFO_POS; ++i)
		D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRotate);

	if (!m_pChild.empty())
	{
		if( 0.f != _vEulers.y )
			for(auto& iter : m_pChild)
				iter->RotateAround(m_vInfo[INFO_POS], _vec3(0.f, 1.f, 0.f), _vEulers.y);
		if (0.f != _vEulers.x)
			for(auto& iter : m_pChild)
				iter->RotateAround(m_vInfo[INFO_POS], m_vInfo[INFO_RIGHT], _vEulers.x);
		if (0.f != _vEulers.z)
			for(auto& iter : m_pChild)
				iter->RotateAround(m_vInfo[INFO_POS], m_vInfo[INFO_LOOK], _vEulers.z);
	}
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

	// D3DXMatrixRotationAxis(&matRotate, &_vAxis, _fAngle);
	D3DXQuaternionRotationAxis(&quat, &_vAxis, _fAngle);
	D3DXMatrixRotationQuaternion(&matRotate, &quat);
	D3DXVec3TransformCoord(&vOut, &(m_vInfo[INFO_POS] - _vPoint), &matRotate);
	for (int i = 0; i < INFO_POS; ++i)
		D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRotate);

	m_vInfo[INFO_POS] = vOut + _vPoint;
}

const _matrix CTransform::WorldMatrix()
{
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);

	for (int i = 0; i < INFO_END; ++i)
		::CopyMemory(&matWorld.m[i], m_vInfo[i], sizeof(_vec3));

	return matWorld;
}

void CTransform::Set_WorldMatrix(_matrix& _matWorld)
{
	for (_int i = 0; i < INFO_END; ++i)
		::CopyMemory(&m_vInfo[i], &_matWorld.m[i][0], sizeof(_vec3));
}

_vec3 CTransform::LocalScale()
{
	_vec3 vScale;
	for (_int i = 0; i < INFO_POS; ++i)
		*(((_float*)&vScale) + i) = D3DXVec3Length(&m_vInfo[i]);

	return vScale;
}

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
	if (m_pParent)
	{
		for_each(m_pChild.begin(), m_pChild.end(), CDeleteObj());
		m_pChild.clear();
	}
		//m_pParent->m_pChild = nullptr;
	CComponent::Free();
}
