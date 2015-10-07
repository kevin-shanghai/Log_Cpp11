// Log.cpp : 定义控制台应用程序的入口点。
//
#include <iostream>
#include <type_traits>
#include <thread>
#include "Log.h"
#include "LogStream.h"
#include <ctime>
#include <chrono>
#include <functional>
#include <sstream>	   
#include <memory>
#include "SyncLogging.h"
#include "AsyncLogging.h"
#include <fstream>
#include "FileOperation.h"
#include "Buffer.h"
#include "SyncQueue.h"
#include <sstream>
#include <iomanip>



std::shared_ptr<Logger::SyncLogging> g_logFile = std::make_shared<Logger::SyncLogging>();
std::shared_ptr<Logger::AsyncLogging> asyncLogStrategy = std::make_shared<Logger::AsyncLogging>();

void Test()
{
	while (1)
	{
		static int64_t iCount = 0;
		std::string temp = "iCount:" + std::to_string(iCount);
		LOG_INFO << temp;
		if ((iCount++) < 1000)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
}

void Test2()
{
	std::ofstream testWriteFileStream("TestWriteFileSpeed.txt");
	
	while (1)
	{
		testWriteFileStream << "12345678910,abcdefgh" << std::endl;
		std::this_thread::sleep_for(std::chrono::microseconds(5));
	}
	

}



void output_func(const char* msg, uint64_t len)
{
	//g_logFile->Append(msg, len);
	asyncLogStrategy->Append(msg, len);
}

void flush_func()
{
	//g_logFile->Flush();
}



int main()
{
	std::cout << "Enter main...." << std::endl;
	Logger::Log::SetOutputFuncCallback(std::bind(output_func, std::placeholders::_1, std::placeholders::_2));;
	Logger::Log::SetFlushFuncCallback(std::bind(flush_func));
	
	LOG_INFO << "kevin";
	int*p = nullptr;
	LOG_INFO << p;
	LOG_INFO << "first line.....";
	LOG_INFO << "second line....";
	LOG_INFO << __TIME__;
	bool b = true;
	LOG_INFO << b;
	auto aa = 8888;
	LOG_INFO << aa;
	
	// LOG_INFO << "Before exception....";
	// *p = 43;
	// LOG_INFO << "after exception....";

	std::thread tt([](){ Test(); });
	tt.join();

	std::this_thread::sleep_for(std::chrono::seconds(1000));
//	system("pause");
	return 0;
}

