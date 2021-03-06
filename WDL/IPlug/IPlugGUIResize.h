#pragma once

/*
Youlean - IPlugGUIResize - GUI resizing class

Copyright (C) 2016 and later, Youlean

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. This notice may not be removed or altered from any source distribution.

*/

/*
////////////////////There are 3 main GUI resize modes:////////////////////////

1. View Resize:
With this you will be able to set up for example mini view, normal view
where you will have more knobs etc.

-----------------------------------------------------------------------------

Normal View:                             Mini View:
-------------------                         ----------
|  Knob1   Knob2  |                         |  Knob1 |
|                 |                         |        |
|                 |                         ----------
-------------------

-----------------------------------------------------------------------------

2. Window resize:
This will resize window of different view. For example you could have mini
view and on that mini view you will have some spectrum analyzer, now when
you resize window you can make spectrum analyzer to be resized but the rest
of controls won't be enlarged, and view won't jump to the advanced view.

-----------------------------------------------------------------------------

Normal View + Window Resize:           Mini View + Window Resize:
-------------------////                     ----------////
|  Knob1   Knob2  |////                     |  Knob1 |////
|                 |////                     |        |////
|                 |////                     ----------////
-------------------////                     //////////////
///////////////////////

-----------------------------------------------------------------------------

3. GUI Scaling:
This will take everything you have done with the view and enlarge it.
This is mainly for monitors with higher resolutions (retina etc.)

-----------------------------------------------------------------------------

Normal View:               Normal View - Scaled 2X:
-------------------     --------------------------------------
|  Knob1   Knob2  |     |                                    |
|                 |     |        Knob1          Knob2        |
|                 |     |                                    |
-------------------     |                                    |
                        |                                    |
                        |                                    |
                        --------------------------------------

-----------------------------------------------------------------------------

*/

#include <vector>
#include <math.h> 
#include "IGraphics.h"
#include "IControl.h"

using namespace std;

struct LayoutItem
{
	LayoutItem(IControl* pControl) : org_pointer(pControl), moved_pointer(pControl) {}
	LayoutItem(IControl* pControl, DRECT org_area, int org_is_hidden) 
		: org_pointer(pControl), moved_pointer(pControl), 
		org_draw_area(org_area), org_target_area(org_area), org_is_hidden(org_is_hidden)
	{}

	IControl* org_pointer;
	IControl* moved_pointer;
	DRECT org_draw_area;
	DRECT org_target_area;
	int org_is_hidden;
};

typedef vector<LayoutItem> LayoutContainer;

const double def_min_window_width_normalized = 0.0;
const double def_min_window_height_normalized = 0.0;
const double def_max_window_width_normalized = 999999999999.0;
const double def_max_window_height_normalized = 999999999999.0;

struct ViewItem
{
	ViewItem(
		int view_mode,
		int view_width,
		int view_height,
		double min_window_width_normalized = def_min_window_width_normalized,
		double min_window_height_normalized = def_min_window_height_normalized,
		double max_window_width_normalized = def_max_window_width_normalized,
		double max_window_height_normalized = def_max_window_height_normalized
	) : 
		view_mode(view_mode),
		view_width((double)view_width),
		view_height((double)view_height),
		min_window_width_normalized(min_window_width_normalized),
		min_window_height_normalized(min_window_height_normalized),
		max_window_width_normalized(max_window_width_normalized),
		max_window_height_normalized(max_window_height_normalized)
	{}

	int view_mode;
	double view_width;
	double view_height;
	double min_window_width_normalized; 
	double min_window_height_normalized;
	double max_window_width_normalized;
	double max_window_height_normalized;
	LayoutContainer layout;
};

typedef vector<ViewItem> ViewContainer;


typedef enum _resizeFlag { drawAndTargetArea, drawAreaOnly, targetAreaOnly } resizeFlag;
typedef enum _resizeOneSide { justHorisontalResizing, justVerticalResizing, horisontalAndVerticalResizing } resizeOneSide;

static bool plugin_resized = false;
static bool bitmaps_rescaled_at_load = false;
static double global_gui_scale_ratio = 1.0;


