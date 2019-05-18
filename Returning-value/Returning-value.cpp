// Returning-value.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <future>

double accumulate(std::vector<double>::iterator begin, std::vector<double>::iterator end) {
	double accumulator = 0.0;
	for_each(begin, end, [&accumulator](double &a) {
		accumulator += a;
	});
	return accumulator;
}

void get_random_number(double &val) {
	static const double fraction = 1.0 / (RAND_MAX + 1.0);
	val = std::rand() * fraction;
}

int main(){
	std::vector<double> vec(10000000);

	/* Generate random numbers in vector (2 threads faster) */
	std::thread t1([&vec] {
		std::for_each(vec.begin(), vec.begin() + vec.size() / 2, get_random_number);
	});
	std::thread t2([&vec] {
		std::for_each(vec.begin() + vec.size() / 2 + 1, vec.end(), get_random_number);
	});
	t1.join();
	t2.join();

	using clock = std::chrono::steady_clock;
	using ms = std::chrono::milliseconds;

	/* Accumulate vector in one thread */
	auto point1 = clock::now();
	double acc = accumulate(vec.begin(), vec.end());
	std::cout << "<One-Threaded program: " << std::chrono::duration_cast<ms>(clock::now() - point1).count()
		<< " > acc = " << acc << "\n";

	/* Accumulate vector in two threads */
	auto point2 = clock::now();
	std::future<double> t3 = std::async(accumulate, vec.begin(), vec.begin() + vec.size() / 2);
	auto t4 = std::async(accumulate, vec.begin() + vec.size() / 2 + 1, vec.end());
	acc = t3.get() + t4.get();
	std::cout << "<Two-Threaded program: " << std::chrono::duration_cast<ms>(clock::now() - point2).count()
		<< " > acc = " << acc << "\n";






}

