/**
*******************************************************************************
*  @file       MainWindow.cpp
*
*  @brief      Class that describes a control panel for the application
*******************************************************************************
*/

//******************************************************************************
//  Include
//******************************************************************************
#include <QFileDialog>
#include <iostream>
#include <exception>

#include "MainWindow.h"
#include "ui_mainwindow.h"

//------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent):
	QMainWindow(parent),
	ui(new Ui::MainWindow)
//------------------------------------------------------------------------------
{
	ui->setupUi(this);

	setFixedSize(geometry().width(), geometry().height());

	//red error message
	ui->errorText->setTextColor(QColor(255, 0, 0));
	ui->imageFileText->setText(m_imageFile.c_str());

	updateImageProcessor();

	setWindowTitle("Control panel");
}

//------------------------------------------------------------------------------
MainWindow::~MainWindow()
//------------------------------------------------------------------------------
{
	delete ui;
}

//------------------------------------------------------------------------------
void MainWindow::on_originalImageButton_clicked()
//------------------------------------------------------------------------------
{
	launchRenderWindow("Original image", m_imageProcessor.getRawData());
}

//------------------------------------------------------------------------------
void MainWindow::on_smoothedImageButton_clicked()
//------------------------------------------------------------------------------
{
	launchRenderWindow("Smoothed image", m_imageProcessor.getSmoothedData());
}

//------------------------------------------------------------------------------
void MainWindow::on_gradientNormButton_clicked()
//------------------------------------------------------------------------------
{
	launchRenderWindow("Gradient norm image", m_imageProcessor.getGradientData());
}

//------------------------------------------------------------------------------
void MainWindow::on_cannyImageButton_clicked()
//------------------------------------------------------------------------------
{
	launchRenderWindow("Cany image", m_imageProcessor.getCannyData());
}

//------------------------------------------------------------------------------
void MainWindow::on_choseImageButton_clicked()
//------------------------------------------------------------------------------
{
	//Chose the name and directory of the file
	QString fileName = QFileDialog::getOpenFileName(nullptr, "Open image file",
                               QCoreApplication::applicationDirPath() + "/resources/data/",
                               "Images (*.png *.xpm *.jpg)");

	if(fileName.size())
	{
		//Convert name into string
		m_imageFile = fileName.toUtf8().constData();

		ui->imageFileText->setText(fileName);

		updateImageProcessor();
	}
}



//------------------------------------------------------------------------------
void MainWindow::on_useIndexButton_clicked()
//------------------------------------------------------------------------------
{
    if(m_useIndex)
    {
        m_useIndex = false;
		ui->useIndexButton->setText("Use index");
    }
	else
	{
		m_useIndex = true;
		ui->useIndexButton->setText("do not use index");
	}
}


//------------------------------------------------------------------------------
void MainWindow::launchRenderWindow(QString const& windowName, const Types::float_matrix &imageData)
//------------------------------------------------------------------------------
{
	//set the size of the depth buffer
	QSurfaceFormat format;
	format.setDepthBufferSize(24);

	//Use regular pointer to avoid deletion after the end of the function
	try
	{
		RenderWindow *renderWindow(new RenderWindow(imageData,
								m_imageProcessor.getN(), m_imageProcessor.getM(),
								m_useIndex));

		renderWindow->setFormat(format);
		renderWindow->setTitle(windowName);
		renderWindow->resize(800, 450);
		renderWindow->show();
	}
	catch(std::exception const& e)
	{
		//display the error message
		ui->errorText->setText(e.what());
		std::cerr << "ERROR : " << e.what() << std::endl;
	}
}

//------------------------------------------------------------------------------
void MainWindow::updateImageProcessor()
//------------------------------------------------------------------------------
{
	//Disable buttons before processing
	ui->originalImageButton->setDisabled(true);
	ui->smoothedImageButton->setDisabled(true);
	ui->gradientNormButton->setDisabled(true);
	ui->cannyImageButton->setDisabled(true);

	try
	{
		//launch processing
		m_imageProcessor = ImageProcessor(m_imageFile);

		//Enable buttons if everything has gone well
		ui->originalImageButton->setEnabled(true);
		ui->smoothedImageButton->setEnabled(true);
		ui->gradientNormButton->setEnabled(true);
		ui->cannyImageButton->setEnabled(true);

		ui->errorText->clear();
	}
	catch(std::exception const& e)
	{
		//display the error message
		ui->errorText->setText(e.what());
		std::cerr << "ERROR : " << e.what() << std::endl;
	}


}
