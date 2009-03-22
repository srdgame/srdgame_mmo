#ifndef EMAIL_H_
#define EMAIL_H_

#include <string>
#include <vector>
#include <ctime>

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
	Email() : _id(0), _from(0), _to(0), _status(ES_NEW), _stamp(0)
	{
	}
	int _id;
	int _from;
	int _to;
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
	EmailBox(int char_id) : _char_id(char_id)
	{
		_new_emails.resize(_max_email_count);
		_read_emails.resize(_max_email_count);
		_deleting_emails.resize(_max_email_count);
	}
	virtual ~EmailBox()
	{
	}

	virtual bool load() = 0;

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
		return get_read_count() + get_unread_count() < _max_email_count;
	}

protected:
	int _char_id;
	static const int _max_email_count = 10;
	std::vector<Email> _new_emails;
	std::vector<Email> _read_emails;
	std::vector<Email> _deleting_emails;
};
}

#endif
