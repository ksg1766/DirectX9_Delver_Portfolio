#include "..\..\Header\Animator.h"

CAnimator::CAnimator()
{
}

CAnimator::CAnimator(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
}

CAnimator::CAnimator(const CAnimator& rhs)
	: CComponent(rhs)
{
}

CAnimator::~CAnimator()
{
}

HRESULT CAnimator::Ready_Animator()
{


	return S_OK;
}

_int CAnimator::LateUpdate_Component(const _float& fTimeDelta)
{
	return _int();
}

CAnimator* CAnimator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAnimator* pInstance = new CAnimator(pGraphicDev);

	if (FAILED(pInstance->Ready_Animator()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Animator Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CAnimator::Clone(void)
{
	return new CAnimator(*this);
}

void CAnimator::Free()
{
}
