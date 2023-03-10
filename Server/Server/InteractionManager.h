#pragma once
class InteractionObject;
class UserObject;
class InteractionManager : public BaseManager<InteractionManager>
{
public:
	~InteractionManager();

public:

	void AddInteractionObject(s_InteractionObejct _interaction);
	void AddListInteractionObejct(const list<s_InteractionObejct>& _lisInteraction);
	void GetInteractionList(list<s_InteractionObejct>& _InteractionObjects);
	void GetInteractionTypeList(eInteractionType _type, lisInteraction& _lisInteraction);

	void GetDeleteInteraction(list<s_InteractionObejct>& _InteractionObjects);
	void ClearDeleteInteraction();

	void AllUpdateInteractionObject();

	UserObject* GetUser(int _iUserIndex);

private:
	void GetNoLockInteractionList(list<s_InteractionObejct>& _InteractionObjects);
	void GetNoLockInteractionTypeList(eInteractionType _type, lisInteraction& _lisInteraction);
private:
	map<eInteractionType, lisInteraction> m_mapInteraction;

	//list<s_InteractionObejct>		m_lisInteraction;
	list<s_InteractionObejct>		m_lisDeleteInteraction;
	mutex							m_lockInteraction;
};

