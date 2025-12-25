#pragma once
#include <vector>
#include <stdexcept>

template <typename T>
class TStack {
private:
	int top_;
	std::vector<T> data;
public:
	TStack() : top_(-1) {};

	size_t size() const {
		return top_ + 1;
	}

	bool empty() const {
		return top_ == -1;
	}

	void push(const T& val) {
		data.push_back(val);
		++top_;
	}

	T top() const {
		return data[top_];
	}

	void pop() {
		data.pop_back(); //если попытаться вызвать для пустого TStack, то возникнет исключение класса vector
		--top_;
	}
};