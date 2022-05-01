#include <iostream>
#include <windows.h>
#include <vector>
using namespace std;
class Vector2D {
private:
	int x; //左右
	int y; //上下
	friend class Map;
public:
	Vector2D() { x = y = 0; }
	Vector2D(int i, int j) { x = j; y = i; } //要注意y是前面那個，x是後面那個，和二維陣列是一樣的
	Vector2D& operator=(const Vector2D& other) { //重載運算子讓這個類別也能直接用=,+=,-=
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
	static const int W = 20; //地圖大小
	static const int H = 20;
	int A[H][W] = {}; //地圖
public:
	Map() { //把地圖內的值都輸入0
		int i, j;
		for (i = 0; i < H; i++)
			for (j = 0; j < W; j++)
				A[i][j] = 0;
	}
	int& operator()(const Vector2D& v) { //重載運算子，用來改地圖內特定位置的值
		return(A[v.y][v.x]);
	}
	friend ostream& operator<<(ostream& os, const Map& map) { //輸出整個地圖
		int i, j;
		for (i = 0; i < H; i++) {
			for (j = 0; j < W; j++)
				os << map.A[i][j];
			os << endl;
		}
		return os;
	}
	void setWall(vector<Vector2D> v) { //設置牆壁
		for (Vector2D temp : v) { //用迴圈跑一遍傳入的vector，將所有位置都設成牆壁
			A[temp.y][temp.x] = 2;
		}
	}
};
Vector2D Orient[4] = { {1,0},{0,-1},{-1,0},{0,1} }; //下右上左
class Player {
private:
	Map* map; //地圖指標
	Vector2D position; //現在位置
	Vector2D faceTo; //面相方向
	int orientation; //面相方向編號
	string actions; //路線
public:
	Player() { map = 0; orientation = 0; }
	void setMap(Map* m) { //設定地圖
		map = m;
	}
	void setPosition(Vector2D v) { //設定位置
		position = v;
		(*map)(position) = 1; //把map的這個位置改成1
	}
	void setFaceTo(int orient) { //設定方向
		orientation = orient; //設定方向編號
		faceTo = Orient[orient]; //根據編號設定面朝方向
	}
	void Forward() { //往前走
		if ((*map)(position += faceTo) != 2) {  //先往前走，然後看該位置是不是牆壁
			(*map)(position) = 1;				//不是的話就把該位置改成1
		}
		else {
			position -= faceTo;					//是牆壁的話就退回去
		}
	}
	void TurnRight() { //右轉
		orientation = (orientation + 1) % 4; //右轉就是編號+1，因為是四個方向所以取4的餘數，這樣就可以循環
		faceTo = Orient[orientation];
	}
	void TurnLeft() {
		orientation = (orientation + 3) % 4; //因為是循環所以左轉就是編號+3
		faceTo = Orient[orientation];
	}
	void setRule(string s) {
		this->actions = s; //設定路線
	}
	void play() {
		string road = ""; //存走過的路
		for (char temp : actions) { //string其實就是char陣列，所以直接用for迴圈跑一遍所有字元，再用switch判斷怎麼走就好
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
			system("cls"); //清空顯示畫面
			cout << *map << endl; //顯示地圖
			road += temp; //紀錄走過的路
			cout << road; //顯示走過的路
			Sleep(500); //等0.5秒
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
	}
};
int main() {
	App app;
	app.run();
}