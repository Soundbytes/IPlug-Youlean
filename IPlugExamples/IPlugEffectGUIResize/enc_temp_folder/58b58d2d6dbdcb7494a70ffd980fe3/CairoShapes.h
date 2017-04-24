#pragma once

#include "ycairo.h"
#include <algorithm>


struct Complex {
	Complex() : R(0), I(1) {}
	Complex(double ang) :  R(sin(ang)), I(cos(ang)){}
	Complex(double ang, double scale) : R(sin(ang) * scale), I(cos(ang) * scale) {}
	double R;
	double I;
};

struct Point {
	Point(double X = 0, double Y = 0) : X(X), Y(Y) {}
	double X;
	double Y;

	const Point& translate(double dX, double dY) {
		X += dX; Y += dY;
		return *this;
	}
	const Point& scale(double scl) {
		X *= scl; Y *= scl;
		return *this;
	}
	const Point& rotate(double ang) {
		rotateSinCos(sin(ang), cos(ang));
		return *this;
	}
	const Point& rotate(double ang, const Point& pivot) {
		X -= pivot.X;
		Y -= pivot.Y;
		rotateSinCos(sin(ang), cos(ang));
		X += pivot.X;
		Y += pivot.Y;
		return *this;
	}
	const Point& rotaScale(Complex c) {
		rotateSinCos(c.R, c.I);
		return *this;
	}
	const Point& rotaScale(Complex c, const Point& pivot) {
		X -= pivot.X;
		Y -= pivot.Y;
		rotateSinCos(c.R, c.I);
		X += pivot.X;
		Y += pivot.Y;
		return *this;
	}

private:
	void rotateSinCos(double s, double c) {
		double xTmp = (X * c) - (Y * s);
		Y = (X * s) - (Y * c);
		X = xTmp;
	}
};

const IColor colHot(0xFA3A13);
const IColor colCold(0x0998B9);
const IColor colKnob(0xfdac5c);
const IColor colBlack(0x000000);
//const IColor colBotm(0x06697C, 64);
const IColor colBotm(0x000000, 36);

inline void cai_set_source_icol(cairo_t *cr, IColor c) {
	const double scale = 1. / 255.;
	cairo_set_source_rgba(cr, c.R * scale, c.G * scale, c.B * scale, c.A * scale);
}

class SbKnob0 : public IKnobControl, ycairo_gui
{
public:
	SbKnob0(IPlugBase* pPlug, ycairo_base *ycairo_base, IRECT pR, int paramIdx = -1,
		EDirection direction = kVertical, double gearing = DEFAULT_GEARING)
		: IKnobControl(pPlug, pR, paramIdx, direction, /*gearing*/ 1), ycairo_gui(ycairo_base, this)
	{
		mRadius = 0.5 * std::min(pR.W(), pR.H());

		mBlend = IChannelBlend(IChannelBlend::kBlendClobber);
		AfterGUIResize(1.0);
	}

	bool Draw(IGraphics* pGraphics) {
		double ang = MIN_ANGLE + mValue * KNOB_RANGE;

		ycairo_prepare_draw();
		cairo_translate(cr, mDrawRECT.MW(), mDrawRECT.MH());

		Point pO(scaledOuterRadius, 0);
		Point pI(scaledInnerRadius, 0);

		cairo_arc(cr, 0, 0, scaledInnerRadius, 0, 2 * PI);
		cai_set_source_icol(cr, colKnob);
		cairo_fill_preserve(cr);

		cai_set_source_icol(cr, colBlack);
		cairo_set_line_width(cr, LINE_WIDTH);
		cairo_stroke(cr);


		cairo_arc(cr, 0, 0, scaledOuterRadius, ang, MIN_ANGLE + KNOB_RANGE);
		cai_set_source_icol(cr, colCold);
		cairo_stroke(cr);

		cairo_arc(cr, 0, 0, scaledOuterRadius, MIN_ANGLE, ang);

		Complex siCo(ang);
		pI.rotaScale(siCo);
		cairo_line_to(cr, pI.X, pI.Y);
		cai_set_source_icol(cr, colHot);
		cairo_stroke(cr);

		cairo_move_to(cr, pI.X, pI.Y);
		cairo_line_to(cr, 0, 0);
		cai_set_source_icol(cr, colBlack);
		cairo_stroke(cr);

		cai_set_source_icol(cr, colBotm);
		cairo_arc(cr, 0, 0, scaledOuterRadius, MIN_ANGLE + KNOB_RANGE, MIN_ANGLE);
		cairo_stroke(cr);

		cairo_identity_matrix(cr);

		ycairo_draw();
		return true;
	}

