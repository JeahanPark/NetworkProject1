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
	list<s_InteractionObejct> GetInteractionList() { return m_lisInteraction; }
	void GetUserInteractionList(list<s_InteractionObejct>& _lisUserInteracction);
 
	void AddDeleteInteraction(list<s_InteractionObejct>& _InteractionObjects);
	void ClearDeleteInteraction();

	void AllUpdateInteractionObject();

private:
	list<s_InteractionObejct>		m_lisInteraction;
	list<s_InteractionObejct>		m_lisDeleteInteraction;
	mutex							m_lockInteraction;
};

