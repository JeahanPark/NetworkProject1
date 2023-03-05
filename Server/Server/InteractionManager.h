#pragma once
class InteractionObject;
class InteractionManager : public BaseManager<InteractionManager>
{
public:
	~InteractionManager();

public:
	static s_InteractionObejct CreateUserInteraction(s_UserController _userController, const UserData* _userData);
	static s_InteractionObejct CreateDummyInteraction(const XMFLOAT3& _startPos);

	void AddInteractionObject(s_InteractionObejct _interaction);
	void AddListInteractionObejct(const list<s_InteractionObejct>& _lisInteraction);
	void GetInteractionList(list<s_InteractionObejct>& _InteractionObjects);
 
	void GetDeleteInteraction(list<s_InteractionObejct>& _InteractionObjects);
	void ClearDeleteInteraction();

	void AllUpdateInteractionObject();

private:
	void GetNoLockInteractionList(list<s_InteractionObejct>& _InteractionObjects);
	
private:
	map<eInteractionType, lisInteraction> m_mapInteraction;

	//list<s_InteractionObejct>		m_lisInteraction;
	list<s_InteractionObejct>		m_lisDeleteInteraction;
	mutex							m_lockInteraction;
};

