/*****************************************************************************
* @Copyright 2017, Meishuhao
* @All rights reserved.
* @brief : event type for observer
* @author : alpha.shuhao@gmail.com
* @date : 2017/07/19
* @file : EventType.h
* @version : ver 1.0
*****************************************************************************/
#ifndef FILE_DEF_EVENTTYPE_H_
#define FILE_DEF_EVENTTYPE_H_

#include <functional>

enum class EventType{
	null,
	end_build_level_up, //Éý¼¶½áÊø
	end_train,			//ÑµÁ·½áÊø
};

template<EventType et, typename EventFunc>
struct EventDef{
	static EventType const event_type = et;
	typedef EventFunc perform_type;
};

#define TIE_EVENT_SELECTOR( et , ft ) typedef EventDef<EventType::##et##,std::function<##ft##> > event_##et;


TIE_EVENT_SELECTOR(end_build_level_up, void(int, std::string))
TIE_EVENT_SELECTOR(end_train, void(int, int, std::string))

#endif //FILE_DEF_EVENTTYPE_H_
