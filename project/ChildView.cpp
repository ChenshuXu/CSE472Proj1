
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "project.h"
#include "ChildView.h"
#include "graphics/GrCamera.h"
#include "Torus.h"
#include "TorusStraight.h"

const double GR_PI = 3.1415926535897932384626433832795;
const double WHITE[] = { 1,1,1 };
const double PINK[] = { 1,0.9,0.9 };
const double RED[] = { 1,0,0};
const double acceleration = 9.8 * sin(14.04 * GR_PI / 180) / 1000;

// CChildView

CChildView::CChildView()
{
	m_camera.Set(80, 20, 60, 0, 0, 0, 0, 1, 0);
	m_ice.LoadFile(L"texture/color_bar.jpg");
	m_snow.LoadFile(L"texture/snow.jpg");
	m_flag.LoadFile(L"texture/flag.png");
	//m_test.LoadFile(L"texture/test.jpg");

	m_trackCurve1.SetTexture(&m_ice);
	m_trackCurve2.SetTexture(&m_ice);

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
	ON_WM_TIMER()
	ON_COMMAND(ID_ANIMATION_START, &CChildView::OnAnimationStart)
	ON_COMMAND(ID_ANIMATION_RESET, &CChildView::OnAnimationReset)
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!COpenGLWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

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
	//glColor3d(0., 1., 1.);

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
	draw_track();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0., -0.15, 0.);
	SlideBase(40., 10., 30., WHITE, 0., 0., 1.);
	glPopMatrix();

	glPushMatrix();
	//glRotated(m_changeAngleY, 0, 1, 0);
	glTranslated(7 + m_changeX, 9.4 - m_changeY, 24.35 + m_changeZ);
	draw_car(0., 0., 0., m_changeAngleX, m_changeAngleY, 0.); // p_x , p_y, p_z to (0,0,0) Start Position r_x, r_y, r_z Rotation offset 
	glPopMatrix();
	
}

//
//        Name : Quad()
// Description : Inline function for drawing 
//               a quadralateral.
//
inline void Quad(GLdouble *v1, GLdouble *v2, GLdouble *v3, GLdouble *v4)
{
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3dv(v1);
	glTexCoord2f(1, 0);
	glVertex3dv(v2);
	glTexCoord2f(1, 1);
	glVertex3dv(v3);
	glTexCoord2f(0, 1);
	glVertex3dv(v4);
	glEnd();
}

//inline void QuadTrack(GLdouble *v1, GLdouble *v2, GLdouble *v3, GLdouble *v4, double step2r, double a2a, double a2b)
//{
//	glBegin(GL_QUADS);
//	glTexCoord2d(a2a / GR_PI, 0. / GR_PI / step2r);
//	glVertex3dv(v1);
//	glTexCoord2d(a2a / GR_PI, 1. / GR_PI / step2r);
//	glVertex3dv(v2);
//	glTexCoord2d(a2b / GR_PI, 1. / GR_PI / step2r);
//	glVertex3dv(v3);
//	glTexCoord2d(a2b / GR_PI, 0. / GR_PI / step2r);
//	glVertex3dv(v4);
//	glEnd();
//}

inline void QuadTrack(GLdouble *v1, GLdouble *v2, GLdouble *v3, GLdouble *v4) {
	glBegin(GL_QUADS);
	//glTexCoord2d(a2a / GR_PI, 0. / GR_PI / step2r);
	glVertex3dv(v1);
	//glTexCoord2d(a2a / GR_PI, 1. / GR_PI / step2r);
	glVertex3dv(v2);
	//glTexCoord2d(a2b / GR_PI, 1. / GR_PI / step2r);
	glVertex3dv(v3);
	//glTexCoord2d(a2b / GR_PI, 0. / GR_PI / step2r);
	glVertex3dv(v4);
	glEnd();
}

inline void Tria(GLdouble *v1, GLdouble *v2, GLdouble *v3)
{
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 0);
	glVertex3dv(v1);
	glTexCoord2f(1, 1);
	glVertex3dv(v2);
	glTexCoord2f(0, 1);
	glVertex3dv(v3);
	glEnd();
}

