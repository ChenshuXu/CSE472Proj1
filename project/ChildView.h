
// ChildView.h : interface of the CChildView class
//

#pragma once
#include "graphics/OpenGLWnd.h"
#include "graphics/GrCamera.h"
#include "Torus.h"
#include "TorusStraight.h"
#include "graphics/GrTexture.h"

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
	void drw_straightTrack();
	void Kinfe(GLdouble p_x, GLdouble p_y, GLdouble p_z, const GLdouble *p_color, GLdouble x_offset, GLdouble y_offset, GLdouble z_offset);
	void drw_carBody(int n, int arg, float mult, float v, const GLdouble *p_color);
	void draw_car(GLdouble p_x, GLdouble p_y, GLdouble p_z);
	void draw_track();
	afx_msg void OnCameraFreecamera();
	afx_msg void OnCameraAerialcamera();
	afx_msg void OnCameraCamera1();
	afx_msg void OnCameraCamera2();
	afx_msg void OnCameraBobsledcamera();
	
private:
	int camNum = 0;
	CGrCamera m_camera;
	//CTorus m_tori;

	CTorusStraight m_straightTrack1;
	CTorusStraight m_straightTrack2;
	CTorusStraight m_straightTrack3;

	CTorus m_trackCurve1;
	CTorus m_trackCurve2;
	CGrTexture m_ice;
	CGrTexture m_snow;
	CGrTexture m_flag;
	CGrTexture m_test;



	UINT_PTR m_AnimTime = 0;

	double m_changeX = 0;
	double m_changeY = 0;
	double m_changeZ = 0;
	double m_changeAngleX = 0;
	double m_changeAngleY = 0;
	double m_changeAngleZ = 0;

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	


	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnAnimationStart();
};

