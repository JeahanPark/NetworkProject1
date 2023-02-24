#pragma once

class Transform;
class Collision
{
private:
	struct collisionInfo
	{
		int m_interactionIndex;
		double m_dRecordTotalDeltaTime;
	};

public :
	Collision(s_InteractionObejct _owner);
	~Collision();

public:
	void Update(const list<s_InteractionObejct>& _lisInteractin);
	void RecivedDamage();

public:
	int					GetInteractionIndex();
	float				GetCollisionSize() { return m_fCollisionSize; }
	Transform*			GetTransform();
	bool				HaveCollisionType(eCollisionType _type);
private:
	bool AlreadDamaged(int _iInteractionIndex);
	void CleanUpCollisionList();

private:
	s_InteractionObejct m_Owner;

	// interactionIndex를 들고있다
	list<collisionInfo> m_lisCollision;
	float m_fCollisionClearTime;
	float m_fCollisionSize;

	int m_CollisionType;
};

