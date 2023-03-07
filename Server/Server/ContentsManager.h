#pragma once
class ContentsManager : public BaseManager<ContentsManager>
{
public:
	void InitContents();
	void CreateFireBall(s_ServerSession _session);


private:
	void CreateDummy();
};

