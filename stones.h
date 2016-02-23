#ifndef STONES_H
#define STONES_H

class Point { //точка в 3Д пространстве
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

class Face { //треугольная грань в 3Д пространстве
private:
	float a, b, c; //к-ты уравнения плоскости
	bool abc_def; //заданы ли к-ты
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

class Unit { //Растр
public:
	static float step; //расстояние между соседними растрами
	float x, y, h; //координаты центра
	float vol, deg; //объем и качество растра
	Unit();
	Unit(float, float, float);
	Unit(int, int, int);
};

class Grid { //Набор растров
public:
	float unitStep; //шаг между растрами
	int nx, ny, nh; //измерения
	Unit*** u;
	void build(Surface *);
};

class Deposit { //Месторождение в целом
public:
	/* 
	 * auto faces = surface->faces();
	 * for (auto face : faces) {
	 *   // do anything
	 * }
	 */
	void buildGrid();
	void buildCut(float, float, float); //поперечное сечение вертикальной плоскостью, проходящей через прямую, заданную уравнением
	
	/*
	 * DeluanayTriangulation deluanayTriangulation(&tochki);
	 * Surface *surface = new Surface(deluanayTriangulation.compute());
	 */
	Deposit(/*.. tochlki ...*/);
private:
	float max_x, max_y;
	Surface *sf;
	Grid grid;
};

#endif
