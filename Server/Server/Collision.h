#pragma once

class Transform;
class Collision
{
public :
	Collision(s_InteractionObejct _owner);
	~Collision();

public:
	void Update(const list<s_InteractionObejct>& _lisInteractin);
	void RecivedDamage();

public:
	int					GetInteractionIndex();
	float				GetCollisionSize() { return m_fCollisionSize; }
	Transform*	GetTransform() { return m_Owner->GetTransform(); }
private:
	bool AlreadDamaged(int _iInteractionIndex);

private:
	s_InteractionObejct m_Owner;

	// interactionIndex를 들고있다
	list<int> m_lisCollision;
	float m_fCollisionClearTime;
	float m_fClearCrtTime;
	float m_fCollisionSize;
};

