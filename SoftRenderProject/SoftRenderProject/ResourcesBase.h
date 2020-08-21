#pragma once
class ResourcesBase
{
public:
	int m_refCount = 0;

	int IncreaseRef() 
	{
		return ++m_refCount;
	}

	int DecreaseRef()
	{
		return --m_refCount;		
	}
};