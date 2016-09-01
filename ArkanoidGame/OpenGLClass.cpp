#include "OpenGLClass.h"
#include <iostream>

bool OpenGLClass::initialize(unsigned int _iScreenWidth, unsigned int _iScreenHeight, bool _bVSyncEnabled, bool _bFullscreen, float _fScreenDepth, float _fScreenNear, void * m_hwnd) {
    ReSizeGLScene(_iScreenWidth, _iScreenHeight);

    glShadeModel(GL_SMOOTH); // Enables Smooth Shading

    glClearColor(0.8f, 0.2f, 0.4f, 0.8f);
    glClearDepth(1.0f); // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    glDepthFunc(GL_LEQUAL); // The Type Of Depth Test To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective Calculations

#ifdef WINDOWS
    m_hdc = GetDC(reinterpret_cast<HWND>(m_hwnd)); // device context handle
#endif

    return true;
}

void OpenGLClass::setCamera(const Camera & _oCamera) {
    gluLookAt(-_oCamera.m_afPosition.x, -_oCamera.m_afPosition.y, -_oCamera.m_afPosition.z, _oCamera.m_afLookAt.x, _oCamera.m_afLookAt.y, _oCamera.m_afLookAt.z, _oCamera.m_afUp.x, _oCamera.m_afUp.y, _oCamera.m_afUp.z);
}

bool OpenGLClass::run() {

    return true;
}


bool OpenGLClass::render(const std::vector<GameObject*> * _scene, const std::vector<UIText*> * _ui) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
    glLoadIdentity();									// Reset The Current Modelview Matrix
    glTranslatef(-1.5f, 0.0f, -6.0f);						// Move Left 1.5 Units And Into The Screen 6.0
    
    glBegin(GL_TRIANGLES);								// Start Drawing A Triangle
    glColor3f(1.0f, 0.0f, 0.0f);						// Red
    glVertex3f(0.0f, 1.0f, 0.0f);					// Top Of Triangle (Front)
    glColor3f(0.0f, 1.0f, 0.0f);						// Green
    glVertex3f(-1.0f, -1.0f, 1.0f);					// Left Of Triangle (Front)
    glColor3f(0.0f, 0.0f, 1.0f);						// Blue
    glVertex3f(1.0f, -1.0f, 1.0f);					// Right Of Triangle (Front)
    glColor3f(1.0f, 0.0f, 0.0f);						// Red
    glVertex3f(0.0f, 1.0f, 0.0f);					// Top Of Triangle (Right)
    glColor3f(0.0f, 0.0f, 1.0f);						// Blue
    glVertex3f(1.0f, -1.0f, 1.0f);					// Left Of Triangle (Right)
    glColor3f(0.0f, 1.0f, 0.0f);						// Green
    glVertex3f(1.0f, -1.0f, -1.0f);					// Right Of Triangle (Right)
    glColor3f(1.0f, 0.0f, 0.0f);						// Red
    glVertex3f(0.0f, 1.0f, 0.0f);					// Top Of Triangle (Back)
    glColor3f(0.0f, 1.0f, 0.0f);						// Green
    glVertex3f(1.0f, -1.0f, -1.0f);					// Left Of Triangle (Back)
    glColor3f(0.0f, 0.0f, 1.0f);						// Blue
    glVertex3f(-1.0f, -1.0f, -1.0f);					// Right Of Triangle (Back)
    glColor3f(1.0f, 0.0f, 0.0f);						// Red
    glVertex3f(0.0f, 1.0f, 0.0f);					// Top Of Triangle (Left)
    glColor3f(0.0f, 0.0f, 1.0f);						// Blue
    glVertex3f(-1.0f, -1.0f, -1.0f);					// Left Of Triangle (Left)
    glColor3f(0.0f, 1.0f, 0.0f);						// Green
    glVertex3f(-1.0f, -1.0f, 1.0f);					// Right Of Triangle (Left)
    glEnd();											// Done Drawing The Pyramid

    glLoadIdentity();									// Reset The Current Modelview Matrix
    glTranslatef(1.5f, 0.0f, -7.0f);						// Move Right 1.5 Units And Into The Screen 7.0
    
    glBegin(GL_QUADS);									// Draw A Quad
    glColor3f(0.0f, 1.0f, 0.0f);						// Set The Color To Green
    glVertex3f(1.0f, 1.0f, -1.0f);					// Top Right Of The Quad (Top)
    glVertex3f(-1.0f, 1.0f, -1.0f);					// Top Left Of The Quad (Top)
    glVertex3f(-1.0f, 1.0f, 1.0f);					// Bottom Left Of The Quad (Top)
    glVertex3f(1.0f, 1.0f, 1.0f);					// Bottom Right Of The Quad (Top)
    glColor3f(1.0f, 0.5f, 0.0f);						// Set The Color To Orange
    glVertex3f(1.0f, -1.0f, 1.0f);					// Top Right Of The Quad (Bottom)
    glVertex3f(-1.0f, -1.0f, 1.0f);					// Top Left Of The Quad (Bottom)
    glVertex3f(-1.0f, -1.0f, -1.0f);					// Bottom Left Of The Quad (Bottom)
    glVertex3f(1.0f, -1.0f, -1.0f);					// Bottom Right Of The Quad (Bottom)
    glColor3f(1.0f, 0.0f, 0.0f);						// Set The Color To Red
    glVertex3f(1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Front)
    glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Front)
    glVertex3f(-1.0f, -1.0f, 1.0f);					// Bottom Left Of The Quad (Front)
    glVertex3f(1.0f, -1.0f, 1.0f);					// Bottom Right Of The Quad (Front)
    glColor3f(1.0f, 1.0f, 0.0f);						// Set The Color To Yellow
    glVertex3f(1.0f, -1.0f, -1.0f);					// Top Right Of The Quad (Back)
    glVertex3f(-1.0f, -1.0f, -1.0f);					// Top Left Of The Quad (Back)
    glVertex3f(-1.0f, 1.0f, -1.0f);					// Bottom Left Of The Quad (Back)
    glVertex3f(1.0f, 1.0f, -1.0f);					// Bottom Right Of The Quad (Back)
    glColor3f(0.0f, 0.0f, 1.0f);						// Set The Color To Blue
    glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Left)
    glVertex3f(-1.0f, 1.0f, -1.0f);					// Top Left Of The Quad (Left)
    glVertex3f(-1.0f, -1.0f, -1.0f);					// Bottom Left Of The Quad (Left)
    glVertex3f(-1.0f, -1.0f, 1.0f);					// Bottom Right Of The Quad (Left)
    glColor3f(1.0f, 0.0f, 1.0f);						// Set The Color To Violet
    glVertex3f(1.0f, 1.0f, -1.0f);					// Top Right Of The Quad (Right)
    glVertex3f(1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Right)
    glVertex3f(1.0f, -1.0f, 1.0f);					// Bottom Left Of The Quad (Right)
    glVertex3f(1.0f, -1.0f, -1.0f);					// Bottom Right Of The Quad (Right)
    glEnd();											// Done Drawing The Quad

