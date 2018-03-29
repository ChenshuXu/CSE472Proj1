
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "project.h"
#include "ChildView.h"
#include "graphics/GrCamera.h"
#include "Torus.h"

// CChildView

CChildView::CChildView()
{
	m_camera.Set(80, 20, 60, 0, 0, 0, 0, 1, 0);
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, COpenGLWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_CAMERA_FREECAMERA, &CChildView::OnCameraFreecamera)
	ON_COMMAND(ID_CAMERA_AERIALCAMERA, &CChildView::OnCameraAerialcamera)
	ON_COMMAND(ID_CAMERA_CAMERA1, &CChildView::OnCameraCamera1)
	ON_COMMAND(ID_CAMERA_CAMERA2, &CChildView::OnCameraCamera2)
	ON_COMMAND(ID_CAMERA_BOBSLEDCAMERA, &CChildView::OnCameraBobsledcamera)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!COpenGLWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}


void CChildView::OnGLDraw(CDC* pDC)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//
	// Set up the camera
	//
	int width, height;
	GetSize(width, height);
	m_camera.Apply(width, height);
						  // Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Cull backfacing polygons
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// Draw a coordinate axis
	glColor3d(0., 1., 1.);

	glBegin(GL_LINES);
	glVertex3d(0., 0., 0.);
	glVertex3d(12., 0., 0.);
	glVertex3d(0., 0., 0.);
	glVertex3d(0., 12., 0.);
	glVertex3d(0., 0., 0.);
	glVertex3d(0., 0., 12.);
	glEnd();


	// 
	// INSERT DRAWING CODE HERE
	//
	

	glPushMatrix();
	glRotated(180, 1, 0, 0);
	glRotated(0, 0, 1, 0);
	glRotated(14, 0, 0, 1);
	glTranslated(15, -11.4, -20);
	m_trackCurve1.Draw();
	glPopMatrix();

	glPushMatrix();
	glRotated(180, 1, 0, 0);
	glRotated(180, 0, 1, 0);
	glRotated(-14, 0, 0, 1);
	glTranslated(-25, -11.4, 10);
	m_trackCurve2.Draw();
	glPopMatrix();

	const double RED[] = { 1, 1, 1 };

	SlideBase(40., 10., 30., RED, 0., 0., 1.);
}

//
//        Name : Quad()
// Description : Inline function for drawing 
//               a quadralateral.
//
inline void Quad(GLdouble *v1, GLdouble *v2, GLdouble *v3, GLdouble *v4)
{
	glBegin(GL_QUADS);
	glVertex3dv(v1);
	glVertex3dv(v2);
	glVertex3dv(v3);
	glVertex3dv(v4);
	glEnd();
}

inline void Tria(GLdouble *v1, GLdouble *v2, GLdouble *v3)
{
	glBegin(GL_TRIANGLES);
	glVertex3dv(v1);
	glVertex3dv(v2);
	glVertex3dv(v3);
	glEnd();
}

void CChildView::SlideBase(GLdouble p_x, GLdouble p_y, GLdouble p_z, const GLdouble *p_color, GLdouble x_offset, GLdouble y_offset, GLdouble z_offset)
{
	GLdouble a[] = { x_offset, y_offset, z_offset+p_z };
	GLdouble b[] = { x_offset+p_x, y_offset, z_offset+p_z };
	GLdouble d[] = { x_offset, y_offset+p_y, z_offset+p_z };
	GLdouble e[] = { x_offset, y_offset, z_offset };
	GLdouble f[] = { x_offset+p_x, y_offset, z_offset };
	GLdouble h[] = { x_offset, y_offset+p_y, z_offset };

	// I'm going to mess with the colors a bit so
	// the faces will be visible in solid shading
	glColor3d(p_color[0], p_color[1], p_color[2]);
	Tria(a, b, d); // Front

	glColor3d(p_color[0] * 1, p_color[1] * 0.95, p_color[2] * 0.95);
	Quad(b, f, h, d); // Top

	glColor3d(p_color[0] * 0.85, p_color[1] * 0, p_color[2] * 0.85);
	Tria(f, e, h); // Back

	glColor3d(p_color[0] * 0.90, p_color[1] * 0, p_color[2] * 0.90);
	Quad(h, e, a, d); // Left

	glColor3d(p_color[0] * 0.80, p_color[1] * 0.80, p_color[2] * 0);
	Quad(a, e, f, b); // Bottom
}


void CChildView::OnCameraFreecamera()
{
	// TODO: Add your command handler code here
	camNum = 0;
	m_camera.Set(80, 20, 60, 0, 0, 0, 0, 1, 0);
	Invalidate();
}


void CChildView::OnCameraAerialcamera()
{
	// TODO: Add your command handler code here
	camNum = 1;
	m_camera.Set(40, 10, 50, 0, 0, 0, 0, 1, 0);
	Invalidate();
}


void CChildView::OnCameraCamera1()
{
	// TODO: Add your command handler code here
	camNum = 2;
	m_camera.Set(40, 10, 50, 0, 0, 0, 0, 1, 0);
	Invalidate();
}


void CChildView::OnCameraCamera2()
{
	// TODO: Add your command handler code here
	camNum = 3;
}


void CChildView::OnCameraBobsledcamera()
{
	// TODO: Add your command handler code here
	camNum = 4;
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (camNum == 0) {
		m_camera.MouseDown(point.x, point.y);
	}

	COpenGLWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (camNum == 0) {
		if (m_camera.MouseMove(point.x, point.y, nFlags))
			Invalidate();
	}

	COpenGLWnd::OnMouseMove(nFlags, point);
}


void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (camNum == 0) {
		m_camera.MouseDown(point.x, point.y, 2);
	}

	COpenGLWnd::OnRButtonDown(nFlags, point);
}
