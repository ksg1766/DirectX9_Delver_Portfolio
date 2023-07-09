#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCamera :
    public CComponent
{
private:
	explicit CCamera();
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();

public:
	

public:
	HRESULT			Ready_Camera();
	virtual _int	Update_Component(const _float& fTimeDelta);
	virtual void	LateUpdate_Component();

public:
	_matrix&		Get_ViewMatrix() { return m_matView; }
	_matrix&		Get_ProjMatrix() { return m_matProj; }
	// TODO : 외부에서 호출할 일이 없다면 protected으로 두고 Update내에서 호출해도 됨. 충분히 고려해 볼 것. -> ???
	void			Set_ViewSpcae();
	virtual void	Set_Projection(const CAMERA_TYPE eMode = CAMERA_TYPE::PERSPECTIVE);

private:
	_matrix	m_matView;
	_matrix	m_matProj;

public:
	static CCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);
private:
	virtual void			Free();
};

END