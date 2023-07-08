#include "..\..\Header\StateMachine.h"
#include "State.h"

CStateMachine::CStateMachine()
{
}

CStateMachine::CStateMachine(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CComponent(pGraphicDev)
{
}

CStateMachine::CStateMachine(const CStateMachine& rhs)
	: Engine::CComponent(rhs)
{
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

	if (eState != m_eCurState)
		Set_State(eState);
}

void CStateMachine::LateUpdate_StateMachine()
{
	m_pCurState->LateUpdate_State();
}

void CStateMachine::Render_StateMachine()
{
	m_pCurState->Render_State();
	// Animation
}

void CStateMachine::Set_State(STATE _eState)
{
	auto iter = find_if(m_StateMap.begin(), m_StateMap.end(), [&](const pair<STATE, CState*>& pair)
		{ return _eState == pair.first; });

	if (iter == m_StateMap.end())
		return;

	m_pCurState = iter->second;
	m_eCurState = _eState;

	// Animation
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
	for_each(m_StateMap.begin(), m_StateMap.end(), CDeleteMap());
	m_StateMap.clear();

	__super::Free();
}
