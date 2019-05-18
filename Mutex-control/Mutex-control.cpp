// Mutex-control.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include <mutex>

void func(std::mutex &m) {
	std::lock_guard<std::mutex> guard(m);
	std::thread::id id = std::this_thread::get_id();
	std::cout << "From thread " << id << std::endl;
}

int main(){
	auto loop = [](std::mutex &m) {
		for (int i = 0; i < 10; i++) func(m);
	};

	std::mutex my_mutex;

	std::thread t1(loop, std::ref(my_mutex));
	std::thread t2(loop, std::ref(my_mutex));
	std::thread t3(loop, std::ref(my_mutex));

	t1.join();
	t2.join();
	t3.join();

}

