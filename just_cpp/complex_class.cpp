#include <iostream>
#include <cmath>

//класс числа
class Complex {
	float mod;
	float arg;

public:
	Complex();
	Complex(float re, float im);
	void alg();
	void trig();
	void pokaz();
	~Complex();
	Complex operator+(const Complex&);
	Complex operator-(const Complex&);
	Complex operator*(const Complex&);
	Complex operator/(const Complex&);
};

Complex::Complex() {
	mod = 0;
	arg = 0;
}

Complex::Complex(float m, float a) {
	mod = m;
	arg = a;
}

void Complex::alg() {
	float x = cos(arg) * mod;
	float y = sin(arg) * mod;
	std::cout << x << " + (" << y << ") * i\n";
}

void Complex::trig() {
	std::cout << mod << " * (cos " << arg << " + i * sin " << arg << ")\n";
}

void Complex::pokaz() {
	std::cout << mod << " * " << "e^(i*" << arg << ")\n";
}

Complex::~Complex() {

}

Complex Complex::operator+(const Complex& c) {
	//получаем алгебраическую форму обоих чисел
	float thisx = cos(arg) * mod;
	float thisy = sin(arg) * mod;
	
	float cx = cos(c.arg) * c.mod;
	float cy = sin(c.arg) * c.mod;

	//считаем общее
	float x = thisx + cx;
	float y = thisy + cy;

	//переводим обратно в показательную форму
	float a = atan(y / x);
	float m = sqrt(x * x + y * y);

	Complex complex = Complex(m, a); //возвращаем новый объект
	return complex;
}

Complex Complex::operator-(const Complex& c) {
	float thisx = cos(arg) * mod;
	float thisy = sin(arg) * mod;

	float cx = cos(c.arg) * c.mod;
	float cy = sin(c.arg) * c.mod;

	float x = thisx - cx;
	float y = thisy - cy;

	float a = atan(y / x);
	float m = sqrt(x * x + y * y);

	Complex complex = Complex(m, a);
	return complex;
}

Complex Complex::operator*(const Complex& c) {
	float thisx = cos(arg) * mod;
	float thisy = sin(arg) * mod;

	float cx = cos(c.arg) * c.mod;
	float cy = sin(c.arg) * c.mod;

	float x = thisx * cx - thisy * cy;
	float y = thisy * cx + thisx * cy;

	float a = atan(y / x);
	float m = sqrt(x * x + y * y);

	Complex complex = Complex(m, a);
	return complex;
}

Complex Complex::operator/(const Complex& c) {
	float thisx = cos(arg) * mod;
	float thisy = sin(arg) * mod;

	float cx = cos(c.arg) * c.mod;
	float cy = sin(c.arg) * c.mod;

	float nmrtr1 = thisx * cx + thisy * cy;
	float nmrtr2 = thisy * cx - thisx * cy;

	float dnmrtr = cx * cx + cy * cy;

	float x = nmrtr1 / dnmrtr;
	float y = nmrtr2 / dnmrtr;

	float a = atan(y / x);
	float m = sqrt(x * x + y * y);

	Complex complex = Complex(m, a);
	return complex;
}


int main() {
	using std::cout;
	using std::cin;

	Complex arr[4];
	for (int i = 0; i < 4; i++) {
		cout << "Number 1. \nEnyter modulus(float): ";
		float m;
		cin >> m;
		cout << "Enter argument(float): ";
		float a;
		cin >> a;
		arr[i] = Complex(m, a);
	}
	cout << std::endl;
	
	cout << "Your numbers in 3 forms:\n";
	for (int i = 0; i < 4; i++) {
		cout << "a" << i + 1 << ":\n";
		arr[i].alg();
		arr[i].trig();
		arr[i].pokaz();
	}
	cout << std::endl;
	
	Complex result = arr[1] / arr[0] + arr[2] * arr[3];
	cout << "Result in 3 forms:\n";
	result.alg();
	result.trig();
	result.pokaz();
}