void CChildView::SlideBase(GLdouble p_x, GLdouble p_y, GLdouble p_z, const GLdouble *p_color, GLdouble x_offset, GLdouble y_offset, GLdouble z_offset)
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, m_snow.TexName());

	GLdouble a[] = { x_offset, y_offset, z_offset + p_z };
	GLdouble b[] = { x_offset + p_x, y_offset, z_offset + p_z };
	GLdouble d[] = { x_offset, y_offset + p_y, z_offset + p_z };
	GLdouble e[] = { x_offset, y_offset, z_offset };
	GLdouble f[] = { x_offset + p_x, y_offset, z_offset };
	GLdouble h[] = { x_offset, y_offset + p_y, z_offset };

	// I'm going to mess with the colors a bit so
	// the faces will be visible in solid shading
	glColor3d(p_color[0], p_color[1], p_color[2]);
	Tria(a, b, d); // Front

				   //glColor3d(p_color[0] * 1, p_color[1] * 0.95, p_color[2] * 0.95);
	Quad(b, f, h, d); // Top

					  //glColor3d(p_color[0] * 0.85, p_color[1] * 0, p_color[2] * 0.85);
	Tria(f, e, h); // Back

				   //glColor3d(p_color[0] * 0.90, p_color[1] * 0, p_color[2] * 0.90);
	Quad(h, e, a, d); // Left

					  //glColor3d(p_color[0] * 0.80, p_color[1] * 0.80, p_color[2] * 0);
	Quad(a, e, f, b); // Bottom
	glDisable(GL_TEXTURE_2D);

}

void CChildView::drw_straightTrack() {

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, m_ice.TexName());

	/*
	Function drw_polygon:
	Arguments:
	n - number of sides
	arg - starting angle (not so important at all)
	mult - multiplying sides to incrase their length
	v - cylinder height
	*/
	int n = 50; // number of steps
	float r = 1.5; // radius of inner half circle
	float r2 = 1.8; // radius of outer half circle
	int arg = 0; // start angle
	float mult = 1.5;
	float v = 10.0;
	float length = 10.0; // Length of track

	const double step2r = 2. * GR_PI / n / 2;

	double previous_x = r * cos(0);
	double previous_y = r * sin(0);

	double count = 0;

	///////////////////
	double coord = 0;

	for (int i = arg; i <= arg + 180;i += (180 / n)) {
		float rad = i * GR_PI / 180; // degrees to radians

		GLdouble a[] = { previous_x, previous_y, 0 };
		GLdouble b[] = { previous_x, previous_y, length };
		GLdouble c[] = { r * cos(rad), r * sin(rad), length };
		GLdouble d[] = { r * cos(rad), r * sin(rad), 0 };

		coord = coord + (1.0 / 60);
		glTexCoord2d(coord , 0);
		QuadTrack(a, b, c, d);
		QuadTrack(a, d, c, b);
		//QuadTrack(a, b, c, d, rad, coord, i);
		//QuadTrack(a, d, c, b, rad, coord, i);


		previous_x = r * cos(rad);
		previous_y = r * sin(rad);
		count++;

	}

	// outer circle
	previous_x = r2 * cos(0);
	previous_y = r2 * sin(0);
	count = 0;

	///////////////////
	coord = 0;

	for (int i = arg; i <= arg + 180; i += (180 / n)) {
		float rad = i * GR_PI / 180; // degrees to radians

		GLdouble a[] = { previous_x, previous_y, 0 };
		GLdouble b[] = { previous_x, previous_y, length };
		GLdouble c[] = { r2 * cos(rad), r2 * sin(rad), length };
		GLdouble d[] = { r2 * cos(rad), r2 * sin(rad), 0 };

		coord = coord + (1.0 / 60);
		glTexCoord2d(coord, 0);
		QuadTrack(a, b, c, d);
		QuadTrack(a, d, c, b);
		//QuadTrack(a, b, c, d, rad, coord, i);
		//QuadTrack(a, d, c, b, rad, coord, i);


		previous_x = r2 * cos(rad);
		previous_y = r2 * sin(rad);
		count++;

	}
	glDisable(GL_TEXTURE_2D);

}

void CChildView::Kinfe(GLdouble p_x, GLdouble p_y, GLdouble p_z, const GLdouble *p_color, GLdouble x_offset, GLdouble y_offset, GLdouble z_offset)
{
	GLdouble a[] = { x_offset, y_offset, z_offset + p_z };
	GLdouble b[] = { x_offset + p_x / 2, y_offset - p_y, z_offset + p_z };
	GLdouble d[] = { x_offset + p_x, y_offset, z_offset + p_z };
	GLdouble e[] = { x_offset, y_offset, z_offset };
	GLdouble f[] = { x_offset + p_x / 2, y_offset - p_y, z_offset };
	GLdouble h[] = { x_offset + p_x, y_offset, z_offset };

	// I'm going to mess with the colors a bit so
	// the faces will be visible in solid shading
	glColor3d(p_color[0], p_color[1], p_color[2]);
	Tria(a, b, d); // Front

				   //glColor3d(p_color[0] * 1, p_color[1] * 0.95, p_color[2] * 0.95);
	Quad(b, f, h, d); // Top

					  //glColor3d(p_color[0] * 0.85, p_color[1] * 0, p_color[2] * 0.85);
	Tria(f, e, h); // Back

				   //glColor3d(p_color[0] * 0.90, p_color[1] * 0, p_color[2] * 0.90);
	Quad(h, e, a, d); // Left

					  //glColor3d(p_color[0] * 0.80, p_color[1] * 0.80, p_color[2] * 0);
	Quad(a, e, f, b); // Bottom
}

