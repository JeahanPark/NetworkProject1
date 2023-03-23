#pragma once

class InGameUpdateManager : public BaseManager<InGameUpdateManager>
{

private:
	const float m_fFramPerSec = 60;
public:
	InGameUpdateManager();
	~InGameUpdateManager();

public:
	void InGameUpdate();

	// 이거 문제 될것같다
	// Update 매개변수 전달 받는 방식으로 바꿔야할것같다
	float GetDeltaTime() { return m_fRealDeltaTime; }
	double GetTotalDeltaTime() { return m_dTotalDeltaTime; }
private:
	bool UpdateClientInteraction();
	bool FramCheck();
	void InitFrame();
private:
	LARGE_INTEGER m_CulTime;
	LARGE_INTEGER m_OldTime;
	LARGE_INTEGER m_CpuTick;

	float m_fDeltaTime;
	float m_fRealDeltaTime;
	double m_dTotalDeltaTime;
	float m_fSpt;
	float m_fSptTime;

	int m_iFPS;
	float m_fFPSTime;
};

