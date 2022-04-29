#include <iostream>
#include <vector>
using namespace std;

class Vector2D {
private:
	int x;
	int y;
	friend class Map;
public:
	Vector2D() { x = y = 0; }
	Vector2D(int i, int j) { x = j; y = i; }
	Vector2D& operator=(const Vector2D& other) {
		x = other.x;
		y = other.y;
		return *this;
	}
	Vector2D& operator+=(const Vector2D& other) {
		x += other.x;
		y += other.y;
		return *this;
	}
	Vector2D& operator-=(const Vector2D& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}
};

class Map {
private:
	static const int W = 20;
	static const int H = 20;
	int A[H][W] = {};
public:
	Map() {
		int i, j;
		for (i = 0; i < H; i++)
			for (j = 0; j < W; j++)
				A[i][j] = 0;
	}
	int& operator()(const Vector2D& v) {
		return(A[v.y][v.x]);
	}
	friend ostream& operator<<(ostream& os, const Map& map) {
		int i, j;
		for (i = 0; i < H; i++) {
			for (j = 0; j < W; j++)
				os << map.A[i][j];
			os << endl;
		}
		return os;
	}
	void setWall(vector<Vector2D> v) {
		for (Vector2D temp : v) {
			A[temp.y][temp.x] = 2;
		}
	}
};
Vector2D Orient[4] = { {1,0},{0,-1},{-1,0},{0,1} };
class Player {
private:
	Map* map;
	Vector2D position;
	Vector2D faceTo;
	int orientation;
	string actions;
public:
	Player() { map = 0; orientation = 0; }
	void setMap(Map* m) {
		map = m;
	}
	void setPosition(Vector2D v) {
		position = v;
		(*map)(position) = 1;
	}
	void setFaceTo(int orient) {
		orientation = orient;
		faceTo = Orient[orient];
	}
	void Forward() {
		if ((*map)(position += faceTo) != 2) {
			(*map)(position) = 1;
		}
		else {
			position -= faceTo;
		}
	}
	void TurnRight() {
		orientation = (orientation + 1) % 4;
		faceTo = Orient[orientation];
	}
	void TurnLeft() {
		orientation = (orientation + 3) % 4;
		faceTo = Orient[orientation];
	}
	void setRule(string s) {
		this->actions = s;
	}
	void play() {
		for (char temp : actions) {
			switch (temp) {
			case 'F':
				Forward();
				break;
			case 'L':
				TurnLeft();
				break;
			case 'R':
				TurnRight();
				break;
			default:
				break;
			}
		}
	}
};
class App {
public:
	void run() {
		Map m;
		m.setWall(vector<Vector2D>({ {1,1},{2,2},{4,4} }));
		Player player;
		player.setMap(&m);
		player.setPosition({ 5,5 });
		player.setFaceTo(0);
		player.setRule("FFFLFFRFFFF");
		player.play();
		cout << m << endl;
	}
};
int main() {
	App app;
	app.run();
}