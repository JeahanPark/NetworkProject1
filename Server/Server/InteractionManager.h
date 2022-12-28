#pragma once
class InteractionObject;
class InteractionManager : public BaseManager<InteractionManager>
{
public:
	~InteractionManager();

public:
	void AddUserInteractionObject(int _userIndex, s_UserController _userController);
	list<InteractionObject*> GetInteractionList() { return m_lisInteraction; }

	void AddDeleteInteraction(list<InteractionObject*>& _InteractionObjects);
	void ClearDeleteInteraction();
	void AllUpdateInteractionObject();

private:
	list<InteractionObject*>		m_lisInteraction;
	list<InteractionObject*>		m_lisDeleteInteraction;
	mutex							m_lockInteraction;
};

