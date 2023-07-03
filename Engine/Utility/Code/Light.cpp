#include "..\..\Header\Light.h"

CLight::CLight() :m_bOn(true)
{
	::ZeroMemory(&m_tLight, sizeof(m_tLight));
}

CLight::CLight(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev), m_bOn(true)
{
	ZeroMemory(&m_tLight, sizeof(m_tLight));
}

CLight::CLight(const CLight& rhs)
	: CComponent(rhs), m_bOn(true)
{
	ZeroMemory(&m_tLight, sizeof(m_tLight));
}

CLight::~CLight()
{
}

HRESULT CLight::Ready_Light(LIGHT_TYPE _eLightType)
{
	switch (_eLightType)
	{
	case LIGHT_TYPE::LIGHT_DIRECTIONAL:
		m_tLight.Type = D3DLIGHT_DIRECTIONAL;
		//
		break;
	case LIGHT_TYPE::LIGHT_POINT:
		m_tLight.Type = D3DLIGHT_POINT;
		//
		break;
	case LIGHT_TYPE::LIGHT_SPOT:
		m_tLight.Type = D3DLIGHT_SPOT;
		//
		break;
	}
    return S_OK;
}

_int CLight::Update_Component(const _float& fTimeDelta)
{
	return _int();
}

CLight* CLight::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLight* pInstance = new CLight(pGraphicDev);
	// TODO : 매개변수 추가
	if (FAILED(pInstance->Ready_Light()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Light Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CLight::Clone(void)
{
	return new CLight(*this);
}

void CLight::Free()
{
}
