#pragma once
#include "Environment.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CBillBoard;

END

class CTree
	: public CEnvironment
{
private:
	explicit CTree(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTree(const CTree& rhs);
	virtual ~CTree();

public:
	void			Set_TreeType(_uint _iType, _uint _iHeight)
	{
		switch (_iType)
		{
		case 0: // 초록색 둥근 타입 - 4가지 크기
			switch (_iHeight)
			{
			case 0:
				m_iTreeNumber = 0;
				break;
			case 1:
				m_iTreeNumber = 1;
				break;
			case 2:
				m_iTreeNumber = 2;
				break;
			case 3:
				m_iTreeNumber = 3;
				break;
			}
			break;

		case 1: // 연초색 둥근 타입 - 4가지 크기
			switch (_iHeight)
			{
			case 0:
				m_iTreeNumber = 4;
				break;
			case 1:
				m_iTreeNumber = 5;
				break;
			case 2:
				m_iTreeNumber = 6;
				break;
			case 3:
				m_iTreeNumber = 7;
				break;
			}
			break;

		case 2: // 진초색 둥근 타입 - 4가지 크기
			switch (_iHeight)
			{
			case 0:
				m_iTreeNumber = 8;
				break;
			case 1:
				m_iTreeNumber = 9;
				break;
			case 2:
				m_iTreeNumber = 10;
				break;
			case 3:
				m_iTreeNumber = 11;
				break;
			}
			break;

		case 3: // 시들색 둥근 타입 - 4가지 크기
			switch (_iHeight)
			{
			case 0:
				m_iTreeNumber = 12;
				break;
			case 1:
				m_iTreeNumber = 13;
				break;
			case 2:
				m_iTreeNumber = 14;
				break;
			case 3:
				m_iTreeNumber = 15;
				break;
			}
			break;

		case 4: // 초록색 세모 타입 - 2가지 크기
			switch (_iHeight)
			{
			case 0:
				m_iTreeNumber = 16;
				break;
			case 1:
				m_iTreeNumber = 17;
				break;
			}
			break;

		case 5: // 연초색 세모 타입 - 2가지 크기
			switch (_iHeight)
			{
			case 0:
				m_iTreeNumber = 18;
				break;
			case 1:
				m_iTreeNumber = 19;
				break;
			}
			break;

		case 6: // 진초색 세모 타입 - 2가지 크기
			switch (_iHeight)
			{
			case 0:
				m_iTreeNumber = 20;
				break;
			case 1:
				m_iTreeNumber = 21;
				break;
			}
			break;

		case 7: // 시들색 세모 타입 - 2가지 크기
			switch (_iHeight)
			{
			case 0:
				m_iTreeNumber = 22;
				break;
			case 1:
				m_iTreeNumber = 23;
				break;
			}
			break;
		}
	}
	void			Set_TreeNumber(_uint _iNum) { m_iTreeNumber = _iNum; }

public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void)					override;
	virtual void	Render_Object(void)						override;

public:
	_uint			Get_TreeNumber()	{ return m_iTreeNumber; }

private:
	HRESULT			Add_Component(void);

private:
	_uint           m_iType   = 0;
	_uint           m_iHeight = 0;
	_uint           m_iTreeNumber = 0;

public:
	static CTree*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free() override;

};

