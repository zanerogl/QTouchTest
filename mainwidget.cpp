#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    QImage img;
    img.load(":/res/cat0.jpg");
    ui->widget->setPicture(img);
}

MainWidget::~MainWidget()
{
    delete ui;
}