void CChildView::drw_carBody(int n, int arg, float mult, float v, const GLdouble *p_color) {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, m_flag.TexName());
	/*
	+	Function drw_polygon:
	+	Arguments:
	+	n - number of sides
	+	arg - starting angle (not so important at all)
	+	mult - multiplying sides to incrase their length
	+	v - cylinder height
	+	*/

	// DumbProof Double Check :)
	if (arg < 0)
		arg = 0;

	// Cylinder Bottom
	glBegin(GL_POLYGON);
	//glColor3d(p_color[0], p_color[1], p_color[2]);
	for (int i = arg; i <= (360 + arg); i += (360 / n)) {
		float a = i * GR_PI / 180; // degrees to radians
		glVertex3f(mult * sin(a), mult * cos(a), 0.0);
		glTexCoord2d(0,0);


	}
	glEnd();

	// Cylinder Top
	glBegin(GL_POLYGON);
	//glColor4f(0.0, 0.0, 1.0, 1.0);
	for (int i = arg; i <= (360 + arg); i += (360 / n)) {
		float a = i * GR_PI / 180; // degrees to radians
		glVertex3f(mult * cos(a), mult * sin(a), v);


	}
	glEnd();

	// Cylinder "Cover"
	glBegin(GL_QUAD_STRIP);
	//glColor4f(1.0, 1.0, 0.0, 1.0);
	for (int i = arg; i < 480; i += (360 / n)) {
		float a = i * GR_PI / 180; // degrees to radians
		glTexCoord2d(0, 0);

		glVertex3f(mult * cos(a), mult * sin(a), v);
		glTexCoord2d(mult * cos(a), mult * sin(a));

		glVertex3f(mult * cos(a), mult * sin(a), 0.0);


	}
	glEnd();
	glDisable(GL_TEXTURE_2D);


}

void CChildView::draw_car(GLdouble p_x, GLdouble p_y, GLdouble p_z, GLdouble r_x, GLdouble r_y, GLdouble r_z) {
	
	
	// cylinder
	glPushMatrix();
	//glTranslated(-24 - p_z, 11.65 + p_y, 6 + p_x);
	//glRotated(87, 0, 1, 0);
	//glRotated(14, 1, 0, 0);
	//glRotated(0, 0, 1, 0);
	//glTranslated(-24, 11.65, 6);
	glRotated(87 + r_y, 0, 1, 0);
	glRotated(14.04 + r_x, 1, 0, 0);
	drw_carBody(360, 0, 0.6, 1.5, PINK);
	glPopMatrix();

	// knife 1
	glPushMatrix();
	glRotated(87 + r_y, 0, 1, 0);
	glRotated(14.04 + r_x, 1, 0, 0);
	glTranslated(0.35, -0.6, 0.8);
	Kinfe(.3, 0.6, 1.2, RED, -0.15, 0.3, -0.6);
	//glRotated(-3, 0, 1, 0);
	//glRotated(-90, 1, 0, 0);
	//glRotated(0, 1, 0, 0);
	//glTranslated(-11.15, -23.4, 6.2);
	//Kinfe(0.8, .2, 2.5, RED, -11.15 - p_y, -23.4 - p_z, 6.2 + p_x);
	glPopMatrix();

	// knife 2
	glPushMatrix();
	glRotated(87 + r_y, 0, 1, 0);
	glRotated(14.04 + r_x, 1, 0, 0);
	glTranslated(-0.35, -0.6, 0.8);
	Kinfe(.3, 0.6, 1.2, RED, -0.15, 0.3, -0.6);
	//glRotated(-3, 0, 1, 0);
	//glRotated(0, 1, 0, 0);
	//Kinfe(0.8, .2, 2.5, RED, -11.15 - p_y, -24.7 - p_z, 6.2 + p_x);
	glPopMatrix();
}