class IPlugGUIResize : public IControl
{
public:
	IPlugGUIResize(IPlugBase *pPlug, IGraphics *pGraphics, bool useHandle = true, int controlSize = 0, int minimumControlSize = 10);
	~IPlugGUIResize() {
		if (pGlobalLayout) DELETE_NULL(pGlobalLayout);
	}

	// These must be called in your plugin constructor -----------------------------------------------------------------------------------------------------
	void UsingBitmaps();
	void DisableFastBitmapResizing();
	void AddNewView(int viewMode, int viewWidth, int viewHeight);
	void UseOneSideResizing(int handleSize, int minHandleSize = 5, resizeOneSide flag = horisontalAndVerticalResizing);
	// -----------------------------------------------------------------------------------------------------------------------------------------------------
	
	
	// These can be called from your custom controls -------------------------------------------------------------------------------------------------------
	void UseHandleForGUIScaling(bool statement = false);
	void UseControlAndClickOnHandleForGUIScaling(bool statement = false);

	void EnableOneSideResizing(resizeOneSide flag = horisontalAndVerticalResizing);
	void DisableOneSideResizing(resizeOneSide flag = horisontalAndVerticalResizing);

	void SelectViewMode(int viewMode);
	void SetGUIScaleRatio(double guiScaleRatio);
	void SetWindowSize(double width, double height);
	void SetWindowWidth(double width);
	void SetWindowHeight(double height);
	void SetGUIScaleLimits(double minSizeInPercentage, double maxSizeInPercentage);
	void SetWindowSizeLimits(int viewMode, double minWindowWidth, double minWindowHeight, double maxWindowWidth, double maxWindowHeight);
	// -----------------------------------------------------------------------------------------------------------------------------------------------------


	// Manipulate controls ----------------------------------------------------------------------------------------------------------------------------------
	void HideControl(int index);
	void HideControl(IControl* pControl);
	void ShowControl(int index);
	void ShowControl(IControl* pControl);

	void MoveControl(int index, double x, double y, resizeFlag flag = drawAndTargetArea);
	void MoveControl(IControl * pControl, double x, double y, resizeFlag flag = drawAndTargetArea);
	void MoveControlHorizontally(int index, double x, resizeFlag flag = drawAndTargetArea);
	void MoveControlHorizontally(IControl * pControl, double x, resizeFlag flag = drawAndTargetArea);
	void MoveControlVertically(int index, double y, resizeFlag flag = drawAndTargetArea);
	void MoveControlVertically(IControl * pControl, double y, resizeFlag flag = drawAndTargetArea);

	void RelativelyMoveControl(int index, double x, double y, resizeFlag flag = drawAndTargetArea);
	void RelativelyMoveControl(IControl * pControl, double x, double y, resizeFlag flag = drawAndTargetArea);
	void RelativelyMoveControlHorizontally(int index, double x, resizeFlag flag = drawAndTargetArea);
	void RelativelyMoveControlHorizontally(IControl * pControl, double x, resizeFlag flag = drawAndTargetArea);
	void RelativelyMoveControlVertically(int index, double y, resizeFlag flag = drawAndTargetArea);
	void RelativelyMoveControlVertically(IControl * pControl, double y, resizeFlag flag = drawAndTargetArea);

	void MoveControlTopEdge(int index, double T, resizeFlag flag = drawAndTargetArea);
	void MoveControlTopEdge(IControl * pControl, double T, resizeFlag flag = drawAndTargetArea);
	void MoveControlLeftEdge(int index, double L, resizeFlag flag = drawAndTargetArea);
	void MoveControlLeftEdge(IControl * pControl, double L, resizeFlag flag = drawAndTargetArea);
	void MoveControlRightEdge(int index, double R, resizeFlag flag = drawAndTargetArea);
	void MoveControlRightEdge(IControl * pControl, double R, resizeFlag flag = drawAndTargetArea);
	void MoveControlBottomEdge(int index, double B, resizeFlag flag = drawAndTargetArea);
	void MoveControlBottomEdge(IControl * pControl, double B, resizeFlag flag = drawAndTargetArea);
	
