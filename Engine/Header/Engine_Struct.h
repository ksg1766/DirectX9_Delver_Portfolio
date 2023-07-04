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
	
	typedef struct tagVertexColor
	{
		_vec3		vPosition;		
		_ulong		dwColor;

		static const _ulong	format = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;
	}VTXCOL;
	
	typedef struct tagVertexTexture
	{
		_vec3		vPosition;
		_vec2		vTexUV;
		static const _ulong	format = D3DFVF_XYZ | D3DFVF_TEX1;
	}VTXTEX;
	
	typedef struct tagVertexCubeBfture
	{
		_vec3		vPosition;
		_vec3		vTexUV;
		static const _ulong	format = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	}VTXCUBE;

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
