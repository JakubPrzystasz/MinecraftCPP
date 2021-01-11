#pragma once

enum class IterationDirection {
	Increase = 1,
	Decrease = 0,
};

class numericIterator {
private:
	long long i;
	IterationDirection direction;
public:
	numericIterator(long long valueOfi = 0, IterationDirection inc = IterationDirection::Increase)
		: i(valueOfi), direction(inc) {}

	long long operator*() { 
		return i;
	}

	numericIterator& operator++() {
		if (static_cast<bool>(direction)) ++i; else --i;
		return *this;
	}

	bool operator!=(numericIterator& other){
		return i != other.i;
	}
};


class range {
	long long a;
	long long b;
public:
	range(long long from, long long to) : a{ from }, b{ to } {}
	numericIterator begin() {
		if (a < b) return numericIterator{ a,IterationDirection::Increase };
		return numericIterator{ a,IterationDirection::Decrease };
	}
	numericIterator end() {
		if (a < b) return numericIterator{ b + 1, IterationDirection::Increase };
		return numericIterator{ b - 1, IterationDirection::Decrease };
	}
};