	void MoveControlRelativeToNonScaledDRECT(int index, DRECT relativeTo, double xRatio, double yRatio, resizeFlag flag = drawAndTargetArea);
	void MoveControlRelativeToNonScaledDRECT(IControl * pControl, DRECT relativeTo, double xRatio, double yRatio, resizeFlag flag = drawAndTargetArea);
	void MoveControlHorizontallyRelativeToNonScaledDRECT(int index, DRECT relativeTo, double xRatio, resizeFlag flag = drawAndTargetArea);
	void MoveControlHorizontallyRelativeToNonScaledDRECT(IControl * pControl, DRECT relativeTo, double xRatio, resizeFlag flag = drawAndTargetArea);
	void MoveControlVerticallyRelativeToNonScaledDRECT(int index, DRECT relativeTo, double yRatio, resizeFlag flag = drawAndTargetArea);
	void MoveControlVerticallyRelativeToNonScaledDRECT(IControl * pControl, DRECT relativeTo, double yRatio, resizeFlag flag = drawAndTargetArea);

	void MoveControlRelativeToControlDrawRect(int moveControlIndex, int relativeToControlIndex, double xRatio, double yRatio, resizeFlag flag = drawAndTargetArea);
	void MoveControlRelativeToControlDrawRect(IControl * moveControl, IControl * relativeToControl, double xRatio, double yRatio, resizeFlag flag = drawAndTargetArea);
	void MoveControlHorizontallyRelativeToControlDrawRect(int moveControlIndex, int relativeToControlIndex, double xRatio, resizeFlag flag = drawAndTargetArea);
	void MoveControlHorizontallyRelativeToControlDrawRect(IControl * moveControl, IControl * relativeToControl, double xRatio, resizeFlag flag = drawAndTargetArea);
	void MoveControlVerticallyRelativeToControlDrawRect(int moveControlIndex, int relativeToControlIndex, double yRatio, resizeFlag flag = drawAndTargetArea);
	void MoveControlVerticallyRelativeToControlDrawRect(IControl * moveControl, IControl * relativeToControl, double yRatio, resizeFlag flag = drawAndTargetArea);

	void MoveControlRelativeToWindowSize(int index, resizeFlag flag = drawAndTargetArea);
	void MoveControlRelativeToWindowSize(IControl * moveControl, resizeFlag flag = drawAndTargetArea);
	void MoveControlHorizontallyRelativeToWindowSize(int index, resizeFlag flag = drawAndTargetArea);
	void MoveControlHorizontallyRelativeToWindowSize(IControl * moveControl, resizeFlag flag = drawAndTargetArea);
	void MoveControlVerticallyRelativeToWindowSize(int index, resizeFlag flag = drawAndTargetArea);
	void MoveControlVerticallyRelativeToWindowSize(IControl * moveControl, resizeFlag flag = drawAndTargetArea);

	void ResizeControlRelativeToWindowSize(int index, resizeFlag flag = drawAndTargetArea);
	void ResizeControlRelativeToWindowSize(IControl * pControl, resizeFlag flag = drawAndTargetArea);
	void ResizeControlHorizontalyRelativeToWindowSize(int index, resizeFlag flag = drawAndTargetArea);
	void ResizeControlHorizontalyRelativeToWindowSize(IControl * pControl, resizeFlag flag = drawAndTargetArea);
	void ResizeControlVerticallyRelativeToWindowSize(int index, resizeFlag flag = drawAndTargetArea);
	void ResizeControlVerticallyRelativeToWindowSize(IControl * pControl, resizeFlag flag = drawAndTargetArea);

	void SetNormalizedDrawRect(int index, double L, double T, double R, double B);
	void SetNormalizedDrawRect(IControl *pControl, double L, double T, double R, double B);
	void SetNormalizedDrawRect(int index, DRECT r);
	void SetNormalizedDrawRect(IControl *pControl, DRECT r);

