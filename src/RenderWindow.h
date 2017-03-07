#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

/**
*******************************************************************************
*
*  @file       RenderWindow.h
*
*  @brief      Class to display the height map
*
*  @version    1.0
*
*  @date       17/06/2016
*
*  @author     Andr�as Meuleman
*******************************************************************************
*/

//******************************************************************************
//  Include
//******************************************************************************
#include <QtGui/QOpenGLWindow>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QVector3D>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <string>
#include <vector>
#include <QKeyEvent>

#include "HeightMapMesh.h"
#include "DepthMap.h"
#include "LvlPlan.h"

//******************************************************************************
//  namespace
//******************************************************************************
using namespace std;

//==============================================================================
/**
*  @class  RenderWindow
*  @brief  RenderWindow is a class to render to the screen
*/
//==============================================================================
class RenderWindow : public QOpenGLWindow, protected QOpenGLFunctions {

public:
    //--------------------------------------------------------------------------
    /// Overloaded constructor with the name of the file. The file has to contain
    /// the width, the height and then the data in the [0,1] range
    /**
    *  @param fileName: the name of the height map file
    */
    //--------------------------------------------------------------------------
    RenderWindow(string const& fileName);

    //--------------------------------------------------------------------------
    /// Overloaded constructor with the image size and data
    /**
    *  @param imageData: the data of the image as floats in the [0,1] range
    *  @param n: height of the image
    *  @param m: width of the image
    */
    //--------------------------------------------------------------------------
    RenderWindow(vector<vector<float>> const& imageData,
                 unsigned int n, unsigned int m);

	virtual ~RenderWindow();

	//--------------------------------------------------------------------------
	/// Initialize the programs and the buffers
	//--------------------------------------------------------------------------
    virtual void initializeGL();

	//--------------------------------------------------------------------------
	/// display on the window
	//--------------------------------------------------------------------------
    virtual void paintGL();
	
	//--------------------------------------------------------------------------
	/// set the visibility of the lvl plan to false if it was true and vice versa
	//--------------------------------------------------------------------------	
	void changeLvlPlanVisibility();

//******************************************************************************
private:
	//No copy constructor
	RenderWindow(RenderWindow const&);

	//No default constructor
	RenderWindow();

    //--------------------------------------------------------------------------
    ///Zoom in or zoom out
    //--------------------------------------------------------------------------
    void wheelEvent(QWheelEvent *wheelEvent);

	//--------------------------------------------------------------------------
	///event to control the display 
    ///rotation of the camera thanks to Z, Q, S, D,
	///Enable/disable lvl plan display: spacebarre
    ///change the height of the lvl plan: R and F
	///Rotation of the light source: arrows
	//--------------------------------------------------------------------------
	void keyPressEvent(QKeyEvent *event);

    //--------------------------------------------------------------------------
    ///Rotate the camera
    //--------------------------------------------------------------------------
    void rotateCamera(float angle, float x, float y, float z);

	//--------------------------------------------------------------------------
	///Rotate the light source and rebuild the shadow
	//--------------------------------------------------------------------------
	void rotateLightSource(float angle, float x, float y, float z);

	//--------------------------------------------------------------------------
	///read the shader file to crate a program
	/**
	*  @param shaderFileName: the name of the shader file to load
	*/
	//--------------------------------------------------------------------------
	static string readShaderFile(string const& shaderFileName);

    //Height map to display
    HeightMapMesh m_heightMapMesh;

    //lvl plan to display if needed
    LvlPlan m_lvlPlan;

	//IDs for inputs in the ground display program
    GLuint m_lightDirID, //ID of the light direction vector (from the vertex to the light)
		m_mvpMatrixID, //ID of the Model view position matrix
		m_cameraPosID, //ID of the position of the camera
		m_shadowMapDisplayMatrixID, //ID of the projection matrix of the shadow map 
		m_shadowMapTextureID; //ID of the texture of the shadow map

	//IDs for inputs in the lvl plan display program
	GLuint m_verticesLvlPlanPositionID,//ID of the position of the vertex
		m_mvpLvlPlanMatrixID;//ID of the Model view position matrix


	QOpenGLShaderProgram *m_displayProgram, // The render program displaying on the screen
        *m_lvlProgram, //Program to display the lvl plan
        *m_depthMapProgram;//the program to create the shadow map

	DepthMap m_shadowMap; //object containing buffers and program to create the shadow map

    unsigned int m_dataCount; //number of vertices

    QVector3D m_lightDir;//the direction of the light (from the vertex to the light)
    QVector3D m_eyePos;//the position of the camera

	QMatrix4x4 m_shadowMapMatrix,	//the  matrix from the light's point of view to create the shadow map  
		m_pMatrix, //the projection matrix for the camera to display on the window
		m_vMatrix, //the view matrix
		m_mMatrix; //the model matrix

    float m_length, //length of the model
        m_width, //width of the model
        m_shadowMatrixSide, //size of the cube that the shadow map take into account
        m_zoomAngle;

	bool m_LvlPlanVisibility; //to chose if the lvl plan has to be displayed
};

#endif //RENDERWINDOW_H