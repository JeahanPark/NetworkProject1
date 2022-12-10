#pragma once
class InteractionObject;
class InteractionManager : public BaseManager<InteractionManager>
{
public:
	~InteractionManager();

public:
	void AddUserInteractionObject(int _userIndex, s_UserController _userController);

	list<InteractionObject*>AllUpdateInteractionObject();

private:
	list<InteractionObject*>		m_lisInteraction;
	mutex							m_lockInteraction;
};

