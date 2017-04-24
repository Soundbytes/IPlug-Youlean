#pragma once

#include "IControl.h"
#include "ycairo.h"


class SimpleCairoControl : public IKnobControl, ycairo_gui
{
public:
	SimpleCairoControl(IPlugBase* pPlug, ycairo_base *ycairo_base, IRECT pR, int paramIdx = -1,
		double innerRadius = 0.0, double outerRadius = 0.0,
		double minAngle = -0.833333 * PI, double maxAngle = 0.833333 * PI,
		EDirection direction = kVertical, double gearing = DEFAULT_GEARING)
		: IKnobControl(pPlug, pR, paramIdx, direction, /*gearing*/ 1), ycairo_gui(ycairo_base, this)
	{
		mMinAngle = (float)minAngle;
		mMaxAngle = (float)maxAngle;
		mOuterRadius = (float)outerRadius;
		if (mOuterRadius == 0.0f) {
			mOuterRadius = 0.5f * (float)pR.W();
		}
		mInnerRadius = innerRadius == 0.0f ? mOuterRadius * 0.5f : (float)innerRadius;

		mBlend = IChannelBlend(IChannelBlend::kBlendClobber);
		AfterGUIResize(1.0);
	}

	bool Draw(IGraphics* pGraphics)
	{
		double v = mMinAngle + mValue * (mMaxAngle - mMinAngle);

		ycairo_prepare_draw();

		cairo_translate(cr, mDrawRECT.MW(), mDrawRECT.MH());

		cairo_arc(cr, 0, 0, scaledOuterRadius, 0, 2 * PI);
		cairo_set_source_rgb(cr, 0.9, 0.9, 0.9);
		cairo_fill_preserve(cr);

		cairo_set_source_rgb(cr, 0, 0, 0);
		cairo_set_line_width(cr, 1.5);
		cairo_stroke(cr);

		cairo_rotate(cr, v);

		cairo_move_to(cr, 0, -scaledInnerRadius);
		cairo_line_to(cr, 0, -scaledOuterRadius);

		cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
		cairo_set_source_rgb(cr, 0.2, 0.2, 0.3);
		cairo_set_line_width(cr, markerWidth);
		cairo_stroke(cr);

		// Make sure to revert all transforms in REVERSE order! 
		//cairo_rotate(cr, -v);
		//cairo_translate(cr, -mDrawRECT.MW(), -mDrawRECT.MH());

		cairo_identity_matrix(cr);

		ycairo_draw();
		return true;
	}

	virtual void AfterGUIResize(double guiScaleRatio) {
		mGUIScaleRatio = guiScaleRatio;
		scaledInnerRadius = mInnerRadius * mGUIScaleRatio;
		scaledOuterRadius = mOuterRadius * mGUIScaleRatio;
		markerWidth = scaledOuterRadius * 0.2;
		scaledOuterRadius *= 0.9;
	}

private:
	double mMinAngle, mMaxAngle, mInnerRadius, mOuterRadius;

	double mGUIScaleRatio;
	double scaledInnerRadius, scaledOuterRadius;
	double markerWidth;
};


