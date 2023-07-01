#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct tagVertexDefault
	{
		_vec3		vPosition;

		static const _ulong	format = D3DFVF_XYZ;
	}VTXDEF;
	//const _ulong	FVF_COL = D3DFVF_XYZ;

	typedef struct tagVertexColor
	{
		_vec3		vPosition;		
		_ulong		dwColor;

		static const _ulong	format = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;
	}VTXCOL;
	//const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTexture
	{
		_vec3		vPosition;
		_vec2		vTexUV;
		static const _ulong	format = D3DFVF_XYZ | D3DFVF_TEX1;
	}VTXTEX;
	//const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef struct tagVertexCubeBfture
	{
		_vec3		vPosition;
		_vec3		vTexUV;
		static const _ulong	format = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	}VTXCUBE;

	//const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); // 텍스처의 UV 좌표 값을 FLOAT형 3개로 표현하겠다는 매크로(괄호안의 숫자 0의 의미는 본래 버텍스에 텍스쳐 UV값이 여러개가 올 수 있는데 그중 0번째 값을 지정하겠다는 의미)
	
	typedef struct tagVertexNormal
	{
		_vec3		vPosition;
		_vec3		vNormal;
		static const _ulong	format = D3DFVF_XYZ | D3DFVF_NORMAL;
	}VTXNOM;

	typedef struct tagVertexNormalColor
	{
		_vec3		vPosition;
		_vec3		vNormal;
		_ulong		dwColor;
		static const _ulong	format = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
	}VTXNCO;

	typedef struct tagVertexNormalTexture
	{
		_vec3		vPosition;
		_vec3		vNormal;
		_vec2		vTexture;
		static const _ulong	format = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	}VTXNTX;


	typedef struct tagIndex16
	{
		_ushort  _0;
		_ushort  _1;
		_ushort  _2;

	}INDEX16;

	typedef struct tagIndex32
	{
		_ulong	_0;
		_ulong	_1;
		_ulong	_2;

	}INDEX32;
	
	union COLLIDER_ID
	{
		struct
		{
			_uint Left_id;
			_uint Right_id;
		};
		ULONGLONG ID;
	};

	struct tagEvent
	{
		EVENT_TYPE	eEvent;
		DWORD_PTR	lParam;
		DWORD_PTR	wParam;
	};
}


#endif // Engine_Struct_h__
