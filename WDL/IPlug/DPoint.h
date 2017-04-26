#pragma once

#include <algorithm>

struct Complex {
	Complex() : R(0), I(1) {}
	Complex(double ang) :  R(sin(ang)), I(cos(ang)){}
	Complex(double ang, double scale) : R(sin(ang) * scale), I(cos(ang) * scale) {}
	double R;
	double I;
};

struct DPoint {
	DPoint(double X = 0, double Y = 0) : X(X), Y(Y) {}
	double X;
	double Y;

	const DPoint& translateMe(double dX, double dY) {
		X += dX; Y += dY;
		return *this;
	}
	const DPoint& translateMe(DPoint d) {
		X += d.X; Y += d.Y;
		return *this;
	}

	const DPoint& scaleMe(double scl) {
		X *= scl; Y *= scl;
		return *this;
	}
	const DPoint& rotateMe(double ang) {
		rotateMe(sin(ang), cos(ang));
		return *this;
	}
	const DPoint& rotateMe(double ang, const DPoint& pivot) {
		X -= pivot.X;
		Y -= pivot.Y;
		rotateMe(sin(ang), cos(ang));
		X += pivot.X;
		Y += pivot.Y;
		return *this;
	}
	const DPoint& rotateMe(Complex c) {
		rotateMe(c.R, c.I);
		return *this;
	}
	const DPoint& rotateMe(Complex c, const DPoint& pivot) {
		X -= pivot.X;
		Y -= pivot.Y;
		rotateMe(c.R, c.I);
		X += pivot.X;
		Y += pivot.Y;
		return *this;
	}
	DPoint& rotateMe(double sin, double cos) {
		double xTmp = (X * cos) - (Y * sin);
		Y = (X * sin) - (Y * cos);
		X = xTmp;
		return *this;
	}

	DPoint translate(double dX, double dY) {
		return DPoint(X + dX, Y + dY);
	}
	DPoint scale(double scl) {
		return DPoint(X * scl, Y * scl);
	}
	DPoint rotate(double ang) {
		return DPoint(*this).rotateMe(sin(ang), cos(ang));
	}
	DPoint rotate(double ang, const DPoint& pivot) {
		DPoint pt(X - pivot.X, Y - pivot.Y);
		return pt.rotateMe(sin(ang), cos(ang)).translateMe(pivot);
	}
	DPoint rotate(Complex c) {
		return DPoint(*this).rotateMe(c.R, c.I);
	}
	DPoint rotate(Complex c, const DPoint& pivot) {
		DPoint pt(X - pivot.X, Y - pivot.Y);
		return pt.rotateMe(c.R, c.I).translateMe(pivot);
	}
};




