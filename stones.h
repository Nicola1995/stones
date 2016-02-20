#ifndef STONES_H
#define STONES_H

class Point { //����� � 3� ������������
public:
	float x, y, h;
	Point (float x = 0, float y = 0, float h = 0);
	
	Point operator - (const Point &p) const;
	Point operator + (const Point &p) const;
	bool operator < (const Point &p) const;
	
	float length() const;
	float scalar(const Point &p) const;
	float distance(const Point &p) const;
	Point cross(const Point &p) const;
	Point normalize() const;
};

class Face { //����������� ����� � 3� ������������
private:
	float a, b, c; //�-�� ��������� ���������
	bool abc_def; //������ �� �-��
	void defABC();
public:
	Point * p[3];
	Face ();
	Face (const Triangle &tr);
	float heightAt(float, float);
	void initUnits();
};

class Surface {
protected:
	std::vector<Face> m_faces;
	
public:
	Surface(const std::vector<Trinangle> &triangles);
	const std::vector<Face> &faces() const;
};

class Unit { //�����
public:
	static float step; //���������� ����� ��������� ��������
	float x, y, h; //���������� ������
	float vol, deg; //����� � �������� ������
	Unit();
	Unit(float, float, float);
	Unit(int, int, int);
};

class Grid { //����� �������
public:
	float unitStep; //��� ����� ��������
	int nx, ny, nh; //���������
	Unit*** u;
};

class Deposit { //������������� � �����
public:
	void buildSurface();
	void buildGrid();
	void buildCut(float, float, float); //���������� ������� ������������ ����������, ���������� ����� ������, �������� ����������
	Deposit();
private:
	float max_x, max_y;
	Surface *sf;
	Grid grid;
};

#endif