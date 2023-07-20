#include "..\..\Header\Material.h"

CMaterial::CMaterial()
{
	::ZeroMemory(&m_tMaterial, sizeof(m_tMaterial));
}

CMaterial::CMaterial(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
	ZeroMemory(&m_tMaterial, sizeof(m_tMaterial));
}

CMaterial::CMaterial(const CMaterial& rhs)
	: CComponent(rhs)
{
	ZeroMemory(&m_tMaterial, sizeof(m_tMaterial));
}

CMaterial::~CMaterial()
{
}

HRESULT CMaterial::Ready_Material()
{
	m_tMaterial.Ambient = D3DCOLORVALUE{ 1.f, 1.f, 1.f, 1.f };

	return S_OK;
}

_int CMaterial::Update_Component(const _float& fTimeDelta)
{


	return _int();
}

CMaterial* CMaterial::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMaterial* pInstance = new CMaterial(pGraphicDev);
	// TODO : 매개변수 추가
	if (FAILED(pInstance->Ready_Material()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Material Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CMaterial::Clone(void)
{
	return new CMaterial(*this);
}

void CMaterial::Free()
{
	CComponent::Free();
}
