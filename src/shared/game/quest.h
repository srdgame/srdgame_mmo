#ifndef QUEST_H_
#define QUEST_H_
namespace srdgame
{
enum QuestState
{
	QS_INACTIVE = 0,
	QS_ACTIVE,
};
class Quest
{
public:
	Quest(uint32 id) : _id(id), _state(QS_INACTIVE){}
	virtual Quest(){}

	inline uint32 get_id()
	{
		return _id;
	}
	inline QuestState get_state()
	{
		return _state;
	}
protected:
	uint32 _id;
	uint32 _expire_time;
	QuestState _state;
};

template <class T>
class QuestEx : public Quest
{
public:
	QuestEx(uint32 id, T& t) : Quest(id), _ex(t)
	{
	}
	virtual ~QuestEx()
	{
	}

	inline T* get_ex_data()
	{
		return &_ex;
	}
protected:
	T _ex;
};
}

#endif
