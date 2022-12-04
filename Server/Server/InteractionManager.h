#pragma once
class InteractionObject;
class InteractionManager : public BaseManager<InteractionManager>
{
public:
	~InteractionManager();

public:
	void AddUserInteractionObject(int _userIndex, s_UserController _userController);

	void AllUpdateInteractionObject();

	const list<InteractionObject*>& GetlisInteractionObject() { return m_lisInteraction; }

private:
	list<InteractionObject*>		m_lisInteraction;
	mutex							m_lockInteraction;
};