	void SetNormalizedTargetRect(int index, double L, double T, double R, double B);
	void SetNormalizedTargetRect(IControl *pControl, double L, double T, double R, double B);
	void SetNormalizedTargetRect(int index, DRECT r);
	void SetNormalizedTargetRect(IControl *pControl, DRECT r);
	// ------------------------------------------------------------------------------------------------------------------------------------------------------
		
	
	// Get the values ---------------------------------------------------------------------------------------------------------------------------------------
	double GetGUIScaleRatio();
	int GetViewMode();
	int GetViewModeSize();
	bool CurrentlyFastResizing();
	double GetWidnowSizeWidthRatio();
	double GetWidnowSizeHeightRatio();
	bool IsAttachedToIPlugBase();
	DRECT GetOriginalDrawRECTForControl(IControl *pControl)
	{
		int index = FindLayoutPointerPosition(current_view_mode, pControl);
		if (index < 0) return DRECT();
		
		return view[current_view_mode].layout[index].org_draw_area;
	}
	DRECT GetOriginalTargetRECTForControl(IControl *pControl)
	{
		int index = FindLayoutPointerPosition(current_view_mode, pControl);
		if (index < 0) return DRECT();

		return view[current_view_mode].layout[index].org_target_area;
	}
	// ------------------------------------------------------------------------------------------------------------------------------------------------------
	
	// You can override this to use in your custom resizing control -----------------------------------------------------------------------------------------
	virtual void DrawBackgroundAtFastResizing(IGraphics* pGraphics, IRECT *pRECT);
	virtual void DrawReopenPluginInterface(IGraphics* pGraphics, IRECT *pRECT);
	virtual void DrawHandle(IGraphics* pGraphics, IRECT *pRECT);
	virtual void DoPopupMenu() {}
	// ------------------------------------------------------------------------------------------------------------------------------------------------------

	// Call this to resize GUI
	void ResizeGraphics();

	// Used by the framework -----------------------------------------------------------------------------------------------------------------------
	IParam* GetGUIResizeParameter(int index);
	int GetGUIResizeParameterSize();
	void ResizeAtGUIOpen();
	bool Draw(IGraphics* pGraphics);
	void RescaleBitmapsAtLoad();
	IPlugGUIResize *AttachGUIResize();
	void LiveEditSetLayout(int viewMode, int moveToIndex, int moveFromIndex, IRECT drawRECT, IRECT targetRECT, bool isHidden);
	void LiveRemoveLayer(IControl* pControl);
	bool IsDirty();
	// ---------------------------------------------------------------------------------------------------------------------------------------------
		
private:
	// Functions that are used internally ----------------------------------------------------------------------------------------------------------
	bool double_equals(double a, double b, double epsilon = 0.0000000001);
	DRECT IRECT_to_DRECT(IRECT * iRECT);
	IRECT DRECT_to_IRECT(DRECT * dRECT);
	IRECT RescaleToIRECT(DRECT * old_IRECT, double width_ratio, double height_ratio);
	DRECT RescaleToDRECT(DRECT * old_IRECT, double width_ratio, double height_ratio);
	DRECT* GetLayoutContainerDrawRECT(int viewMode, IControl* pControl);
	DRECT* GetLayoutContainerTargetRECT(int viewMode, IControl* pControl);
	int* GetLayoutContainerIsHidden(int viewMode, IControl* pControl);
	void SetLayoutContainerAt(int viewMode, IControl* pControl, DRECT drawIn, DRECT targetIn, int isHiddenIn);
	int FindLayoutPointerPosition(int viewMode, IControl* pControl);
	void RearrangeLayers();

	void SetIntToFile(const char *name, int x);
	int GetIntFromFile(const char *name);
	void SetDoubleToFile(const char *name, double x);
	double GetDoubleFromFile(const char *name);

	void ResizeBackground();
	void ResizeControlRects();
	void InitializeGUIControls(IGraphics *pGraphics);
	void ResetControlsVisibility();

	void MoveHandle();

