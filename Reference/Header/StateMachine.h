#pragma once
#include "Component.h"

BEGIN(Engine)

class CState;
class CGameObject;


class ENGINE_DLL CStateMachine : public CComponent
{
private:
	explicit CStateMachine();
	explicit CStateMachine(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStateMachine(const CStateMachine& rhs);
	virtual	~CStateMachine();

public:
	HRESULT		Ready_StateMachine();
	// 현재 상태의 업데이트 반환 상태 타입을 받아 비교하여 상태가 변했는지 판단?
	void	Update_StateMachine(const _float& fTimeDelta);
	void	LateUpdate_StateMachine();
	void	Render_StateMachine();

public://Animation tap

public:

	// 상태를 설정 및 변경해주는 함수(애니메이터 까지 바꾼다.)
	void		Set_State(STATE _eState);
	// 상태를 맵 컨테이너에 추가
	HRESULT		Add_State(STATE _eState, CState* pState);

public:
	STATE		Get_State() { return m_eCurState; }

protected:
	// 상태 들을 담는 맵 컨테이너
	map<STATE, CState*>		m_StateMap;

	// 어떤 상태인지 담는 정하는 변수
	CState* m_pCurState = nullptr;
	STATE	m_eCurState = STATE::STATE_END;
	
	// AI 내부에서 STATE 전환 시 쿨타임으로 사용할 변수

	
	// 어떤 객체가 클론 했는지 확인

	// 애니메이션 

public:
	static	CStateMachine* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual	void	Free();
};

END

