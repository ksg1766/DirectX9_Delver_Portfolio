CBase::CBase(void) : m_dwRefCnt(0)
{

}
CBase::~CBase(void)
{

}

unsigned long CBase::AddRef()
{
	return ++m_dwRefCnt;
}
unsigned long CBase::Release()
{
	if (0 == m_dwRefCnt)
	{
		Free();

		delete this;

		return 0;
	}

	return m_dwRefCnt--;
}
