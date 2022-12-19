#pragma once

enum class IterationDirection {
	Increase = 1,
	Decrease = 0,
};

class numericIterator {
private:
	int i;
	IterationDirection direction;
public:
	numericIterator(int valueOfi = 0, IterationDirection inc = IterationDirection::Increase)
		: i(valueOfi), direction(inc) {}

	int operator*() {
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
	int a;
	int b;
public:
	range(int from, int to) : a{ from }, b{ to } {}
	numericIterator begin() {
		if (a < b) return numericIterator{ a,IterationDirection::Increase };
		return numericIterator{ a,IterationDirection::Decrease };
	}
	numericIterator end() {
		if (a < b) return numericIterator{ b + 1, IterationDirection::Increase };
		return numericIterator{ b - 1, IterationDirection::Decrease };
	}
};

enum class DIR {
	UP,
	DOWN,
	None
};

class rectangleCicrum {
	bool iterated;
	int d;
	int x;
	int y;
	DIR xDIR;
	DIR yDIR;
	rectangleCicrum(int _x, int _y, int _d) :d(_d), x(_x), y(_y), xDIR(DIR::UP), yDIR(DIR::None), iterated(false) {};
public:
	rectangleCicrum(int _d) :d(_d), x(0), y(_d), xDIR(DIR::UP), yDIR(DIR::None), iterated(false) {};
	rectangleCicrum begin() {
		if (d == 0)
			return rectangleCicrum(0, 0, 1);

		return rectangleCicrum(0, d, d);
	}
	rectangleCicrum end() {
		if (d == 0)
			return rectangleCicrum(1, 0, 1);

		return rectangleCicrum(0, d, d);
	}

	vec2 operator*() {
		return vec2(x, y);
	}

	rectangleCicrum& operator++() {
		iterated = true;
		if (xDIR == DIR::UP) {
			x++;
			if (x >= d)
			{
				x = d;
				xDIR = DIR::None;
				yDIR = DIR::DOWN;
			}
			return *this;
		}

		if (xDIR == DIR::DOWN) {
			x--;
			if (-x >= d)
			{
				x = -d;
				xDIR = DIR::None;
				yDIR = DIR::UP;
			}
			return *this;
		}

		if (yDIR == DIR::UP) {
			y++;
			if (y >= d)
			{
				y = d;
				yDIR = DIR::None;
				xDIR = DIR::UP;
			}
			return *this;
		}

		if (yDIR == DIR::DOWN) {
			y--;
			if (-y >= d)
			{
				y = -d;
				yDIR = DIR::None;
				xDIR = DIR::DOWN;
			}
			return *this;
		}
		return *this;
	}

	bool operator!=(rectangleCicrum& other) {
		if (d == 0)
			return false;
		if (iterated == true && x == other.x && y == other.y)
			return false;
		if (iterated == false && x == other.x && y == other.y)
			return true;
		return !((x == other.x) && (y == other.y));
	}
};

