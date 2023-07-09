#include "InputDev.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInputDev)

Engine::CInputDev::CInputDev(void)
{

}

Engine::CInputDev::~CInputDev(void)
{
	Free();
}

bool CInputDev::Key_Pressing(_ubyte byKeyID)
{
	if (m_byKeyState[byKeyID] & 0x80)
		return true;

	return false;
}

bool CInputDev::Key_Down(_ubyte byKeyID)
{
	// 이전에 눌린 적이 없고, 지금 눌렸을 때

	if (!m_byKeyStateBF[byKeyID] && (m_byKeyState[byKeyID] & 0x80))
	{
		m_byKeyStateBF[byKeyID] = !m_byKeyStateBF[byKeyID];
		return true;
	}

	for (int i = 0; i < 256; ++i)
	{
		if (m_byKeyStateBF[i] && !(m_byKeyState[i] & 0x80))
			m_byKeyStateBF[i] = !m_byKeyStateBF[i];
	}

	return false;
}

bool CInputDev::Key_Up(_ubyte byKeyID)
{
	// 이전에 눌린 적이 있고, 현재는 눌리지 않은 경우
	if (m_byKeyStateBF[byKeyID] && !(m_byKeyState[byKeyID] & 0x80))
	{
		m_byKeyStateBF[byKeyID] = !m_byKeyStateBF[byKeyID];
		return true;
	}

	for (int i = 0; i < 256; ++i)
	{
		if (!m_byKeyStateBF[byKeyID] && (m_byKeyState[byKeyID] & 0x80))
			m_byKeyStateBF[i] = !m_byKeyStateBF[i];
	}

	return false;
}

_bool CInputDev::Mouse_Pressing(MOUSEKEYSTATE eMouseID)
{
	if (m_tMouseState.rgbButtons[eMouseID] & 0x80)
		return true;

	return false;
}

_bool CInputDev::Mouse_Down(MOUSEKEYSTATE eMouseID)
{
	// 이전에 눌린 적이 없고, 지금 눌렸을 때

	if (!m_ubyMouseStateBF[eMouseID] && (m_tMouseState.rgbButtons[eMouseID] & 0x80))
	{
		m_ubyMouseStateBF[eMouseID] = !m_ubyMouseStateBF[eMouseID];
		return true;
	}

	for (int i = 0; i < 4; ++i)
	{
		if (m_ubyMouseStateBF[i] && !(m_tMouseState.rgbButtons[i] & 0x80))
			m_ubyMouseStateBF[i] = !m_ubyMouseStateBF[i];
	}

	return false;
}

_bool CInputDev::Mouse_Up(MOUSEKEYSTATE eMouseID)
{
	// 이전에 눌린 적이 있고, 현재는 눌리지 않은 경우
	if (m_ubyMouseStateBF[eMouseID] && !(m_tMouseState.rgbButtons[eMouseID] & 0x80))
	{
		m_ubyMouseStateBF[eMouseID] = !m_ubyMouseStateBF[eMouseID];
		return true;
	}

	for (int i = 0; i < 4; ++i)
	{
		if (!m_ubyMouseStateBF[eMouseID] && (m_tMouseState.rgbButtons[eMouseID] & 0x80))
			m_ubyMouseStateBF[i] = !m_ubyMouseStateBF[i];
	}

	return false;
}

