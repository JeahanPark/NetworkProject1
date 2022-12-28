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

private:
	bool UpdateClientInteraction();
	bool FramCheck();
	void InitFrame();
private:
	LARGE_INTEGER m_CulTime;
	LARGE_INTEGER m_OldTime;
	LARGE_INTEGER m_CpuTick;

	float m_fDeltaTime;
	float m_fSpt;
	float m_fSptTime;

	int m_iFPS;
	float m_fFPSTime;
};

