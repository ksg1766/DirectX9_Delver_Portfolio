#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CBillBoard;

END

class CMoon : public Engine::CGameObject
{
private:
	explicit CMoon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMoon(const CMoon& rhs);
	virtual ~CMoon();

public:

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

private:
	HRESULT		Add_Component(void);

private:
	CRcTex* m_pBufferCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CBillBoard* m_pBillBoardCom = nullptr;

public:
	static CMoon* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};