_bool CInputDev::Get_AnyKeyDown()
{
	if (Key_Down(DIK_ESCAPE)
		|| Key_Down(DIK_1)
		|| Key_Down(DIK_2)
		|| Key_Down(DIK_3)
		|| Key_Down(DIK_4)
		|| Key_Down(DIK_5)
		|| Key_Down(DIK_6)
		|| Key_Down(DIK_7)
		|| Key_Down(DIK_8)
		|| Key_Down(DIK_9)
		|| Key_Down(DIK_0)
		|| Key_Down(DIK_MINUS)
		|| Key_Down(DIK_EQUALS)
		|| Key_Down(DIK_BACK)
		|| Key_Down(DIK_TAB)
		|| Key_Down(DIK_Q)
		|| Key_Down(DIK_W)
		|| Key_Down(DIK_E)
		|| Key_Down(DIK_R)
		|| Key_Down(DIK_T)
		|| Key_Down(DIK_Y)
		|| Key_Down(DIK_U)
		|| Key_Down(DIK_I)
		|| Key_Down(DIK_O)
		|| Key_Down(DIK_P)
		|| Key_Down(DIK_LBRACKET)
		|| Key_Down(DIK_RBRACKET)
		|| Key_Down(DIK_RETURN)
		|| Key_Down(DIK_LCONTROL)
		|| Key_Down(DIK_A)
		|| Key_Down(DIK_S)
		|| Key_Down(DIK_D)
		|| Key_Down(DIK_F)
		|| Key_Down(DIK_G)
		|| Key_Down(DIK_H)
		|| Key_Down(DIK_J)
		|| Key_Down(DIK_K)
		|| Key_Down(DIK_L)
		|| Key_Down(DIK_SEMICOLON)
		|| Key_Down(DIK_APOSTROPHE)
		|| Key_Down(DIK_GRAVE)
		|| Key_Down(DIK_LSHIFT)
		|| Key_Down(DIK_BACKSLASH)
		|| Key_Down(DIK_Z)
		|| Key_Down(DIK_X)
		|| Key_Down(DIK_C)
		|| Key_Down(DIK_V)
		|| Key_Down(DIK_B)
		|| Key_Down(DIK_N)
		|| Key_Down(DIK_M)
		|| Key_Down(DIK_COMMA)
		|| Key_Down(DIK_PERIOD)
		|| Key_Down(DIK_SLASH)
		|| Key_Down(DIK_RSHIFT)
		|| Key_Down(DIK_MULTIPLY)
		|| Key_Down(DIK_LMENU)
		|| Key_Down(DIK_SPACE)
		|| Key_Down(DIK_CAPITAL)
		|| Key_Down(DIK_F1)
		|| Key_Down(DIK_F2)
		|| Key_Down(DIK_F3)
		|| Key_Down(DIK_F4)
		|| Key_Down(DIK_F5)
		|| Key_Down(DIK_F6)
		|| Key_Down(DIK_F7)
		|| Key_Down(DIK_F8)
		|| Key_Down(DIK_F9)
		|| Key_Down(DIK_F10)
		|| Key_Down(DIK_NUMLOCK)
		|| Key_Down(DIK_SCROLL)
		|| Key_Down(DIK_NUMPAD7)
		|| Key_Down(DIK_NUMPAD8)
		|| Key_Down(DIK_NUMPAD9)
		|| Key_Down(DIK_SUBTRACT)
		|| Key_Down(DIK_NUMPAD4)
		|| Key_Down(DIK_NUMPAD5)
		|| Key_Down(DIK_NUMPAD6)
		|| Key_Down(DIK_ADD)
		|| Key_Down(DIK_NUMPAD1)
		|| Key_Down(DIK_NUMPAD2)
		|| Key_Down(DIK_NUMPAD3)
		|| Key_Down(DIK_NUMPAD0)
		|| Key_Down(DIK_DECIMAL)
		|| Key_Down(DIK_OEM_102)
		|| Key_Down(DIK_F11)
		|| Key_Down(DIK_F12)
		|| Key_Down(DIK_F13)
		|| Key_Down(DIK_F14)
		|| Key_Down(DIK_F15)
		|| Key_Down(DIK_KANA)
		|| Key_Down(DIK_ABNT_C1)
		|| Key_Down(DIK_CONVERT)
		|| Key_Down(DIK_NOCONVERT)
		|| Key_Down(DIK_YEN)
		|| Key_Down(DIK_ABNT_C2)
		|| Key_Down(DIK_NUMPADEQUALS)
		|| Key_Down(DIK_PREVTRACK)
		|| Key_Down(DIK_AT)
		|| Key_Down(DIK_COLON)
		|| Key_Down(DIK_UNDERLINE)
		|| Key_Down(DIK_KANJI)
		|| Key_Down(DIK_STOP)
		|| Key_Down(DIK_AX)
		|| Key_Down(DIK_UNLABELED)
		|| Key_Down(DIK_NEXTTRACK)
		|| Key_Down(DIK_NUMPADENTER)
		|| Key_Down(DIK_RCONTROL)
		|| Key_Down(DIK_MUTE)
		|| Key_Down(DIK_CALCULATOR)
		|| Key_Down(DIK_PLAYPAUSE)
		|| Key_Down(DIK_MEDIASTOP)
		|| Key_Down(DIK_VOLUMEDOWN)
		|| Key_Down(DIK_VOLUMEUP)
		|| Key_Down(DIK_WEBHOME)
		|| Key_Down(DIK_NUMPADCOMMA)
		|| Key_Down(DIK_DIVIDE)
		|| Mouse_Down(DIM_LB)
		|| Mouse_Down(DIM_RB)
		|| Mouse_Down(DIM_MB))
	{
		return true;
	}

	return false;
}

HRESULT Engine::CInputDev::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{

	// DInput 컴객체를 생성하는 함수
	FAILED_CHECK_RETURN(DirectInput8Create(hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK,
		NULL), E_FAIL);

	// 키보드 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyBoard->Acquire();

	ZeroMemory(m_byKeyStateBF, sizeof(m_byKeyStateBF));

	// 마우스 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pMouse->Acquire();

	ZeroMemory(m_ubyMouseStateBF, sizeof(m_ubyMouseStateBF));

	return S_OK;
}

void Engine::CInputDev::Update_InputDev(void)
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);
}

void Engine::CInputDev::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}
