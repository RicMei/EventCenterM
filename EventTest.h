/*****************************************************************************
* @Copyright 2017, Meishuhao.
* @All rights reserved.
* @brief : test notification center
* @author : alpha.shuhao@gmail.com
* @date : 2017/07/26
* @file : EventTest.h
* @version : ver 1.0
*****************************************************************************/
#ifndef FILE_DEF_EVENTTEST_H_
#define FILE_DEF_EVENTTEST_H_
#include <iostream>
#include "Observer.h"
#include "EventCenter.h"
#include <string>

class DeallocTest : public Observer{
public:
	DeallocTest() = default;
	void OnLevelupx(int uid, std::string name){
		std::cout << "void OnLevelupx uid = " << uid << " name =" << name << std::endl;
	}

	virtual void regist_event_handler() override{
		
	}

	virtual void unregist_event_handler() override{
		
	}

};

class EventTest :public Observer{
public:
	void OnLevelup(int uid, std::string name){
		std::cout << "void OnLevelup uid = " << uid << " name =" << name << std::endl;
	}

	void OnEndTrain(int uid, int tid ,std::string name){
		std::cout << "void OnEndTrain uid = " << uid  << " tid = " << tid << " name = " << name << std::endl;
	}

	void go(){
		// test 
		regist_event_handler();
		
		do{
			DeallocTest de;
			g_event_center.subscribe<event_end_build_level_up>(de, std::bind(&DeallocTest::OnLevelupx, de, std::placeholders::_1, std::placeholders::_2));

			g_event_center.notify_all<event_end_build_level_up>(1, "aaa");
			g_event_center.notify_all<event_end_train>( 1, 2, "aaa");

			// test notify to object
			g_event_center.notify_to<event_end_build_level_up>(de, 5, "eee");

			g_event_center.unsubscribe<event_end_build_level_up>(*this);
			g_event_center.notify_all<event_end_build_level_up>( 2, "bbb");
			g_event_center.notify_all<event_end_train>( 2, 3, "bbb");

			g_event_center.unsubscribe<event_end_train>(*this);
			g_event_center.notify_all<event_end_build_level_up>( 3, "ccc");
			g_event_center.notify_all<event_end_train>( 3, 4, "ccc");



		} while (false);

		g_event_center.notify_all<event_end_build_level_up>( 4, "ddd");
		g_event_center.notify_all<event_end_train>( 4, 5, "ddd");
	}
public:
	virtual void regist_event_handler() override
	{
		g_event_center.subscribe<event_end_build_level_up>(*this, std::bind(&EventTest::OnLevelup, this, std::placeholders::_1, std::placeholders::_2));
		g_event_center.subscribe<event_end_train>(*this, std::bind(&EventTest::OnEndTrain, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}

	virtual void unregist_event_handler() override
	{
		g_event_center.unsubscribe<event_end_build_level_up>(*this);
		g_event_center.unsubscribe<event_end_train>(*this);
	}
};

#endif //FILE_DEF_EVENTTEST_H_
