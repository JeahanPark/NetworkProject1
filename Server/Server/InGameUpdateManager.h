#pragma once

class InGameUpdateManager : public BaseManager<InGameUpdateManager>
{

private:
	const double m_dUpdateCycleSecond = 1;

public:
	InGameUpdateManager();
	~InGameUpdateManager();

public:
	void InGameUpdateStart();
	void InGameUpdate();
};