void CChildView::draw_track() {
	glColor3d(1, 1, 1);

	// Curve track 1
	glPushMatrix();
	glRotated(180, 1, 0, 0);
	glRotated(0, 0, 1, 0);
	glRotated(14.04, 0, 0, 1);
	glTranslated(15, -11.4, -20);
	m_trackCurve1.Draw();
	glPopMatrix();

	// Curve track 2
	glPushMatrix();
	glRotated(180, 1, 0, 0);
	glRotated(180, 0, 1, 0);
	glRotated(-14.04, 0, 0, 1);
	glTranslated(-24.88, -11.36, 10);
	m_trackCurve2.Draw();
	glPopMatrix();

	// Straight track 1
	glPushMatrix();
	glRotated(0, 1, 0, 0);
	glRotated(0, 0, 1, 0);
	glRotated(165, 0, 0, 1);
	glTranslated(-19.78, -11.73, 10);
	drw_straightTrack();
	glPopMatrix();

	// Straight track 2
	glPushMatrix();
	glRotated(-3, 0, 1, 0);
	glRotated(-13.7, 0, 0, 1);
	glRotated(180, 1, 0, 1);
	glTranslated(24, -11.6, 6);
	drw_straightTrack();
	glPopMatrix();

	// Straight track 3
	glPushMatrix();
	glRotated(-3, 0, 1, 0);
	glRotated(-13.7, 0, 0, 1);
	glRotated(180, 1, 0, 1);
	glTranslated(3.58, -11.31, 25.48);
	drw_straightTrack();
	glPopMatrix();
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
	m_camera.Set(100, 100, 30, 10, -10, 30, 0, 0, 0);
	Invalidate();
}


void CChildView::OnCameraCamera1()
{
	// TODO: Add your command handler code here
	camNum = 2;
	m_camera.Set(40, 20, 50, 12, 1, 0, 0, 1, 0);
	Invalidate();
}


void CChildView::OnCameraCamera2()
{
	// TODO: Add your command handler code here
	camNum = 3;
	m_camera.Set(70, 20, -40, 20, 5, 11, 0, 1, 0);
	Invalidate();
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


void CChildView::OnAnimationStart()
{
	// TODO: Add your command handler code here
	if (m_AnimTime == 0)
	{
		// Create the timer
		m_AnimTime = SetTimer(1, 30, NULL);
	}
	else
	{
		// Destroy the timer
		KillTimer(m_AnimTime);
		m_AnimTime = 0;
	}
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	Invalidate();

	m_time += 1.;
	m_positionX = acceleration * m_time * m_time / 2.;
	
	
	if (m_positionX < (12 * cos(14.04 * GR_PI / 180))) {
		m_speed += acceleration;
		m_changeX += (m_speed);
		m_changeY += (m_speed * tan(14.04 * GR_PI / 180));
		m_changeZ += (m_speed * tan(3. * GR_PI / 180));
		m_lineSpeed = m_speed;
	}
	else if (m_rotationY < 93.){
		m_rotationSpeed = m_speed * 180 / (3.5 * GR_PI);

		m_rotationY += m_rotationSpeed;

		m_changeAngleY += m_rotationSpeed;
		m_changeAngleX -= m_rotationSpeed * 14.04 / 93.;

		m_centripetalAccelation = m_lineSpeed * m_lineSpeed / 3.5;
		m_lineSpeed = m_lineSpeed - m_centripetalAccelation;
		m_speedZ += m_centripetalAccelation;
		
		m_changeX += (m_lineSpeed);
		m_changeY += (m_lineSpeed * tan(14.04 * GR_PI / 180) );
		m_changeZ -= (m_speed * tan(3. * GR_PI / 180) + m_speedZ);
	}
	else if (m_positionZ < 12) {
		m_speedZ = 0;
		m_time1 += 1.;
		m_positionZ = m_speed * m_time1;
		m_changeZ -= (m_speed);
		m_lineSpeed = m_speed;
	}
	else if(m_changeAngleY > 0.) {

		m_changeAngleY -= m_rotationSpeed;
		m_changeAngleX += m_rotationSpeed * 14.04 / 90.;

		m_lineSpeed = m_lineSpeed - m_centripetalAccelation;
		m_speedZ += m_centripetalAccelation;

		m_changeX += (m_speed * tan(3. * GR_PI / 180) + m_speedZ * 4);
		m_changeY += (m_speedZ * tan(14.04 * GR_PI / 180) * 4.);
		m_changeZ -= (m_lineSpeed);
	}
	else if (m_time2 < ( - m_speed + sqrt( m_speed * m_speed + 2 * acceleration * (14 * cos(14.04 * GR_PI / 180)))) / acceleration){
		m_time2 += 1.;
		m_speed += acceleration;
		m_changeX += (m_speed);
		m_changeY += (m_speed * tan(14.04 * GR_PI / 180));
		m_changeZ += (m_speed * tan(3. * GR_PI / 180));
	}
	

	COpenGLWnd::OnTimer(nIDEvent);
}


void CChildView::OnAnimationReset()
{
	m_time = 0.;
	m_time1 = 0.;
	m_time2 = 0.;
	m_changeX = 0;
	m_changeY = 0;
	m_changeZ = 0;
	m_speed = 0;
	m_rotationSpeed = 0;
	m_lineSpeed = 0;
	m_centripetalAccelation = 0;
	m_positionX = 0;
	m_positionZ = 0;
	m_rotationY = 0;
	m_changeAngleY = 0;
	m_changeAngleX = 0;
	m_speedZ = 0;
	m_changeZ = 0;
}
