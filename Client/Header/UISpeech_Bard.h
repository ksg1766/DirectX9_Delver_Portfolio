#pragma once
#include "TempUI.h"


BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CFont;
END

class CUIspeech_Bard :
    public CTempUI
{
private:
    explicit CUIspeech_Bard(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CUIspeech_Bard();

public:
    HRESULT		Ready_Object();
    _int		Update_Object(const _float& fTimeDelta);
    void		LateUpdate_Object(void);
    void		Render_Object();

private:
    HRESULT		Add_Component(void);
    void	    Key_Input(void);
    CFont*      m_pFont = nullptr;
    ID3DXFont* m_pFontconfig[2];
    CGameObject* m_pGameObject;

    _uint      m_iSpeech;
    _bool       m_bQuest;
    
public:
    static CUIspeech_Bard* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free();
};

