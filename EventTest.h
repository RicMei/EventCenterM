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
			g_event_center.subscribe<Event_Levelup>(de, std::bind(&DeallocTest::OnLevelupx, de, std::placeholders::_1, std::placeholders::_2));

			g_event_center.notify<Event_Levelup>(1, "aaa");
			g_event_center.notify<Event_EndTrain>(1, 2, "aaa");
			g_event_center.unsubscribe<Event_Levelup>(Observer::id());
			g_event_center.notify<Event_Levelup>(2, "bbb");
			g_event_center.notify<Event_EndTrain>(2, 3, "bbb");
			g_event_center.unsubscribe<Event_EndTrain>(Observer::id());
			g_event_center.notify<Event_Levelup>(3, "ccc");
			g_event_center.notify<Event_EndTrain>(3, 4, "ccc");
		} while (false);

		g_event_center.notify<Event_Levelup>(4, "ddd");
		g_event_center.notify<Event_EndTrain>(4, 5, "ddd");
	}
public:
	virtual void regist_event_handler() override
	{
		g_event_center.subscribe<Event_Levelup>(*this,std::bind(&EventTest::OnLevelup, this, std::placeholders::_1, std::placeholders::_2));
		g_event_center.subscribe<Event_EndTrain>(*this, std::bind(&EventTest::OnEndTrain, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}

	virtual void unregist_event_handler() override
	{
		g_event_center.unsubscribe<Event_Levelup>(Observer::id());
		g_event_center.unsubscribe<Event_EndTrain>(Observer::id());
	}
};

#endif //FILE_DEF_EVENTTEST_H_
