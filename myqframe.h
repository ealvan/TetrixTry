#ifndef MYQFRAME_H
#define MYQFRAME_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MyQFrame; }
QT_END_NAMESPACE

class MyQFrame : public QMainWindow
{
    Q_OBJECT

public:
    MyQFrame(QWidget *parent = nullptr);
    ~MyQFrame();

private:
    Ui::MyQFrame *ui;
};
#endif // MYQFRAME_H
