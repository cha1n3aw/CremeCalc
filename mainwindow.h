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
    void on_undo_clicked();
    void on_redo_clicked();
    void on_degrees_clicked();
    void on_theme_clicked();

private:
    Ui::MainWindow *ui;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void result();
    void stringremover();
    void errorwindow();
};

#endif // MAINWINDOW_H
