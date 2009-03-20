#ifndef AUCTION_H_
#define AUCTION_H_

namespace srdgame
{
struct AuctionItem
{
	uint32 _id;
	uint32 _seller_id;
	std::string _seller_name;

	uint32 _item_id;
	std::string _item_name;

	uint32 _price;
	time_t _stamp; // 
	uint32 _expire_time;
};
}

#endif
