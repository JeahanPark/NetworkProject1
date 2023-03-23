#pragma once
class ContentsManager : public BaseManager<ContentsManager>
{
public:
	ContentsManager();
public:
	void InitContents();
	void UpdateFieldItem();

	void UpdateReflectionMaxCount(int _iCount);
	void UpdateReflectionCrtCount(int _iCount);
private:
	void CreateDummy();
	void CreateReflectionItem();
private:

	// ReflectionItem ���� ����
	atomic<int> m_iReflectionItemMaxCount;
	atomic<int> m_iReflectionItemCrtCount;
	const float m_fReflectionCreateMaxTime = 3;
	float m_fReflectionCreateCrtTime;
};

