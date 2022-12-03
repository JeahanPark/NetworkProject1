#pragma once
class InteractionObject;
class InteractionManager : public BaseManager<InteractionManager>
{
public:
	~InteractionManager();

public:
	void AddUserInteractionObject(s_InGameObject _ingameOBject,
		s_UserController _userController);


private:
	list<InteractionObject*>		m_lisInteraction;
	mutex							m_lockInteraction;
};