	void OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod);
	void OnMouseOver(int x, int y, IMouseMod* pMod);
	void OnMouseOut();
	void OnMouseDown(int x, int y, IMouseMod* pMod);
	void OnMouseDblClick(int x, int y, IMouseMod * pMod);
	void OnMouseUp(int x, int y, IMouseMod* pMod);
	// ---------------------------------------------------------------------------------------------------------------------------------------------


	int current_view_mode;

	ViewContainer view;
	LayoutContainer* pGlobalLayout;

	bool use_handle = true;
	bool handle_controls_gui_scaling = false;
	bool control_and_click_on_handle_controls_gui_scaling = false;

	// Parameters set
	int viewMode = 0,
		windowWidth = 1,
		windowHeight = 2;

	IGraphics *mGraphics;

	// GUI scale variables
	double gui_scale_ratio = 1.0;
	double min_gui_scale_ratio = 0.0;
	double max_gui_scale_ratio = 1000000000.0;

	// Window size variables
	double window_width_normalized, window_height_normalized;

	// One side resizing variables
	int one_side_handle_size = 0, one_side_handle_min_size = 0;
	bool using_one_size_resize = false;

	int default_gui_width, default_gui_height;
	int plugin_width, plugin_height; // This is current plugin instance width
	int min_control_size, control_size;
	bool currentlyFastResizing = false;
	bool mouse_is_dragging = false;
	bool gui_should_be_closed = false;
	bool using_bitmaps = false;
	bool fast_bitmap_resizing = true;
	bool bitmaps_rescaled_at_load_skip = false;
	bool presets_loaded = false;
	double* backup_parameters;
	IRECT gui_resize_area;
	WDL_String settings_ini_path;
	char buf[128]; // temp buffer for writing integers to profile strings
	resizeOneSide one_side_flag;
	WDL_PtrList<IParam> guiResizeParameters;

	bool attachedToIPlugBase = false;

	friend class IPlugGUILiveEdit;
};

// One side handle classes
// BaseResizing  - Base class for Horizontal and Vertical Handles ------------------------------------
class BaseResizing : public IControl
{
public:
	BaseResizing::BaseResizing(IPlugBase *pPlug, IGraphics *pGraphics, int width)
		: IControl(pPlug, IRECT(pGraphics->Width() - width, 0, pGraphics->Width(), pGraphics->Height())) {
		mGraphics = pGraphics;
		mGraphics->HandleMouseOver(true);
	}
protected:
	~BaseResizing() {} // protected destructor prevents instatiation of base class
public:
	bool BaseResizing::Draw(IGraphics * pGraphics) {
		return true;
	}
	void BaseResizing::OnMouseDrag(int x, int y, int dX, int dY, IMouseMod * pMod) {
		double window_width_normalized = (double)x / GetGUIResize()->GetGUIScaleRatio();
		GetGUIResize()->SetWindowWidth(window_width_normalized);
		GetGUIResize()->ResizeGraphics();
	}
private:
	IGraphics* mGraphics;
};

// Horizontal handle ------------------------------------------------------------------------------------
// NOTE: Horizontal control position is control size - 2
class HorizontalResizing : public BaseResizing
{
public:
	HorizontalResizing::HorizontalResizing(IPlugBase *pPlug, IGraphics *pGraphics, int width)
		: BaseResizing(pPlug, pGraphics, width) {}

	void HorizontalResizing::OnMouseDown(int x, int y, IMouseMod * pMod) {
		SetCursor(LoadCursor(NULL, IDC_SIZEWE));
	}
	void HorizontalResizing::OnMouseDrag(int x, int y, int dX, int dY, IMouseMod * pMod) {
		BaseResizing::OnMouseDrag(x, y, dX, dY, pMod);
		SetCursor(LoadCursor(NULL, IDC_SIZEWE));
	}
	void HorizontalResizing::OnMouseOver(int x, int y, IMouseMod * pMod) {
		SetCursor(LoadCursor(NULL, IDC_SIZEWE));
	}
	void HorizontalResizing::OnMouseOut() {
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
};

// Vertical handle ------------------------------------------------------------------------------------
// NOTE: Vertical control position is control size - 3
class VerticalResizing : public BaseResizing
{
public:
	VerticalResizing::VerticalResizing(IPlugBase *pPlug, IGraphics *pGraphics, int width)
		: BaseResizing(pPlug, pGraphics, width) {}

	void VerticalResizing::OnMouseDown(int x, int y, IMouseMod * pMod) {
		SetCursor(LoadCursor(NULL, IDC_SIZENS));
	}
	void VerticalResizing::OnMouseDrag(int x, int y, int dX, int dY, IMouseMod * pMod) {
		BaseResizing::OnMouseDrag(x, y, dX, dY, pMod);
		SetCursor(LoadCursor(NULL, IDC_SIZENS));
	}
	void VerticalResizing::OnMouseOver(int x, int y, IMouseMod * pMod) {
		SetCursor(LoadCursor(NULL, IDC_SIZENS));
	}
	void VerticalResizing::OnMouseOut() {
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
};