#ifdef WINDOWS
    SwapBuffers(m_hdc);
#endif // WINDOWS

    return true;
}

bool OpenGLClass::shutdown() {
    return true;
}

void * OpenGLClass::getHandle() {
    return nullptr;
}

GraphicsInterface * OpenGLClass::getInstance() {
    if (OpenGLClass::instance == nullptr) {
        OpenGLClass::instance = new OpenGLClass();
    }
    return OpenGLClass::instance;
}

OpenGLClass::OpenGLClass() {}

void OpenGLClass::ReSizeGLScene(unsigned int _iScreenWidth, unsigned int _iScreenHeight) {
    if (_iScreenHeight <= 0)
        _iScreenHeight = 1;

    float aspectRatio = _iScreenWidth / _iScreenHeight;

    glViewport(0, 0, _iScreenWidth, _iScreenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspectRatio, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
}

bool OpenGLClass::loadMesh(GameObject * _object) {

    std::vector<Vector3> out_vertices;

    for (unsigned int i = 0; i < _object->m_oMesh->m_iIndexCount; i++) {
        unsigned int vertexIndex = _object->m_oMesh->m_alIndices[i];
        Vector3 vertex = _object->m_oMesh->m_aoVertices[vertexIndex - 1].position;
        out_vertices.push_back(vertex);
    }

    //glBufferData(GL_ARRAY_BUFFER, _object->m_oMesh->m_iVertexCount * sizeof(Vector3), &out_vertices[0], GL_STATIC_DRAW); TODO use Glew
    return true;
}
