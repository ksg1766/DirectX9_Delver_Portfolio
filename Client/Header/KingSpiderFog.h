#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTransform;
class CTexture;
class CBillBoard;
END

class CKingSpiderFog :
    public CGameObject
{
private:
    explicit CKingSpiderFog(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CKingSpiderFog(const CKingSpiderFog& rhs);
    virtual ~CKingSpiderFog();
public:
    virtual HRESULT Ready_Object(void)						override;
    virtual _int	Update_Object(const _float& fTimeDelta)	override;
    virtual void	LateUpdate_Object(void)					override;
    virtual void	Render_Object(void);

private:
    CRcTex* m_pBuffer = nullptr;
    CTexture* m_pTexture = nullptr;
    _float      m_fDelay;
    _float      m_fScale;
private:
    HRESULT			Add_Component(void);
public:
    static CKingSpiderFog* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
    virtual void Free() override;
};

