#include "Export_Utility.h"
#include "Transform.h"

CBillBoard::CBillBoard()
{

}

CBillBoard::CBillBoard(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
}

CBillBoard::CBillBoard(const CBillBoard& rhs)
	: CComponent(rhs)
{
}

CBillBoard::~CBillBoard()
{
}

HRESULT CBillBoard::Ready_BillBoard()
{

	return S_OK;
}

void CBillBoard::LateUpdate_Component()
{
	_matrix		matView, matBill;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	// 주의 사항 : 빌(자-1) * 월(I * 자 * 이)
	m_pTransform->Set_WorldMatrix(matBill * m_pTransform->WorldMatrix());
}

CBillBoard* CBillBoard::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBillBoard* pInstance = new CBillBoard(pGraphicDev);

	if (FAILED(pInstance->Ready_BillBoard()))
	{
		Safe_Release(pInstance);
		MSG_BOX("BillBoard Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CBillBoard::Clone(void)
{
	return new CBillBoard(*this);
}

void CBillBoard::Free()
{
}
