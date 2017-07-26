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
	et_null,
	et_end_build_level_up, //Éý¼¶½áÊø
	et_end_train,			//ÑµÁ·½áÊø
};

template<EventType et, typename EventFunc>
struct EventDef{
	static EventType const event_type = et;
	typedef EventFunc perform_type;
};

typedef EventDef<EventType::et_end_build_level_up, std::function<void(int, std::string)> > Event_Levelup;
typedef EventDef<EventType::et_end_train, std::function<void(int, int, std::string)>> Event_EndTrain;

#endif //FILE_DEF_EVENTTYPE_H_