	void AfterGUIResize(double guiScaleRatio) {
		mGUIScaleRatio = guiScaleRatio;
		scaledOuterRadius = mRadius * mGUIScaleRatio - LINE_WIDTH;
		scaledInnerRadius = scaledOuterRadius * INNER_RATIO;
	}

private:
	double mRadius;

	double mGUIScaleRatio;
	double scaledInnerRadius, scaledOuterRadius;

	const double MIN_ANGLE = 0.666666666 * PI;
	const double KNOB_RANGE = 1.666666666 * PI;

	const double INNER_RATIO = 0.8;
	const double LINE_WIDTH = 1.5;
};

class SbKnob1 : public IKnobControl, ycairo_gui
{
public:
	SbKnob1(IPlugBase* pPlug, ycairo_base *ycairo_base, IRECT pR, int paramIdx = -1,
		EDirection direction = kVertical, double gearing = DEFAULT_GEARING)
		: IKnobControl(pPlug, pR, paramIdx, direction, /*gearing*/ 1), ycairo_gui(ycairo_base, this)
	{
		mRadius = 0.5 * std::min(pR.W(), pR.H());

		mBlend = IChannelBlend(IChannelBlend::kBlendClobber);
		AfterGUIResize(1.0);
	}

	bool Draw(IGraphics* pGraphics) {
		double ang = MIN_ANGLE + mValue * KNOB_RANGE;

		ycairo_prepare_draw();
		cairo_translate(cr, mDrawRECT.MW(), mDrawRECT.MH());

		Point pO(scaledOuterRadius, 0);
		Point pI(scaledInnerRadius, 0);

		cairo_arc(cr, 0, 0, scaledInnerRadius, 0, 2 * PI);
		cairo_set_source_rgb(cr, 0.9, 0.9, 0.9);
		cairo_fill_preserve(cr);

		cairo_set_source_rgb(cr, 0, 0, 0);
		cairo_set_line_width(cr, 1.5);
		cairo_stroke(cr);

		cairo_arc(cr, 0, 0, scaledOuterRadius, ang, MIN_ANGLE + KNOB_RANGE);
		cairo_stroke(cr);

		cairo_arc(cr, 0, 0, scaledOuterRadius, MIN_ANGLE, ang);


		Complex siCo(ang);
		pI.rotaScale(siCo);
		cairo_line_to(cr, pI.X, pI.Y);
		cairo_set_source_rgb(cr, 0.9, 0.9, 0.9);
		cairo_stroke(cr);

		cairo_move_to(cr, pI.X, pI.Y);
		cairo_line_to(cr, 0, 0);
		cairo_set_source_rgb(cr, 1, 0, 0);
		cairo_stroke(cr);

		cairo_identity_matrix(cr);

		ycairo_draw();
		return true;

	}

	void AfterGUIResize(double guiScaleRatio) {
		mGUIScaleRatio = guiScaleRatio;
		scaledOuterRadius = mRadius * mGUIScaleRatio - LINE_WIDTH;
		scaledInnerRadius = scaledOuterRadius * INNER_RATIO;
	}

private:
	double mRadius;

	double mGUIScaleRatio;
	double scaledInnerRadius, scaledOuterRadius;

	const double MIN_ANGLE = 0.666666666 * PI;
	const double KNOB_RANGE = 1.666666666 * PI;

	const double INNER_RATIO = 0.8;
	const double LINE_WIDTH = 1.5;
};


