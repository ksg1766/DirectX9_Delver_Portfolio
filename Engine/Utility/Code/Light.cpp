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
		//m_tLight.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_tLight.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_tLight.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_tLight.Range = 100.f;
		m_tLight.Direction = _vec3(1.f, -1.5f, 0.f);

		break;

	case LIGHT_TYPE::LIGHT_POINT:
		m_tLight.Type = D3DLIGHT_POINT;
		//m_tLight.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_tLight.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_tLight.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_tLight.Attenuation0 = 0.15f;
		m_tLight.Attenuation1 = 0.15f;
		m_tLight.Attenuation2 = 0.f;
		m_tLight.Falloff = 1.f;
		m_tLight.Range = 100.f;
		m_tLight.Position = _vec3(0.f, 5.f, 0.f);

		break;

	case LIGHT_TYPE::LIGHT_SPOT:
		m_tLight.Type = D3DLIGHT_SPOT;
		//m_tLight.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_tLight.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_tLight.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_tLight.Attenuation0 = 0.15f;
		m_tLight.Attenuation1 = 0.15f;
		m_tLight.Attenuation2 = 0.f;
		m_tLight.Falloff = 1.f;
		m_tLight.Theta = D3DXToRadian(40.f);
		m_tLight.Phi = D3DXToRadian(80.f);
		m_tLight.Range = 100.f;
		m_tLight.Position = _vec3(0.f, 5.f, 0.f);
		m_tLight.Direction = _vec3(1.f, -1.5f, 0.f);

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
	if (FAILED(pInstance->Ready_Light(LIGHT_TYPE::LIGHT_POINT)))
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
	CComponent::Free();
}
