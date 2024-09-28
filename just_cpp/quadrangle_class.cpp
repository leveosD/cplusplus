#include <iostream>
#include <map>
#include <cmath>


//класс четырехугольника
class Quadrangle {
protected:
	std::pair<int, int> points[4]; //саммив пар чисел для записи вершин
	////функция для удобства вычислений
	float getSide(std::pair<int, int> a, std::pair<int, int> b); //возвращает длину стороны фигуры

public:
	Quadrangle();
	bool paralSides(std::pair<int, int> p1, std::pair<int, int> p2); //проверка на параллельность
	void showLen();
	void showDiag();
	float showP();
	float showS();
};

//класс трапеции
class Trapeze : Quadrangle {
public:
	bool isTrapeze();
	float avrgLine();
};

Quadrangle::Quadrangle() {
	using std::cout;
	using std::cin;

	cout << "Constructor.\n";
	for (int i = 0; i < 4; i++) {
		cout << "Point " << i << std::endl <<
			"Enter x: ";
		int x;
		cin >> x;
		cout << "Enter y: ";
		int y;
		cin >> y;
		points[i] = std::make_pair(x, y);
		cout << "\n";
	}
}


float Quadrangle::getSide(std::pair<int, int> a, std::pair<int, int> b) {
	std::pair<int, int> vect = std::make_pair(b.first - a.first, b.second - a.second);
	return float(sqrt(vect.first * vect.first + vect.second * vect.second));
}

bool Quadrangle::paralSides(std::pair<int, int> side1, std::pair<int, int> side2) {
	float k1, k2; //отношения x-ов и y-ов двух сторон
	if (side1.first == 0 || side2.first == 0) //исключаем деление на ноль
		k1 = 0;
	else
		k1 = abs(float(side1.first / side2.first));

	if (side1.second == 0 || side2.second == 0)
		k2 = 0;
	else
		k2 = abs(float(side1.second / side2.second));
	if (k1 == k2) //ксли отношения равны, значит стороны параллельны
		return true;
	return false;
}

void Quadrangle::showLen() {
	std::cout << "Sides: ";
	float* sides = new float[4];
	for (int i = 1; i < 4; i++) {
		sides[i] = getSide(points[i], points[i - 1]);
		std::cout << sides[i] << " ";
	}
	sides[3] = getSide(points[3], points[0]);
	std::cout << sides[3] << "\n";
}

void Quadrangle::showDiag() {
	std::cout << "Diagonals: ";
	std::pair<int, int> vect;
	for (int i = 0; i < 2; i++) {
		std::cout << getSide(points[i], points[i + 2]) << " ";
	}
	std::cout << "\n";
}

float Quadrangle::showP() {
	float p = 0;
	for (int i = 0; i < 3; i++) {
		p += getSide(points[i], points[i + 1]);
	}
	p += getSide(points[3], points[0]);
	std::cout << "Perimeter: " << p << "\n";
	return p;
}

float Quadrangle::showS() {
	int s;
	float p = this->showP();
	float* sides = new float[4];
	
	float d1 = getSide(points[0], points[2]);
	float d2 = getSide(points[1], points[3]);

	std::pair<int, int> vect1 = std::make_pair(points[2].first - points[0].first, points[2].second - points[0].second);
	std::pair<int, int> vect2 = std::make_pair(points[3].first - points[1].first, points[3].second - points[1].second);
	float vectMultpl = vect1.first * vect2.first + vect1.second * vect2.second;

	float cosD = vectMultpl / (d1 * d2);
	float sinD = sin(acos(cosD));

	s = 0.5f * d1 * d2 * sinD;
	
	std::cout << "Square: " << s << "\n";
	return s;
}

bool Trapeze::isTrapeze() {
	int paral = 0;
	std::pair<int, int> vect[4];
	for (int i = 0; i < 3; i++) {
		vect[i] = std::make_pair(points[i].first - points[i + 1].first, points[i].second - points[i + 1].second);
	}
	vect[3] = std::make_pair(points[3].first - points[0].first, points[3].second - points[0].second);
	
	//узнаем, сколько пар параллельных сторон у фигуры
	if (paralSides(vect[0], vect[2]))
		paral++;
	if (paralSides(vect[1], vect[3]))
		paral++;
	if (paral == 1) //если пара одна, то это трапеция
		return true;
	else
		return false;
}

float Trapeze::avrgLine() {
	if (!this->isTrapeze()) { //проверяем тропеция ли наша фигура
		std::cout << "it's not a trapeze";
		return 0;
	}

	std::pair<int, int> vect[4];
	for (int i = 0; i < 3; i++) {
		vect[i] = std::make_pair(points[i].first - points[i + 1].first, points[i].second - points[i + 1].second);
	}
	vect[3] = std::make_pair(points[3].first - points[0].first, points[3].second - points[0].second);

	float result = 0;
	for (int i = 0; i < 2; i++) {
		if (paralSides(vect[i], vect[i + 2])) {
			float side1 = sqrt(vect[i].first * vect[i].first + vect[i].second * vect[i].second);
			float side2 = sqrt(vect[i + 2].first * vect[i + 2].first + vect[i + 2].second * vect[i + 2].second);
			result = (side1 + side2) / 2;
			break;
		}
	}
	return result;
}

int main() {
	using namespace std;

	int n, m;
	cout << "Amount of quadrangle: ";
	cin >> n;
	cout << "Amount of trapezes: ";
	cin >> m;

	Quadrangle* qdr = new Quadrangle[n];
	Trapeze* trpz = new Trapeze[m];

	float minS = -1;
	for (int i = 0; i < n; i++) {
		cout << "Quadrangle " << i << endl;
		float s = qdr[i].showS();
		if (minS > s || minS < 0)
			minS = s;
		cout << endl;
	}
	cout << "Min square: " << minS << endl;

	float maxAL = 0;
	for (int i = 0; i < m; i++) {
		if (trpz[i].isTrapeze()) {
			float al = trpz[i].avrgLine();
			if (maxAL < al)
				maxAL = al;
		}
	}
	cout << "Max average line: " << maxAL;
}