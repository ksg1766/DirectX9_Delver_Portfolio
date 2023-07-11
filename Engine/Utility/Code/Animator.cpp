#include "..\..\Header\Animator.h"
#include "Texture.h"
#include "Animation.h"

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

_int CAnimator::Update_Animator(const _float& fTimeDelta)

{

	if(m_pCurAnimation != nullptr)
	m_pCurAnimation->Update_Animation(fTimeDelta);

	return S_OK;
}

void CAnimator::LateUpdate_Component()
{
	if (m_pCurAnimation != nullptr)
	m_pCurAnimation->LateUpdate_Animation();
}

void CAnimator::Render_Animator()
{
	if (m_pCurAnimation != nullptr)
	m_pCurAnimation->Render_Animation();
}

void CAnimator::Set_Animation(STATE eState)
{
	auto iter = find_if(m_mapAnimation.begin(), m_mapAnimation.end(),
		[&](const pair<STATE, CAnimation*>& pair)
		{
			return eState == pair.first;
		});

	if (iter == m_mapAnimation.end())
		return;
	//예외처리 

	m_eCurState = eState;
	m_pCurAnimation = iter->second;
	// 현재 애니메이션의 state를 넎고
	// 애니메이션의 어떤 포인터냐에 따라 현재 애니메이션의 밸류를 받고 그 업데이트가 진행 됨.
}

HRESULT CAnimator::Add_Animation(STATE _eState, CAnimation* pAnimation)
{
	if (nullptr == pAnimation)
		return E_FAIL;
	// 예외처리

	m_mapAnimation.insert({ _eState, pAnimation });

	return S_OK;
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
	for_each(m_mapAnimation.begin(), m_mapAnimation.end(), CDeleteMap());
	m_mapAnimation.clear();
	
	Safe_Release<CAnimation*>(m_pCurAnimation);

	CComponent::Free();
}
