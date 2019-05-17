// Thread_creation.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>

void get_random_number(double &val) {
	static const double fraction = 1.0 / (RAND_MAX + 1.0);
	val = std::rand() * fraction;
}

class Accumulator {
public:
	double acc;
	void operator()(const std::vector<double>::iterator begin, const std::vector<double>::iterator end) {
		double accumulator = 0.0;
		for_each(begin, end, [&accumulator](double &a) {
			accumulator += a;
		});
		acc = accumulator;
	}
};

int main(){
	std::vector<double> vec(10000000);

	using clock = std::chrono::steady_clock;
	using ms = std::chrono::milliseconds;

	auto point1 = clock::now();
	for_each(vec.begin(), vec.end(), get_random_number);
    std::cout << "One-Threaded program: " << std::chrono::duration_cast<ms>(clock::now() - point1).count() <<"\n";

	auto point2 = clock::now();
	std::thread t1([&vec] {
		std::for_each(vec.begin(), vec.begin() + vec.size() / 2, get_random_number);
	});
	std::thread t2([&vec] {
		std::for_each(vec.begin() + vec.size() / 2 + 1, vec.end(), get_random_number);
	});
	t1.join();
	t2.join();
	std::cout << "Two-Threaded program: " << std::chrono::duration_cast<ms>(clock::now() - point2).count() << "\n";

	auto point3 = clock::now();
	Accumulator one_th;
	one_th(vec.begin(), vec.end());
	std::cout << "< One-Threaded program: " << std::chrono::duration_cast<ms>(clock::now() - point3).count() 
				<< " > acc = " << one_th.acc << "\n";

	auto point4 = clock::now();
	Accumulator two_th1, two_th2;
	std::thread t3(std::ref(two_th1), vec.begin(), vec.begin() + vec.size() / 2);
	std::thread t4(std::ref(two_th2), vec.begin() + vec.size() / 2 + 1, vec.end());
	t3.join();
	t4.join();
	std::cout << "< Two-Threaded program: " << std::chrono::duration_cast<ms>(clock::now() - point4).count()
		<< " > acc = " << two_th1.acc + two_th2.acc << "\n";

	unsigned int c = std::thread::hardware_concurrency();
	std::cout << " number of cores: " << c << "\n";
	


}

