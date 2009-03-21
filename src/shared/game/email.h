#ifndef EMAIL_H_
#define EMAIL_H_

namespace srdgame
{
enum EmailStatus
{
	ES_NEW,
	ES_UNREAD,
	ES_READ,
	ES_DELETING,
};
struct Email
{
	uint32 _id;
	uint32 _from;
	uint32 _to;
	std::string _title;
	std::string _msg;
	EmailStatus _status;
	time_t _stamp;
};

template <class T>
struct EmailEx : Email
{
	T _ex;
};

class EmailBox
{
public:
	EmailBox(uint32 char_id) : _char_id(char_id)
	{
		_new_emails.resize(_max_email_count);
		_read_emails.resize(_max_email_count);
		_deleting_emails.resize(_max_email_count);
	}
	virtual ~EmailBox()
	{
	}

	virtual load() = 0;

	inline size_t get_unread_count()
	{
		return _new_emails.size();
	}
	inline size_t get_read_count()
	{
		return _read_emails.size();
	}
	inline bool is_full()
	{
		return get_read_count() + get_unread_count < _max_email_count;
	}

protected:
	uint32 _char_id;
	static const _max_email_count;
	std::vector _new_emails;
	std::vector _read_emails;
	std::vector _deleting_emails;
};
}

#endif
