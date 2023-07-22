#ifndef Engine_Function_h__
#define Engine_Function_h__

#include "Engine_Typedef.h"

namespace Engine
{
	// 템플릿은 기능의 정해져있으나 자료형은 정해져있지 않은 것
	// 기능을 인스턴스화 하기 위하여 만들어두는 틀

	template<typename T>
	void	Safe_Delete(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	void	Safe_Delete_Array(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete [] Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	unsigned long Safe_Release(T& pInstance)
	{
		unsigned long		dwRefCnt = 0;

		if (nullptr != pInstance)
		{
			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = NULL;
		}

		return dwRefCnt;
	}


	// Functor
	class CTag_Finder
	{
	public:
		explicit CTag_Finder(const _tchar* pTag) : m_pTargetTag(pTag){}
		~CTag_Finder(void) {}

	public:
		template<typename T> 
		_bool		operator()(const T& pair)
		{
			if (0 == lstrcmpW(m_pTargetTag, pair.first))
				return true;
			
			return false;
		}

	private:
		const _tchar*		m_pTargetTag = nullptr;
	};

	// Functor
	template <typename T>
	class CTag_FinderEnum
	{
	public:
		explicit CTag_FinderEnum(const T& pTag) : m_eTargetTag(pTag) {}
		~CTag_FinderEnum(void) {}

	public:
		template<typename T>
		_bool		operator()(const T& pair)
		{
			if (m_eTargetTag == pair.first)
				return true;

			return false;
		}

	private:
		T		m_eTargetTag = -1;
	};

	class CDeleteObj
	{
	public:
		explicit CDeleteObj(void) {}
		~CDeleteObj(void) {}
	public: // operator
		template <typename T>
		void operator () (T& pInstance)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = nullptr;
		}
	};

	// 연관컨테이너 삭제용
	class CDeleteMap
	{
	public:
		explicit CDeleteMap(void) {}
		~CDeleteMap(void) {}
	public: // operator	
		template <typename T>
		void operator () (T& Pair)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = Pair.second->Release();

			if (0 == dwRefCnt)
				Pair.second = NULL;
		}
	};

	class CDeleteVector
	{
	public:
		explicit CDeleteVector(void) {}
		~CDeleteVector(void) {}
	public: // operator	
		template <typename T>
		void operator () (T& Pair)
		{
			//if (OBJECTTAG::MONSTER == Pair.first && OBJECTTAG::EFFECT == Pair.first)
			if (OBJECTTAG::MONSTER == Pair.first)
			{
				for (auto& iter : Pair.second)
				{
					EventManager()->DeleteObject(iter);
				}
			}
			else
			{
				for (auto& iter : Pair.second)
				{
					_ulong dwRefCnt = 0;

					dwRefCnt = iter->Release();

					if (0 == dwRefCnt)
						iter = NULL;
				}
			}
		}
	};
}

#endif // Engine_Function_h__
