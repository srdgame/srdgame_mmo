#ifndef AUCTION_H_
#define AUCTION_H_

#include <string>
#include <ctime>

namespace srdgame
{
struct AuctionItemInfo
{
	int _id;
	int _seller_id;
	std::string _seller_name;

	int _item_id;
	std::string _item_name;

	int _price;
	time_t _stamp; // 
	int _expire_time;
};
}

#endif
