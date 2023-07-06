#include "..\..\Header\TempUI.h"

CTempUI::CTempUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CTempUI::CTempUI(const CTempUI& rhs)
	: CGameObject(rhs.m_pGraphicDev)//, ~~~~
{
}

CTempUI::~CTempUI()
{
}

HRESULT CTempUI::Ready_Object()
{
	m_pBufferCom = nullptr;
	m_pTextureCom = nullptr;

	//m_bPopup = false;
	m_bCollider = false;

	m_fCurrentImage = 0;

	//D3DXMatrixIdentity(&m_matWorld);
	//m_fX = 0.f;
	//m_fY = 0.f;
	//m_fSizeX = 0.f;
	//m_fSizeY = 0.f;

	return S_OK;
}

_int CTempUI::Update_Object(const _float & fTimeDelta)
{
	//_int iExit = CGameObject::Update_Object(fTimeDelta);
	Key_Input();

	return 0;
}

void CTempUI::LateUpdate_Object(void)
{
	//CGameObject::LateUpdate_Object();
}

void CTempUI::Render_Object(void)
{
}

const _matrix CTempUI::WorldMatrix(float _fX, float _fY, float _fSizeX, float _fSizeY)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_matWorld._11 = _fSizeX;
	m_matWorld._22 = _fSizeY;
	m_matWorld._41 = _fX;
	m_matWorld._42 = _fY;

	return m_matWorld;
}

_bool CTempUI::OnCollision(POINT& MousePoint, float _fX, float _fY, float _fSizeX, float _fSizeY)
{
	//POINT MousePoint{};
	//GetCursorPos(&MousePoint);           
	//ScreenToClient(g_hWnd, &MousePoint);

	MousePoint.y = WINCY - MousePoint.y; 

	float distX = fabs(MousePoint.x - _fX);
	float distY = fabs(MousePoint.y - _fY);

	float ThresholdX = _fSizeX / 2;
	float ThresholdY = _fSizeY / 2;

	if (distX <= (_fSizeX / 2) + ThresholdX && distY <= (_fSizeY / 2) + ThresholdY) {
		return true; 
	}

	return false;
}

void CTempUI::Free(void)
{
	CGameObject::Free();
}
