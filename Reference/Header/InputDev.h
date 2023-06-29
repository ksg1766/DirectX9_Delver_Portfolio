#ifndef InputDev_h__
#define InputDev_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CInputDev : public CBase
{
	DECLARE_SINGLETON(CInputDev)

private:
	explicit CInputDev(void);
	virtual ~CInputDev(void);
	
public:
	_byte	Get_DIKeyState(_ubyte byKeyID)			{ return m_byKeyState[byKeyID]; }
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse) 	{ 	return m_tMouseState.rgbButtons[eMouse]; 	}
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)	
	{	
		return *(((_long*)&m_tMouseState) + eMouseState);	
	}
	
	bool	Key_Pressing(_ubyte byKeyID);
	bool	Key_Down(_ubyte byKeyID);
	bool	Key_Up(_ubyte byKeyID);

public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Update_InputDev(void);

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse	= nullptr;

private:
	_byte					m_byKeyState[256];		// 키보드에 있는 모든 키값을 저장하기 위한 변수
	_byte					m_byKeyStateBF[256];	// 입력 정보 백업 버퍼
	DIMOUSESTATE			m_tMouseState;	

public:
	virtual void	Free(void);

};
END
#endif // InputDev_h__
