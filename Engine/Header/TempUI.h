#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;

class ENGINE_DLL CTempUI : public CGameObject
{
protected:
	explicit CTempUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTempUI(const CTempUI& rhs);
	virtual ~CTempUI();

public:
	const _matrix   WorldMatrix(float _fX, float _fY, float _fSizeX, float _fSizeY);
	void Set_Popup(_bool _Use) { m_IsDead = _Use; }
	void Set_UIObjID(UIOBJECTTTAG _UIObjID, _uint _UINumber) {
		m_UIObjID  = _UIObjID;
		m_UINumber = _UINumber;
	}
	void Get_UIObjID(UIOBJECTTTAG& _UIObjID, _uint& _UINumber) {
		_UIObjID = m_UIObjID;
		_UINumber = m_UINumber;
	}
	CGameObject* Get_Parent()             { return m_pParent; }
	CGameObject* Get_Child()              { return m_pChild; }
	CGameObject* Get_BeforeChild()        { return m_pBeforeChild; }
	void Set_UIImage(_uint _UINumber)     { m_fCurrentImage = _UINumber; }
	void Set_Setup(_bool _Setup)          { m_bSetup = _Setup; }
	_bool Get_EmptyBool()                 { return m_bEmpty; }
	void Set_EmptyBool(_bool _Empty)      { m_bEmpty = _Empty; }
	void Set_Parent(CGameObject* _parent) { m_pParent = _parent; }
	void Set_BeforeChild(CGameObject* _Beforechild) { m_pBeforeChild = _Beforechild; }
	void Set_NextChild(CGameObject* _Nextchild)     { m_pNextChild = _Nextchild; }
	void Set_Child(CGameObject* _child)  
	{ 
		if (_child != nullptr) {
			m_bChildExit = false;
			m_bChildEntrance = true;
		}
		else {
			m_bChildExit = true;
			m_bChildEntrance = false;
		}

		m_pChild = _child; 
	}

	HRESULT		Ready_Object();
	_int	    Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object(void);

protected:
	virtual HRESULT	Add_Component(void) { return S_OK; };
	virtual void	Key_Input(void) {};
	_bool           OnCollision(POINT& MousePoint, float _fX, float _fY, float _fSizeX, float _fSizeY);

protected:
	CRcTex*      m_pBufferCom;
	CTexture*    m_pTextureCom;

	//_bool      m_bPopup;
	_bool        m_bCollider;
	_bool        m_bSetup;
	_bool        m_bEmpty;

	_matrix      m_matWorld;
	_uint        m_fCurrentImage;

	UIOBJECTTTAG m_UIObjID;
	_uint        m_UINumber;

	CGameObject* m_pParent      = nullptr;
	CGameObject* m_pChild       = nullptr;

	CGameObject* m_pBeforeChild = nullptr;
	CGameObject* m_pNextChild   = nullptr;

	_bool        m_bChildEntrance = false;
	_bool        m_bChildExit = true;

protected:
	virtual void	Free(void);
};

END
