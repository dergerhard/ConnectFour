#include "connectfourliebmann.h"

ConnectFourLiebmann::ConnectFourLiebmann()
{
    glWidget = new ConnectFourWidget();

    xSlider = createSlider();
    ySlider = createSlider();
    zSlider = createSlider();
    btDo = new QPushButton();
    btDo->setText("Do it");
    btDo->setMaximumWidth(100);
    btDo->setStyleSheet("QPushButton{background-color:black; color:white; border:1px solid white;}");

    connect(btDo, SIGNAL(clicked()), this, SLOT(onBtDoClicked()));
    connect(xSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setXRotation(int)));
    connect(glWidget, SIGNAL(xRotationChanged(int)), xSlider, SLOT(setValue(int)));
    connect(ySlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setYRotation(int)));
    connect(glWidget, SIGNAL(yRotationChanged(int)), ySlider, SLOT(setValue(int)));
    connect(zSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setZRotation(int)));
    connect(glWidget, SIGNAL(zRotationChanged(int)), zSlider, SLOT(setValue(int)));


    mainLayout = new QHBoxLayout;
    this->setStyleSheet("QWidget{background-color:black; color:white;}");


    mainLayout->addWidget(glWidget);
    mainLayout->addWidget(xSlider);
    mainLayout->addWidget(ySlider);
    mainLayout->addWidget(zSlider);

    QVBoxLayout *sideLayout = new QVBoxLayout;
    edX = new QLineEdit;
    edY = new QLineEdit;
    edZ = new QLineEdit;
    edX->setMaximumWidth(100);
    edY->setMaximumWidth(100);
    edZ->setMaximumWidth(100);

    sideLayout->addWidget(edX);
    sideLayout->addWidget(edY);
    sideLayout->addWidget(edZ);
    sideLayout->addWidget(btDo);
    //mainLayout->addWidget(sideLayout);
    mainLayout->addLayout(sideLayout);
    setLayout(mainLayout);

    xSlider->setValue(15 * 16);
    ySlider->setValue(345 * 16);
    zSlider->setValue(0 * 16);
    setWindowTitle(tr("Connect Four Liebmann"));
}


ConnectFourLiebmann::~ConnectFourLiebmann()
{
    
}

QSlider *ConnectFourLiebmann::createSlider()
{
    QSlider *slider = new QSlider(Qt::Vertical);
    slider->setRange(0, 360 * 16);
    slider->setSingleStep(16);
    slider->setPageStep(15 * 16);
    slider->setTickInterval(15 * 16);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}


void ConnectFourLiebmann::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}


void ConnectFourLiebmann::onBtDoClicked()
{
    std::cout << "btDo" << std::endl;

    glWidget->setCube(edX->text().toInt(),
                      edY->text().toInt(),
                      edZ->text().toInt(), PlayerA);



}

