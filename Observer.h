/*****************************************************************************
* @Copyright 2017, Meishuhao
* @All rights reserved.
* @brief : observer def
* @author : alpha.shuhao@gmail.com
* @date : 2017/07/26
* @file : Observer.h
* @version : ver 1.0
*****************************************************************************/
#ifndef FILE_DEF_OBSERVER_H_
#define FILE_DEF_OBSERVER_H_
#include <stdint.h>
#include <memory>
#include <vector>
#include "EventType.h"
#include <algorithm>
#include "Singleton.h"

class ObserverWrapperBase{
public:
	explicit ObserverWrapperBase(EventType _type) :type_(_type){}
	inline EventType etype() const { return type_; }
private:
	EventType type_;
};

class Observer {
public:
	Observer(){
		static uint64_t static_observer = 0;
		id_ = ++static_observer;
	}
	virtual ~Observer(){
	}
public:
	virtual void regist_event_handler() = 0;
	virtual void unregist_event_handler() = 0;
public:
	inline uint64_t id(){ return id_; }

	inline void add_wraper(std::shared_ptr<ObserverWrapperBase>  _wrapper_ptr){
		if (std::find_if(vector_.begin(), vector_.end(), [&_wrapper_ptr](std::shared_ptr<ObserverWrapperBase> wrapper_ptr){
			return wrapper_ptr->etype() == _wrapper_ptr->etype();
		}) == vector_.end()){
			vector_.emplace_back(_wrapper_ptr);
		}
	}
	inline void del_wrapper(EventType _etype){
		auto find_itr = vector_.erase(std::find_if(vector_.begin(), vector_.end(), [_etype](std::shared_ptr<ObserverWrapperBase> wrapper_ptr){
			return wrapper_ptr->etype() == _etype;
		}));
	}
private:
	uint64_t id_ = 0;
	std::vector<std::shared_ptr<ObserverWrapperBase>> vector_;
};

template<typename ObserverWrapperType>
class ObserverWrapper : public ObserverWrapperBase{
public:
	typedef ObserverWrapper DataType;
public:
	typedef typename ObserverWrapperType::perform_type PerformType;
	ObserverWrapper(Observer * _observer, typename PerformType _pr) :obj_(_observer), pr_(_pr), ObserverWrapperBase(ObserverWrapperType::event_type){
	}
	~ObserverWrapper(){
		obj_ = nullptr;
	}
	inline uint64_t id(){
		if (obj_)
			return obj_->id();
		return 0;
	}
public:
	inline Observer * get()const{
		return obj_;
	}
	inline PerformType pr(){
		return pr_; 
	}
private:
	Observer * obj_ = nullptr;
	PerformType pr_ = nullptr;
};


template<typename ObserverWrapperType>
class ObserverWrapperContainer : public Singleton<ObserverWrapperContainer<ObserverWrapperType>> {
public:
	typedef ObserverWrapper<ObserverWrapperType> DataType;
public:
	template<typename ...Args>
	void perform(Args ...args){
		std::vector<std::weak_ptr< DataType>> copy_container(observer_all_.begin(), observer_all_.end());
			for (auto itr = copy_container.begin(); itr != copy_container.end(); itr++){
				if (auto observer_wrapper_ptr = itr->lock()){
					observer_wrapper_ptr->pr()(args...);
				}
			}
			//remove all expired
			observer_all_.erase(std::remove_if(observer_all_.begin(), observer_all_.end(), [](const std::weak_ptr< DataType> wrap_wptr){
				return !wrap_wptr.lock();
			}), observer_all_.end());
	}
	void add( std::shared_ptr<DataType> _d ){
		if (_d->get()){
			if (std::find_if(observer_all_.begin(), observer_all_.end(), [&_d](std::weak_ptr<DataType> _dp) {
				if (auto dp = _dp.lock()){
					return _d->id() == dp->id();
				}
				return  false;
			}) == observer_all_.end()){
				observer_all_.emplace_back(_d);
				_d->get()->add_wraper(_d);
			}
		}
	}

	void del(uint64_t _id){
		for (auto itr = observer_all_.begin(); itr != observer_all_.end();){
			auto dp = itr->lock();
			if (dp && dp->id() != _id){
				itr++;
				continue;
			}
			if (dp){
				if (auto observer_ptr = dp->get()){
					observer_ptr->del_wrapper( dp->etype() );
				}
			}
			itr = observer_all_.erase(itr);
		}
	}
private:
	std::vector< std::weak_ptr<DataType >> observer_all_;
};

#endif //FILE_DEF_OBSERVER_H_