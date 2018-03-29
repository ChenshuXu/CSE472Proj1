
// ChildView.h : interface of the CChildView class
//
// test

#pragma once
#include "graphics/OpenGLWnd.h"
#include "graphics/GrCamera.h"
#include "Torus.h"
// CChildView window

class CChildView : public COpenGLWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	void OnGLDraw(CDC* pDC);
	void SlideBase(GLdouble p_x, GLdouble p_y, GLdouble p_z, const GLdouble * p_color, GLdouble x_offset, GLdouble y_offset, GLdouble z_offset);
	void Box(GLdouble p_x, GLdouble p_y, GLdouble p_z, const GLdouble * p_color);
	afx_msg void OnCameraFreecamera();
	afx_msg void OnCameraAerialcamera();
	afx_msg void OnCameraCamera1();
	afx_msg void OnCameraCamera2();
	afx_msg void OnCameraBobsledcamera();
	
private:
	int camNum = 0;
	CGrCamera m_camera;
	CTorus m_trackCurve1;
	CTorus m_trackCurve2;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);


};

