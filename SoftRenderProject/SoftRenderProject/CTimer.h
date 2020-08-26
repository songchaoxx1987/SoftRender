#pragma once
class CTimer
{
	static CTimer* _pInstance;
	CTimer() 
	{
		deltaTime = 0;
		timePass = 0;
	}
public:
	static CTimer* Instance()
	{
		if (!_pInstance)
			_pInstance = new CTimer();
		return _pInstance;
	}

	float deltaTime;
	double timePass;

};

