#include "..\..\Header\StateMachine.h"
#include "State.h"
#include "Animator.h"

CStateMachine::CStateMachine()
{
}

CStateMachine::CStateMachine(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CComponent(pGraphicDev), m_FirstOn(true)
{
}

CStateMachine::CStateMachine(const CStateMachine& rhs)
	: Engine::CComponent(rhs), m_FirstOn(rhs.m_FirstOn)
{
	m_StateMap = rhs.m_StateMap;
	m_pCurState = rhs.m_pCurState;
	m_pPrevState = rhs.m_pPrevState;
	m_pAnimator = rhs.m_pAnimator;
}

CStateMachine::~CStateMachine()
{
	
}

HRESULT CStateMachine::Ready_StateMachine()
{


	return S_OK;
}

void CStateMachine::Update_StateMachine(const _float& fTimeDelta)
{
	STATE	eState = m_pCurState->Update_State(fTimeDelta);

	if(m_pAnimator != nullptr)
	m_pAnimator->Update_Animator(fTimeDelta);

	if (eState != m_eCurState)
		Set_State(eState);
	
}

void CStateMachine::LateUpdate_StateMachine()
{
	m_pCurState->LateUpdate_State();

	if (m_pAnimator != nullptr)
	m_pAnimator->LateUpdate_Component();
}

void CStateMachine::Render_StateMachine()
{
	m_pCurState->Render_State();

	if (m_pAnimator != nullptr)
	m_pAnimator->Render_Animator();
	// Animation
}

void CStateMachine::Set_State(STATE _eState)
{
	auto iter = find_if(m_StateMap.begin(), m_StateMap.end(), [&](const pair<STATE, CState*>& pair)
		{ return _eState == pair.first; });

	if (iter == m_StateMap.end())
		return;

	m_ePrevState = m_eCurState;
	m_pPrevState = m_pCurState;
	m_pCurState = iter->second;

	if (m_FirstOn)
		m_pPrevState = iter->second;

	m_eCurState = _eState;

	// Animation
	if (m_pAnimator != nullptr)
		m_pAnimator->Set_Animation(_eState);

	// 여기서 애니메이션의 키 값도 같이 설정해준다.
}

HRESULT CStateMachine::Add_State(STATE _eState, CState* pState)
{
	if (nullptr == pState)
		return E_FAIL;

	m_StateMap.insert({ _eState, pState });


	return S_OK;
}

CStateMachine* CStateMachine::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStateMachine* pInstance = new CStateMachine(pGraphicDev);

	if (FAILED(pInstance->Ready_StateMachine()))
	{
		Safe_Release<CStateMachine*>(pInstance);

		MSG_BOX("StateMachine Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CStateMachine::Clone()
{
	return new CStateMachine(*this);
}

void CStateMachine::Free()
{
	__super::Free();

	for_each(m_StateMap.begin(), m_StateMap.end(), CDeleteMap());
	m_StateMap.clear();

	Safe_Release<CState*>(m_pCurState);
	Safe_Release<CState*>(m_pPrevState);
	Safe_Release<CAnimator*>(m_pAnimator);


}
