#include "myqframe.h"
#include "ui_myqframe.h"

MyQFrame::MyQFrame(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyQFrame)
{
    ui->setupUi(this);
}

MyQFrame::~MyQFrame()
{
    delete ui;
}

