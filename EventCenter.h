/*****************************************************************************
* @Copyright 2017, Meishuhao
* @All rights reserved.
* @brief : event center patten
* @author : alpha.shuhao@gmail.com
* @date : 2017/07/26
* @file : EventCenter.h
* @version : ver 1.0
*****************************************************************************/
#ifndef FILE_DEF_EVENTCENTER_H_
#define FILE_DEF_EVENTCENTER_H_
#include "EventType.h"
#include "Singleton.h"
#include <stdint.h>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <functional>


class EventCenter : public Singleton<EventCenter> {
public:
	template<typename ObserverWrapperType, typename PerformType = ObserverWrapperType::perform_type>
	void subscribe(Observer & observer, PerformType cb);
	template<typename ObserverWrapperType>
	void unsubscribe(uint64_t _observer_id);
public:
	template<class ObserverWrapperType, typename ...Args>
	void notify(Args ...args);
};

template<class ObserverWrapperType, typename ...Args>
void EventCenter::notify(Args ...args){
	ObserverWrapperContainer<ObserverWrapperType>::instance().perform(args ...);
}

template<typename ObserverWrapperType, typename PerformType /*= ObserverWrapperType::perform_type*/>
void EventCenter::subscribe(Observer & observer , PerformType cb){
	typedef ObserverWrapper<ObserverWrapperType>::DataType _Dt;
	std::shared_ptr< _Dt > wrapper_ptr = std::make_shared<_Dt>(&observer, cb);
	ObserverWrapperContainer<ObserverWrapperType>::instance().add(wrapper_ptr);
}

template<typename ObserverWrapperType>
void EventCenter::unsubscribe(uint64_t _observer_id){
	ObserverWrapperContainer<ObserverWrapperType>::instance().del(_observer_id);
}
#define g_event_center EventCenter::instance()

#endif //FILE_DEF_EVENTCENTER_H_
