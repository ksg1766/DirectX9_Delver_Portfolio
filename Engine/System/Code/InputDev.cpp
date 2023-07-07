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

