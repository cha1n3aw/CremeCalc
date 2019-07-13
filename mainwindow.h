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
    void erase();

private slots:

    void on_bt1_clicked();

    void on_bt2_clicked();

    void on_bt3_clicked();

    void on_bt4_clicked();

    void on_bt5_clicked();

    void on_bt6_clicked();

    void on_bt7_clicked();

    void on_bt8_clicked();

    void on_bt9_clicked();

    void on_bt0_clicked();

    void on_btdot_clicked();

    void on_btpl_clicked();

    void on_btmin_clicked();

    void on_btmult_clicked();

    void on_btdiv_clicked();

    void on_btpow_clicked();

    void on_btroot_clicked();

    void on_btp_clicked();

    void on_bte_clicked();

    void on_bteq_clicked();

    void on_btclr_clicked();

    void on_btdel_clicked();

    void on_btms_clicked();

    void on_btmr_clicked();

    void on_btmc_clicked();

    void on_btopen_clicked();

    void on_btclose_clicked();

    void on_btlog_clicked();

    void on_btln_clicked();

    void on_btsin_clicked();

    void on_btcos_clicked();

    void on_bttan_clicked();

    void on_btctan_clicked();

    void on_sqroot_clicked();

private:
    Ui::MainWindow *ui;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void result();
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void drawroot(int xs, int xe);
    void write(QString expression);
};

#endif // MAINWINDOW_H
