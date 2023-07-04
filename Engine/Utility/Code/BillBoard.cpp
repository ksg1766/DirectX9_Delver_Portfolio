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
	//_matrix		matView, matBill;

	//D3DXMatrixInverse(&matView, 0,
	//	&SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->WorldMatrix());
	////m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//D3DXMatrixIdentity(&matBill);

	//matBill._11 = matView._11;
	//matBill._13 = matView._13;
	//matBill._31 = matView._31;
	//matBill._33 = matView._33;

	//D3DXMatrixInverse(&matBill, 0, &matBill);

	//// 주의 사항 : 빌(자-1) * 월(I * 자 * 이)
	//m_pTransform->Set_WorldMatrix(matBill * m_pTransform->WorldMatrix());

	// 
	m_pTransform->m_vInfo[INFO_LOOK] = *D3DXVec3Normalize(&_vec3(), &(m_pTransform->m_vInfo[INFO_POS] -
		SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS]));
	m_pTransform->m_vInfo[INFO_RIGHT] = *D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pTransform->m_vInfo[INFO_LOOK]);
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
	CComponent::Free();
}
