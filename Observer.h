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
class ObserverWrapperBase;

template<typename ObserverWrapperType>
class ObserverWrapper;

class ObserverWrapperBase{
public:
	explicit ObserverWrapperBase(EventType _type) :type_(_type){}
	virtual ~ObserverWrapperBase(){};
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

	template <typename ObserverWrapperType,typename ...Args>
	void perform(Args ... args){
		typedef ObserverWrapper<ObserverWrapperType> DataType;
		for (auto itr : vector_){
			if (itr->etype() == ObserverWrapperType::event_type){
				if (auto casted_ptr = std::dynamic_pointer_cast<DataType>((itr))){
					casted_ptr->pr()(args...);
				}
			}
		}
	}

	inline void add_wrapper(std::shared_ptr<ObserverWrapperBase>  _wrapper_ptr){
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
			for (auto itr = observer_all_.begin(); itr != observer_all_.end(); ){
				if (auto observer_wrapper_ptr = itr->lock()){
					observer_wrapper_ptr->pr()(args...);
					itr++;
				}
				else{
					itr = observer_all_.erase(itr);
				}
			}
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
				_d->get()->add_wrapper(_d);
			}
		}
	}

private:
	std::vector< std::weak_ptr<DataType >> observer_all_;
};

#endif //FILE_DEF_OBSERVER_H_