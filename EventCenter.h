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
#include "Observer.h"

class EventCenter : public Singleton<EventCenter> {
public:
	template<typename ObserverWrapperType, typename PerformType = ObserverWrapperType::perform_type>
	void subscribe(Observer & observer, PerformType cb){
		typedef ObserverWrapper<ObserverWrapperType>::DataType _Dt;
		std::shared_ptr< _Dt > wrapper_ptr = std::make_shared<_Dt>(&observer, cb);
		ObserverWrapperContainer<ObserverWrapperType>::instance().add(wrapper_ptr);
	}
	template<typename ObserverWrapperType>
	void unsubscribe(Observer& observer){
		observer.del_wrapper(ObserverWrapperType::event_type);
	}
public:
	template<class ObserverWrapperType, typename ...Args>
	void notify_to( Observer & target ,Args ...args){
		target.perform<ObserverWrapperType>(args...);			
	}

	template<class ObserverWrapperType, typename ...Args>
	void notify_all(Args ...args){
			ObserverWrapperContainer<ObserverWrapperType>::instance().perform(args ...);
	}
};

#define g_event_center EventCenter::instance()

#endif //FILE_DEF_EVENTCENTER_H_
