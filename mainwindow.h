#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtGui>
#include <QMainWindow>
#include <ctype.h>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void write(QString expression);

private slots:

    void buttons();
    void nondefaultop();

    void on_indicator_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void result();
    void errorwindow();
};

#endif // MAINWINDOW_H
