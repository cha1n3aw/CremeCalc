#include "mainwindow.h"
#include "errorwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QStack>
#include <string>
#include <cmath>
#include <vector>
#include <QKeyEvent>
#include <QLabel>
#include <QDebug>

using namespace std;

double calculation(double* digitbuffer, char* opbuffer, int digitstacksize, int opstacksize); //прототип функции для вычисления
double brackets(double* digitbuffer, char* opbuffer, int digitstacksize, int opstacksize); //прототип функции для выделения выражения внутри скобок
double stacktobuffer(); //прототип функции вычислений
void firewall(string expression);
void stringremover(); //прототип функции чистки строки от пробелов
void string_parsing(string); //прототип функции парсинга стринга
void buttons();
void write(QString expression);
void errorwindow();
static QStack <double> digitstack; //стек чисел
static QStack <char> opstack; //стек операторов
static QString buffer;
static vector <string> history;
static vector <string> texthistory;
static bool powstatus = false;
static bool error = false;
static bool bracketstatus = false;
static bool radians = false;
static bool berrytheme = false;
static double savedresult = 0;
static unsigned long long undostep = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    setWindowTitle("berry&creme calc");

    connect(ui->bt0, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->bt1, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->bt2, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->bt3, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->bt4, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->bt5, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->bt6, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->bt7, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->bt8, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->bt9, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->btdot, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->btpl, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->btmin, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->btmult, SIGNAL(clicked()), this, SLOT(nondefaultop()));
    connect(ui->btdiv, SIGNAL(clicked()), this, SLOT(nondefaultop()));
    connect(ui->btpow, SIGNAL(clicked()), this, SLOT(nondefaultop()));
    connect(ui->btdel, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->btp, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->bte, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->btopen, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->btclose, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->btlg, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->btln, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->btsin, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->btcos, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->bttan, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->btctan, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->btclr, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->bteq, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->btroot, SIGNAL(clicked()), this, SLOT(nondefaultop()));
    connect(ui->bt00, SIGNAL(clicked()), this, SLOT(buttons()));
    connect(ui->btlog, SIGNAL(clicked()), this, SLOT(nondefaultop()));
    connect(ui->sqroot, SIGNAL(clicked()), this, SLOT(nondefaultop()));
    connect(ui->btms, SIGNAL(clicked()), this, SLOT(nondefaultop()));
    connect(ui->btmr, SIGNAL(clicked()), this, SLOT(nondefaultop()));
    connect(ui->btmc, SIGNAL(clicked()), this, SLOT(nondefaultop()));
    connect(ui->btmpl, SIGNAL(clicked()), this, SLOT(nondefaultop()));
    history.push_back("");
    texthistory.push_back("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_theme_clicked()
{
    QString creme_label = "QLabel{background-color:transparent;color:#91835a;font-family:Trebuchet MS;font-size:16px;font-weight:bold;"
"text-decoration:none;text-shadow:0px 1px 0px #2d2e29;}";

    QString creme_expression = "QLabel{border: 3px solid #91835a;border-radius: 18px;"
"color: #b8ab85;font-family:Trebuchet MS;font-size:18px;font-weight:bold;background: transparent;padding-left: 10px;padding-top: 1px;}";

    QString creme_mainbuttons = "QPushButton{background-color:#91835a;border-radius:16px;border:2px solid #333029;"
"color:#25261f;font-family:Trebuchet MS;font-size:16px;font-weight:bold;}QPushButton:hover{background-color:#a89a6f;}"
"QPushButton:pressed{background-color: #ccc2a6;}";

    QString creme_opbuttons = "QPushButton{background-color:#b8ab85;border-radius:18px;border:2px solid #333029;color:#505739;"
"font-family:Trebuchet MS;font-size:16px;font-weight:bold;}QPushButton:hover{background-color:#a89a6f;}"
"QPushButton:pressed{background-color: #ccc2a6;}";
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    QString berry_label = "QLabel{background-color:transparent;color:#191136;font-family:Trebuchet MS;font-size:16px;"
"font-weight:bold;}";

    QString berry_expression = "QLabel{border: 3px solid #a83151;border-radius: 18px;"
"color: #191136;font-family:Trebuchet MS;font-size:18px;font-weight:bold;background: transparent;padding-left: 10px;padding-top: 1px;}";

    QString berry_mainbuttons = "QPushButton{background-color:#a83151;border-radius:16px;border:2px solid #fae9d9;"
"color:#fae9d9;font-family:Trebuchet MS;font-size:16px;font-weight:bold;}QPushButton:hover{background-color:#e887a0;}"
"QPushButton:pressed{background-color: #cccccc;}"; //#d5e38c

    QString berry_opbuttons = "QPushButton{background-color:#191136;border-radius:18px;border:2px solid #fae9d9;color:#fae9d9;"
"font-family:Trebuchet MS;font-size:16px;font-weight:bold;}QPushButton:hover{background-color:#cbe9eb;}"
"QPushButton:pressed{background-color: #cccccc;}"; //#a0dadb

    if (!berrytheme)
    {
        setStyleSheet("background-color:#fae9d9;border:0px solid #fae9d9;");
        ui->theme->setText("berry"); ui->expression->setStyleSheet(berry_expression);
        ui->history_1->setStyleSheet(berry_label); ui->history_2->setStyleSheet(berry_label);
        ui->history_3->setStyleSheet(berry_label); ui->history_4->setStyleSheet(berry_label);
        ui->history_5->setStyleSheet(berry_label); ui->degrees->setStyleSheet(berry_mainbuttons);
        ui->history_6->setStyleSheet(berry_label); ui->history_7->setStyleSheet(berry_label);
        ui->btdel->setStyleSheet(berry_mainbuttons); ui->bttan->setStyleSheet(berry_mainbuttons);
        ui->btctan->setStyleSheet(berry_mainbuttons); ui->btsin->setStyleSheet(berry_mainbuttons);
        ui->btcos->setStyleSheet(berry_mainbuttons); ui->btms->setStyleSheet(berry_mainbuttons);
        ui->btmr->setStyleSheet(berry_mainbuttons); ui->btmc->setStyleSheet(berry_mainbuttons);
        ui->btclr->setStyleSheet(berry_mainbuttons); ui->bteq->setStyleSheet(berry_mainbuttons);
        ui->bt0->setStyleSheet(berry_opbuttons); ui->bt00->setStyleSheet(berry_opbuttons);
        ui->bt1->setStyleSheet(berry_opbuttons); ui->bt2->setStyleSheet(berry_opbuttons);
        ui->bt3->setStyleSheet(berry_opbuttons); ui->bt4->setStyleSheet(berry_opbuttons);
        ui->bt5->setStyleSheet(berry_opbuttons); ui->bt6->setStyleSheet(berry_opbuttons);
        ui->bt7->setStyleSheet(berry_opbuttons); ui->bt8->setStyleSheet(berry_opbuttons);
        ui->bt9->setStyleSheet(berry_opbuttons); ui->btpl->setStyleSheet(berry_opbuttons);
        ui->btmin->setStyleSheet(berry_opbuttons); ui->btmult->setStyleSheet(berry_opbuttons);
        ui->btdiv->setStyleSheet(berry_opbuttons); ui->btpow->setStyleSheet(berry_opbuttons);
        ui->btroot->setStyleSheet(berry_opbuttons); ui->sqroot->setStyleSheet(berry_opbuttons);
        ui->btdot->setStyleSheet(berry_opbuttons); ui->btopen->setStyleSheet(berry_opbuttons);
        ui->btclose->setStyleSheet(berry_opbuttons); ui->bte->setStyleSheet(berry_opbuttons);
        ui->btp->setStyleSheet(berry_opbuttons); ui->btlog->setStyleSheet(berry_opbuttons);
        ui->btlg->setStyleSheet(berry_opbuttons); ui->btln->setStyleSheet(berry_opbuttons);
        ui->undo->setStyleSheet(berry_mainbuttons); ui->redo->setStyleSheet(berry_mainbuttons);
        ui->theme->setStyleSheet(berry_mainbuttons);
        berrytheme = true;
    }
    else
    {
        setStyleSheet("background-color:#333333;border:0px solid #333333;");
        ui->theme->setText("creme"); ui->expression->setStyleSheet(creme_expression);
        ui->history_1->setStyleSheet(creme_label); ui->history_2->setStyleSheet(creme_label);
        ui->history_3->setStyleSheet(creme_label); ui->history_4->setStyleSheet(creme_label);
        ui->history_5->setStyleSheet(creme_label); ui->degrees->setStyleSheet(creme_mainbuttons);
        ui->history_6->setStyleSheet(creme_label); ui->history_7->setStyleSheet(creme_mainbuttons);
        ui->btdel->setStyleSheet(creme_mainbuttons); ui->bttan->setStyleSheet(creme_mainbuttons);
        ui->btctan->setStyleSheet(creme_mainbuttons); ui->btsin->setStyleSheet(creme_mainbuttons);
        ui->btcos->setStyleSheet(creme_mainbuttons); ui->btms->setStyleSheet(creme_mainbuttons);
        ui->btmr->setStyleSheet(creme_mainbuttons); ui->btmc->setStyleSheet(creme_mainbuttons);
        ui->btclr->setStyleSheet(creme_mainbuttons); ui->bteq->setStyleSheet(creme_mainbuttons);
        ui->bt0->setStyleSheet(creme_opbuttons); ui->bt00->setStyleSheet(creme_opbuttons);
        ui->bt1->setStyleSheet(creme_opbuttons); ui->bt2->setStyleSheet(creme_opbuttons);
        ui->bt3->setStyleSheet(creme_opbuttons); ui->bt4->setStyleSheet(creme_opbuttons);
        ui->bt5->setStyleSheet(creme_opbuttons); ui->bt6->setStyleSheet(creme_opbuttons);
        ui->bt7->setStyleSheet(creme_opbuttons); ui->bt8->setStyleSheet(creme_opbuttons);
        ui->bt9->setStyleSheet(creme_opbuttons); ui->btpl->setStyleSheet(creme_opbuttons);
        ui->btmin->setStyleSheet(creme_opbuttons); ui->btmult->setStyleSheet(creme_opbuttons);
        ui->btdiv->setStyleSheet(creme_opbuttons); ui->btpow->setStyleSheet(creme_opbuttons);
        ui->btroot->setStyleSheet(creme_opbuttons); ui->sqroot->setStyleSheet(creme_opbuttons);
        ui->btdot->setStyleSheet(creme_opbuttons); ui->btopen->setStyleSheet(creme_opbuttons);
        ui->btclose->setStyleSheet(creme_opbuttons); ui->bte->setStyleSheet(creme_opbuttons);
        ui->btp->setStyleSheet(creme_opbuttons); ui->btlog->setStyleSheet(creme_opbuttons);
        ui->btlg->setStyleSheet(creme_opbuttons); ui->btln->setStyleSheet(creme_opbuttons);
        ui->undo->setStyleSheet(creme_mainbuttons); ui->redo->setStyleSheet(creme_mainbuttons);
        ui->theme->setStyleSheet(creme_mainbuttons);
        berrytheme = false;
    }
}


void MainWindow::buttons()
{
    QPushButton *button = static_cast<QPushButton*>(sender());
    write(button->text());
}

void MainWindow::nondefaultop()
{
    QPushButton *button = static_cast<QPushButton*>(sender());
    QString op = button->text();
    if (op == "÷") write("/");
    if (op == "×") write("*");
    if (op == "²√")
    {
        write("2");
        powstatus = false;
        write("r");
    }
    if (op == "√") write("r");
    if (op == "log") write("g");
    if (op == "x^ʸ") write("^");
    if (op == "MS")
    {
        int i = buffer.length() - 1;
        QString temp;
        while (i >= 0 && buffer[i] != '=')
        {
            temp[buffer.length() - 1 - i] = buffer[i];
            i--;
        }
        QString temp1;
        temp1.resize(temp.length());
        for (i = 0; i < temp.length(); i++)
        {
            temp1[temp1.length() - 1 - i] = temp[i];
        }
        savedresult = stod(temp1.toLocal8Bit().constData());
    }
    if (op == "M+")
    {
        int i = buffer.length() - 1;
        QString temp;
        while (i >= 0 && buffer[i] != '=')
        {
            temp[buffer.length() - 1 - i] = buffer[i];
            i--;
        }
        QString temp1;
        temp1.resize(temp.length());
        for (i = 0; i < temp.length(); i++)
        {
            temp1[temp1.length() - 1 - i] = temp[i];
        }
        savedresult += stod(temp1.toLocal8Bit().constData());
    }
    if (op == "MR")
    {
        QString temp = QString::fromStdString(to_string(savedresult));
        int i=temp.length() - 1;
        while (temp[i] == '0' && i>0)
        {
           if (temp[i-1] == '.') temp.resize(i-1);
            else temp.resize(i);
           i--;
        }
        write(temp);
    }
    if (op == "MC") savedresult = 0;
}

void MainWindow::errorwindow()
{
    ui->expression->setText("");
    buffer = "";
    ErrorWindow errorwindow;
    errorwindow.setModal(true);
    errorwindow.exec();
}

void MainWindow::result ()
{
    error = false;
    QString result;
    if (buffer.length() != 0)
    {
        stringremover();
        string expression = buffer.toLocal8Bit().constData();
        firewall(expression);
        if (!error)
        {
                string_parsing(expression);
                result = QString::fromStdString(to_string(stacktobuffer()));
                if(!error)
                {
                    int i=result.length() - 1;
                    while (result[i] == '0' && i>0)
                    {
                       if (result[i-1] == '.') result.resize(i-1);
                        else result.resize(i);
                       i--;
                    }
                    buffer = buffer + "=" + result;
                    ui->expression->setText(ui->expression->text() + "=" + result);
                    if (undostep != history.size() - 1)
                    {
                        history.resize(history.size() - (history.size() - undostep - 1));
                        texthistory.resize(texthistory.size() - (texthistory.size() - undostep - 1));
                    }
                    history.push_back(buffer.toLocal8Bit().constData());
                    texthistory.push_back(ui->expression->text().toLocal8Bit().constData());
                    undostep++;
                    ui->history_7->setText(ui->history_6->text());
                    ui->history_6->setText(ui->history_5->text());
                    ui->history_5->setText(ui->history_4->text());
                    ui->history_4->setText(ui->history_3->text());
                    ui->history_3->setText(ui->history_2->text());
                    ui->history_2->setText(ui->history_1->text());
                    ui->history_1->setText(ui->expression->text());
                }
                else errorwindow();
        }
        else errorwindow();
    }
}

void MainWindow::on_degrees_clicked()
{
    if (!radians)
    {
        radians = true;
        ui->degrees->setText("radians");
    }
    else
    {
        ui->degrees->setText("degrees");
        radians = false;
    }
}

void MainWindow::on_undo_clicked()
{
    if (undostep > 0 && history.size() > 0)
    {
        undostep--;
        buffer = QString::fromStdString(history[undostep]);
        ui->expression->setText(QString::fromStdString(texthistory[undostep]));
    }
}

void MainWindow::on_redo_clicked()
{
    if (history.size() > undostep + 1)
    {
        undostep++;
        buffer = QString::fromStdString(history[undostep]);
        ui->expression->setText(QString::fromStdString(texthistory[undostep]));
    }
}

void MainWindow::write(QString action)
{
    switch (action.toLocal8Bit().constData()[0])
    {
        case '-': powstatus = false; break;
        case '+': powstatus = false; break;
        case '/': powstatus = false; break;
        case '*': powstatus = false; break;
        case 'C':
        {
            buffer = "";
            ui->expression->setText("");
            history.push_back("");
            texthistory.push_back("");
            undostep++;
            powstatus = false;
            break;
        }
        case '^': if (buffer.length() > 0) powstatus = true; break;
        case '=': result(); break;
        case 'r':
        {
            if (buffer.length() > 0 && buffer[buffer.length() - 1] != 'r' && buffer[buffer.length() - 1] != '^' && buffer[buffer.length() - 1] != 'g')
            {
                QString text;
                QString temp = ui->expression->text();
                int i = buffer.length() - 1;
                QString text1;
                if(buffer[i] == ')')
                {
                    while (i >= 0 && buffer[i + 1] != '(')
                        {
                            text += buffer[i];
                            temp.resize(temp.length() - 1);
                            i--;
                        }
                }
                else
                {
                    while (i >= 0
                           && buffer[i] != '+'
                           && buffer[i] != '-'
                           && buffer[i] != '*'
                           && buffer[i] != '/'
                           && buffer[i] != '^'
                           && buffer[i] != '=')
                        {
                            text += buffer[i];
                            temp.resize(temp.length() - 1);
                            i--;
                        }
                }
                text1.resize(text.length());
                for (i = 0; i < text.length(); i++)
                {
                    text1[text1.length() - 1 - i] = text[i];
                }
                ui->expression->setText(temp + "<sup>" + text1 + "</sup>");
                buffer = buffer + "r";
                ui->expression->setText(ui->expression->text() + "√");
                break;
            }
            else break;
        }
        case 'g':
        {
            if (buffer.length() > 0 && buffer[buffer.length() - 1] != 'r' && buffer[buffer.length() - 1] != '^' && buffer[buffer.length() - 1] != 'g')
            {
                QString text;
                QString temp = ui->expression->text();
                int i = buffer.length() - 1;
                QString text1;
                if(buffer[i] == ')')
                {
                    while (i >= 0 && buffer[i + 1] != '(')
                        {
                            text += buffer[i];
                            temp.resize(temp.length() - 1);
                            i--;
                        }
                }
                else
                {
                    while (i >= 0
                           && buffer[i] != '+'
                           && buffer[i] != '-'
                           && buffer[i] != '*'
                           && buffer[i] != '/'
                           && buffer[i] != '^'
                           && buffer[i] != '='
                           && buffer[i] != '(')
                        {
                            text += buffer[i];
                            temp.resize(temp.length() - 1);
                            i--;
                        }
                }
                text1.resize(text.length());
                for (i = 0; i < text.length(); i++)
                {
                    text1[text1.length() - 1 - i] = text[i];
                }
                ui->expression->setText(temp + "log<sub>" + text1 + "</sub>");
                buffer = buffer + "log";
                break;
            }
        }
    }
    if (action != "=" && action != "r" && action != "g" && action != "C")
    {
        if (action == "⌫")
        {

            QString text = ui->expression->text();
            if (buffer.length() >= 3 && (
               (buffer[buffer.length() - 1] == 'n'
             && buffer[buffer.length() - 2] == 'i'
             && buffer[buffer.length() - 3] == 's')
            || (buffer[buffer.length() - 1] == 's'
             && buffer[buffer.length() - 2] == 'o'
             && buffer[buffer.length() - 3] == 'c')
            || (buffer[buffer.length() - 1] == 'g'
             && buffer[buffer.length() - 2] == 't'
             && buffer[buffer.length() - 3] == 'c')
            || (buffer[buffer.length() - 1] == 'p'
             && buffer[buffer.length() - 2] == 'x'
             && buffer[buffer.length() - 3] == 'e')))
            {
                buffer.resize(buffer.length() - 3);
                text.resize(text.length() - 3);
                ui->expression->setText(text);
            }
            else
            {
                if(buffer.length() >= 2 && (
                   (buffer[buffer.length() - 1] == 'g'
                 && buffer[buffer.length() - 2] == 't')
                || (buffer[buffer.length() - 1] == 'g'
                 && buffer[buffer.length() - 2] == 'l')
                || (buffer[buffer.length() - 1] == 'n'
                 && buffer[buffer.length() - 2] == 'l')
                || (buffer[buffer.length() - 1] == 'i'
                 && buffer[buffer.length() - 2] == 'p')))
                {
                    buffer.resize(buffer.length() - 2);
                    text.resize(text.length() - 2);
                    ui->expression->setText(text);
                }
                else
                {
                    if(buffer.length() >= 3 && buffer[buffer.length() - 1] == 'g' && buffer[buffer.length() - 2] == 'o' && buffer[buffer.length() - 3] == 'l')
                    {
                        QString number;
                        QString tempnumber;
                        text.resize(text.length() - 6);
                        while (text[text.length() - 1] != '>')
                        {
                            number += text[text.length() - 1];
                            text.resize(text.length() - 1);
                        }
                        text.resize(text.length() - 8);
                        tempnumber.resize(number.length());
                        for (int i = 0; i < number.length(); i++)
                        {
                            tempnumber[tempnumber.length() - 1 - i] = number[i];
                        }
                        ui->expression->setText(text + tempnumber);
                        buffer.resize(buffer.length() - 3);
                    }
                    else
                    {
                        if(buffer.length() > 0 && buffer[buffer.length() - 1] == 'r')
                        {
                            int c = 0;
                            int i = text.length() - 1;
                            while (text[i] != '<')
                            {
                                text.resize(text.length() - 1);
                                i--;
                            }
                            text.resize(text.length() - 1);
                            i = text.length() - 1;
                            while (text[i] != '<')
                            {
                                c = i;
                                i--;
                            }
                            for (c = c-1; c<text.length(); c++)
                            {
                                text[c] = text[c+5];
                            }
                            text.resize(text.length() - 5);
                            ui->expression->setText(text);
                            buffer.resize(buffer.length() - 1);
                        }
                        else
                        {
                            if (text.length() > 0 && text[(text.length() - 1)] != '>')
                            {
                                if (buffer[buffer.length() - 1] == '^') buffer.resize(buffer.length() - 1);
                                else
                                {
                                    buffer.resize(buffer.length() - 1);
                                    text.resize(text.length() - 1);
                                }
                                ui->expression->setText(text);
                                powstatus = false;
                            }
                            else
                            {
                                if (text.length() > 0)
                                {
                                    buffer.resize(buffer.length() - 1);
                                    text.resize(text.length() - 12);
                                    ui->expression->setText(text);
                                    powstatus = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if (bracketstatus) powstatus = true;
            if (powstatus == true)
            {
                if (action != "^")
                {
                    if(action == '(')
                    {
                        bracketstatus = true;
                    }
                    if(action == ')')
                    {
                        bracketstatus = false;
                    }
                    buffer += action;
                    ui->expression->setText(ui->expression->text() + "<sup>" + action + "</sup>");
                }
                else if (buffer.length() > 0 && buffer[buffer.length() - 1] != '^' && buffer[buffer.length() - 1] != 'r' && buffer[buffer.length() - 1] != 'g') buffer += action;
            }
            else
            {
                if (action == "*")
                {
                    if (buffer.length() > 0)
                    {
                        buffer = buffer + "*";
                        ui->expression->setText(ui->expression->text() + "×");
                    }
                }
                else
                {
                    if (action == "/")
                    {
                        if (buffer.length() > 0)
                        {
                            buffer = buffer + "/";
                            ui->expression->setText(ui->expression->text() + "÷");
                        }
                    }
                    else
                    {
                        if (action == "(" || action == ")" || action == "-")
                        {
                            buffer = buffer + action;
                            ui->expression->setText(ui->expression->text() + action);
                        }
                        else
                        {
                            if (action != "^")
                            {
                                buffer = buffer + action;
                                ui->expression->setText(ui->expression->text() + action);
                            }
                        }
                    }
                }
            }
        }
    }
    QString text;
    text = ui->expression->text();
}

void MainWindow::keyPressEvent(QKeyEvent *pressedkey)
{
    if ((pressedkey->modifiers()==Qt::ControlModifier) && (pressedkey->key()==Qt::Key_Z))
    {
        on_undo_clicked();
    }
    else
    {
        if ((pressedkey->modifiers()==Qt::ControlModifier) && (pressedkey->key()==Qt::Key_Y))
        {
            on_redo_clicked();
        }
        else
        {
            switch (pressedkey->key())
            {
            case Qt::Key_1: write("1");
                break;
            case Qt::Key_2: write("2");
                break;
            case Qt::Key_3: write("3");
                break;
            case Qt::Key_4: write("4");
                break;
            case Qt::Key_5: write("5");
                break;
            case Qt::Key_6: write("6");
                break;
            case Qt::Key_7: write("7");
                break;
            case Qt::Key_8: write("8");
                break;
            case Qt::Key_9: write("9");
                break;
            case Qt::Key_0: write("0");
                break;
            case Qt::Key_Minus: powstatus = false; write("-");
                break;
            case Qt::Key_Plus: powstatus = false; write("+");
                break;
            case Qt::Key_Slash: powstatus = false; write("/");
                break;
            case Qt::Key_Asterisk: powstatus = false; write("×");
                break;
            case Qt::Key_Equal: result();
                break;
            case Qt::Key_Backspace: write("⌫");
                break;
            case Qt::Key_S: write("sin");
                break;
            case Qt::Key_C: write("cos");
                break;
            case Qt::Key_T: write("tan");
                break;
            case Qt::Key_E: write("exp");
                break;
            case Qt::Key_P: write("pi");
                break;
            case Qt::Key_Period: write(".");
                break;
            case Qt::Key_AsciiCircum: write("^");
                break;
            case Qt::Key_BracketLeft: write("(");
                break;
            case Qt::Key_BracketRight: write(")");
                break;
            case Qt::Key_Return: result();
                break;
            case Qt::Key_Delete: write("C");
                break;
            }
        }
    }
}

void firewall(string expression)
{
    bool prevdot = false;
    bool prevop = false;
    int open = 0;
    int close = 0;
    if (expression.length() != 0)
    {
        if (expression.length() != 0 &&
              (expression[0] == '+'
            || expression[0] == '.'
            || expression[0] == '-'
            || expression[0] == '*'
            || expression[0] == '/'
            || expression[0] == '^'
            || expression[0] == 'r'
            || expression[expression.length() - 1] == '.'
            || expression[expression.length() - 1] == '+'
            || expression[expression.length() - 1] == '-'
            || expression[expression.length() - 1] == '*'
            || expression[expression.length() - 1] == '/'
            || expression[expression.length() - 1] == '^'
            || expression[expression.length() - 1] == 'r'
            || (expression[0] == 'l' && expression[1] == 'o' && expression[2] == 'g')))
        {
            error = true;
        }

        for (unsigned long long i = 0; i < expression.length(); i++)
        {
            if (isdigit(expression[i]))
            {
                if (i < expression.length() && expression[i + 1] == '(') error = true;
                prevop = false;
                continue;
            }
            if (expression[i] == '(')
            {
                prevop = false;
                open++;
                continue;
            }
            if (expression[i] == ')')
            {
                prevop = false;
                close++;
                continue;
            }
            if (expression[i] == '.')
            {
                if (prevdot == true) error = true;
                else
                {
                    prevdot = true;
                    prevop = true;
                }
                continue;
            }
            if (expression[i] == '+'
                || expression[i] == '-'
                || expression[i] == '*'
                || expression[i] == '/'
                || expression[i] == '^'
                || expression[i] == 'r')
            {
                if (i < expression.length() && expression[i + 1] == ')') error = true;
                if (prevop == true) error = true;
                else prevop = true;
                continue;
            }

        }
    if (open != close) error = true;
    }
    else error = true;
}

void MainWindow::stringremover()
{
    QString text = ui->expression->text();
    QString texttemp;
    for(int i = 0; i < text.length(); i++)
    {
        if(text[i] == '=')
        {
            int c = 0;
            for(i = i + 1; i < text.length(); i++)
            {
                texttemp.resize(texttemp.length() + 1);
                texttemp[c] = text[i];
                c++;
            }
            ui->expression->setText(texttemp);
        }
    }
    texttemp.clear();
    for(int i = 0; i < buffer.length(); i++)
    {
        if(buffer[i] == '=')
        {
            int c = 0;
            for(i = i + 1; i < buffer.length(); i++)
            {
                texttemp.resize(texttemp.length() + 1);
                texttemp[c] = buffer[i];
                c++;
            }

            buffer = texttemp;
        }
    }
    for (int i = 0; i < buffer.length(); i++)
    {
        if (buffer[i] == ' ')
        {
            for (int c = i; c < buffer.length(); c++)
            {
                buffer[c] = buffer[c + 1];
            }
            buffer.resize(buffer.length() - 1);
            i--;
        }
    }
    for (int i = 0; i < buffer.length() ; i++)
    {
        if (buffer[i] == '(' && (buffer[i + 1] == '-' || buffer[i + 1] == '+'))
        {
                buffer.resize(buffer.length() + 1);
                for (int c = buffer.length() - 1; c > i; c--)
                {
                    buffer[c] = buffer [c - 1];
                }
                buffer[i + 1] = '0';
        }
    }
    if (buffer[0] == '-' || buffer[0] == '+')
    {
        buffer.resize(buffer.length() + 1);
        for (int i = buffer.length() - 1; i >= 0; i--)
        {
            buffer[i] = buffer [i - 1];
        }
        buffer[0] = '0';
    }
}

void string_parsing(string expression)
{
    unsigned long long i = 0; //переменная для счетчика
    double number = 0; //число которое записывается в стек
    double fraction = 1; //счетчик десятичной дроби в числе
    for (i = 0; i < expression.length(); i++)
    {
        if ((expression[i] == 'e' || expression[i] == 'E') && (expression[i+1] == 'x' || expression[i+1] == 'X') && (expression[i+2] == 'p' || expression[i+2] == 'P'))
        {
            digitstack.push(exp(1));
            i+=2;
            continue;
        }
        if ((expression[i] == 'p' || expression[i] == 'P') && (expression[i+1] == 'i' || expression[i+1] == 'I'))
        {
            digitstack.push(atan(1.0) * 4);
            i++;
            continue;
        }
        if ((expression[i] == 'l' || expression[i] == 'L') && (expression[i+1] == 'o' || expression[i+1] == 'O') && (expression[i+2] == 'g' || expression[i+2] == 'G'))
        {
            if (!isdigit(expression[i+3]) && expression[i+3] != 'e' && expression[i+3] != 'p' && expression[i+3] != '(' && expression[i+3] != 's' && expression[i+3] != 'c' && expression[i+3] != 't')
            {
                error = true;
                break;
            }
            else
            {
                if (!isdigit(expression[i-1]) && expression[i-1] != ')')
                {
                    error = true;
                    break;
                }
                else
                {
                    opstack.push('l');
                    fraction = 1;
                    i+=2;
                    continue;
                }
            }
        }
        if ((expression[i] == 'l' || expression[i] == 'L') && (expression[i+1] == 'g' || expression[i+1] == 'G'))
        {
            if (!isdigit(expression[i+2]) && expression[i+2] != 'e' && expression[i+2] != 'p' && expression[i+2] != '(' && expression[i+2] != 's' && expression[i+2] != 'c' && expression[i+2] != 't')
            {
                error = true;
                break;
            }
            else
            {
                digitstack.push(0);
                opstack.push('g');
                fraction = 1;
                i++;
                continue;
            }
        }
        if ((expression[i] == 'l' || expression[i] == 'L') && (expression[i+1] == 'n' || expression[i+1] == 'N'))
        {
            if (!isdigit(expression[i+2]) && expression[i+2] != 'e' && expression[i+2] != 'p' && expression[i+2] != '(' && expression[i+2] != 's' && expression[i+2] != 'c' && expression[i+2] != 't')
            {
                error = true;
                break;
            }
            else
            {
                digitstack.push(0);
                opstack.push('n');
                fraction = 1;
                i++;
                continue;
            }
        }
        if ((expression[i] == 's' || expression[i] == 'S') && (expression[i+1] == 'i' || expression[i+1] == 'I') && (expression[i+2] == 'n' || expression[i+2] == 'N'))
        {
            if (!isdigit(expression[i+3]) && expression[i+3] != 'e' && expression[i+3] != 'p' && expression[i+3] != '(' && expression[i+3] != 'l')
            {
                error = true;
                break;
            }
            else
            {
                digitstack.push(0);
                opstack.push('s');
                fraction = 1;
                i+=2;
                continue;
            }
        }
        if ((expression[i] == 'c' || expression[i] == 'C') && (expression[i+1] == 'o' || expression[i+1] == 'O') && (expression[i+2] == 's' || expression[i+2] == 'S'))
        {
            if (!isdigit(expression[i+3]) && expression[i+3] != 'e' && expression[i+3] != 'p' && expression[i+3] != '(' && expression[i+3] != 'l')
            {
                error = true;
                break;
            }
            else
            {
                digitstack.push(0);
                opstack.push('c');
                fraction = 1;
                i+=2;
                continue;
            }
        }
        if ((expression[i] == 't' || expression[i] == 'T') && (expression[i+1] == 'g' || expression[i+1] == 'G'))
        {
            if (!isdigit(expression[i+2]) && expression[i+2] != 'e' && expression[i+2] != 'p' && expression[i+2] != '(' && expression[i+2] != 'l')
            {
                error = true;
                break;
            }
            else
            {
                digitstack.push(0);
                opstack.push('t');
                fraction = 1;
                i++;
                continue;
            }
        }
        if ((expression[i] == 'c' || expression[i] == 'C') && (expression[i+1] == 't' || expression[i+1] == 'T') && (expression[i+2] == 'g' || expression[i+2] == 'G'))
        {
            if (!isdigit(expression[i+3]) && expression[i+3] != 'e' && expression[i+3] != 'p' && expression[i+3] != '(' && expression[i+3] != 'l')
            {
                error = true;
                break;
            }
            else
            {
                digitstack.push(0);
                opstack.push('k');
                fraction = 1;
                i+=2;
                continue;
            }
        }
        if (expression[i] == 'r')
        {
            if (!isdigit(expression[i-1]) && expression[i-1] != ')')
            {
                error = true;
                break;
            }
            else
            {
                if (!isdigit(expression[i+1]) && expression[i+1] != 'e' && expression[i+1] != 'p' && expression[i+1] != '(')
                {
                    error = true;
                    break;
                }
                else
                {
                    opstack.push('r');
                    fraction = 1;
                    continue;
                }
            }
        }
        if (expression[i] == '(')
        {
            if (expression[i+1] == ')')
            {
                error = true;
                break;
            }
            else
            {
                digitstack.push(0);
                opstack.push(expression[i]);
                fraction = 1;
                continue;
            }
        }
        if (expression[i] == ')')
        {
            if (expression[i+1] == '(')
            {
                error = true;
                break;
            }
            else
            {
                digitstack.push(0);
                opstack.push(expression[i]);
                fraction = 1;
                continue;
            }
        }
        if (expression[i] != '.' && !isalpha(expression[i]) && !isdigit(expression[i]))
        {
            opstack.push(expression[i]);
            fraction = 1;
            continue;
        }

        if (isdigit(expression[i]))
        {
            //если показатель десятичной части числа равен единице то мы проводим обычное добавление к числу
            if (fraction == 1.0) number = (number * 10) + (expression[i] - '0');
            //если показатель десятичной части числа не равен дефолтной единице
            //то мы делим на показатель и добавляем к числу
            if (fraction != 1.0)
            {
                number = number + ((expression[i] - '0') / fraction);
                fraction *= 10; //и увеличиваем показатель для следующего деления
            }
        }
        if (expression[i] == '.')
        {
            fraction *= 10;
        }

        if (expression[i + 1] == '.') continue;

        if (!isdigit(expression[i + 1]))
        {
            digitstack.push(number);
            number = 0;
        }
    }
}

double calculation(double* digitbuffer, char* opbuffer, int digitstacksize, int opstacksize)
{
    int i, c;
    double angle;
    for (i = 0; i < opstacksize; i++)
    {
        if (opbuffer[i] == 'l')
        {
            digitbuffer[i] = log(digitbuffer[i+1])/log(digitbuffer[i]);
            for (c = i; c < opstacksize - 1; c++)
            {
                digitbuffer[c + 1] = digitbuffer[c + 2];
                opbuffer[c] = opbuffer[c + 1];
            }
            digitstacksize--;
            opstacksize--;
            i--;
            continue;
        }
        if (opbuffer[i] == 'g')
        {
            digitbuffer[i] = log10(digitbuffer[i+1]);
            for (c = i; c < opstacksize - 1; c++)
            {
                digitbuffer[c + 1] = digitbuffer[c + 2];
                opbuffer[c] = opbuffer[c + 1];
            }
            digitstacksize--;
            opstacksize--;
            i--;
            continue;
        }
        if (opbuffer[i] == 'n')
        {
            digitbuffer[i] = log(digitbuffer[i+1]);
            for (c = i; c < opstacksize - 1; c++)
            {
                digitbuffer[c + 1] = digitbuffer[c + 2];
                opbuffer[c] = opbuffer[c + 1];
            }
            digitstacksize--;
            opstacksize--;
            i--;
            continue;
        }
        if (opbuffer[i] == 's')
        {
            if (radians == true) angle = digitbuffer[i+1];
            else angle = digitbuffer[i+1]*(atan(1.0) * 4)/180;
            digitbuffer[i] = sin(angle);
            for (c = i; c < opstacksize - 1; c++)
            {
                digitbuffer[c + 1] = digitbuffer[c + 2];
                opbuffer[c] = opbuffer[c + 1];
            }
            digitstacksize--;
            opstacksize--;
            i--;
            continue;
        }
        if (opbuffer[i] == 'c')
        {
            if (radians == true) angle = digitbuffer[i+1];
            else angle = digitbuffer[i+1]*(atan(1.0) * 4)/180;
            digitbuffer[i] = cos(angle);
            for (c = i; c < opstacksize - 1; c++)
            {
                digitbuffer[c + 1] = digitbuffer[c + 2];
                opbuffer[c] = opbuffer[c + 1];
            }
            digitstacksize--;
            opstacksize--;
            i--;
            continue;
        }
        if (opbuffer[i] == 't')
        {
            if (digitbuffer[i+1] != 90.0 && digitbuffer[i+1] != 270.0)
            {
                if (radians == true) angle = digitbuffer[i+1];
                else angle = digitbuffer[i+1]*(atan(1.0) * 4)/180;
                digitbuffer[i] = tan(angle);
                for (c = i; c < opstacksize - 1; c++)
                {
                    digitbuffer[c + 1] = digitbuffer[c + 2];
                    opbuffer[c] = opbuffer[c + 1];
                }
                digitstacksize--;
                opstacksize--;
                i--;
                continue;
            }
            else error = true;
        }
        if (opbuffer[i] == 'k')
        {
            if (digitbuffer[i+1] != 0.0 && digitbuffer[i+1] != 180.0 && digitbuffer[i+1] != 360.0)
            {
                if (radians == true) angle = digitbuffer[i+1];
                else angle = digitbuffer[i+1]*(atan(1.0) * 4)/180;
                digitbuffer[i] = 1/(tan(angle));
                for (c = i; c < opstacksize - 1; c++)
                {
                    digitbuffer[c + 1] = digitbuffer[c + 2];
                    opbuffer[c] = opbuffer[c + 1];
                }
                digitstacksize--;
                opstacksize--;
                i--;
                continue;
            }
            else error = true;
        }
    }

    for (i = 0; i < opstacksize; i++)
    {
        if (opbuffer[i] == '^')
        {
            digitbuffer[i] = pow(digitbuffer[i], digitbuffer[i + 1]);
            for (c = i; c < opstacksize - 1; c++)
            {
                digitbuffer[c + 1] = digitbuffer[c + 2];
                opbuffer[c] = opbuffer[c + 1];
            }
            digitstacksize--;
            opstacksize--;
            i--;
            continue;
        }
        if (opbuffer[i] == 'r')
        {
            if (digitbuffer[i + 1] >= 0)
            {
                digitbuffer[i] = pow(digitbuffer[i+1], 1 / digitbuffer[i]);
                for (c = i; c < opstacksize - 1; c++)
                {
                    digitbuffer[c + 1] = digitbuffer[c + 2];
                    opbuffer[c] = opbuffer[c + 1];
                }
                digitstacksize--;
                opstacksize--;
                i--;
                continue;
            }
            else error = true;
        }
    }

    for (i = 0; i < opstacksize; i++)
    {
        if (opbuffer[i] == '*')
        {
            digitbuffer[i] = digitbuffer[i] * digitbuffer[i + 1];
            for (c = i; c < opstacksize - 1; c++)
            {
                digitbuffer[c + 1] = digitbuffer[c + 2];
                opbuffer[c] = opbuffer[c + 1];
            }
            digitstacksize--;
            opstacksize--;
            i--;
            continue;
        }
        if (opbuffer[i] == '/')
        {
            if (digitbuffer[i+1] != 0.0)
            {
                digitbuffer[i] = digitbuffer[i] / digitbuffer[i + 1];
                for (c = i; c < opstacksize - 1; c++)
                {
                    digitbuffer[c + 1] = digitbuffer[c + 2];
                    opbuffer[c] = opbuffer[c + 1];
                }
                digitstacksize--;
                opstacksize--;
                i--;
                continue;
            }
            else error = true;
        }
    }

    for (i = 0; i < opstacksize; i++)
    {
        if (opbuffer[i] == '+')
        {
            digitbuffer[i] = digitbuffer[i] + digitbuffer[i + 1];
            for (c = i; c < opstacksize - 1; c++)
            {
                digitbuffer[c + 1] = digitbuffer[c + 2];
                opbuffer[c] = opbuffer[c + 1];
            }
            digitstacksize--;
            opstacksize--;
            i--;
            continue;
        }
        if (opbuffer[i] == '-')
        {
            digitbuffer[i] = digitbuffer[i] - digitbuffer[i + 1];
            for (c = i; c < opstacksize - 1; c++)
            {
                digitbuffer[c + 1] = digitbuffer[c + 2];
                opbuffer[c] = opbuffer[c + 1];
            }
            digitstacksize--;
            opstacksize--;
            i--;
            continue;
        }
    }
    return (digitbuffer[0]);
}

double stacktobuffer()
{
    double result = 0;
    int i = 0;
    int digitstacksize = digitstack.size();
    int opstacksize = opstack.size();
    double* digitbuffer = new double[digitstacksize];
    char* opbuffer = new char[opstacksize];

    for (i = digitstacksize - 1; i >= 0; i--)
    {
        digitbuffer[i] = digitstack.top();
        digitstack.pop();
    }

    for (i = opstacksize - 1; i >= 0; i--)
    {
        opbuffer[i] = opstack.top();
        opstack.pop();
    }

    for (int a = 0; a < opstacksize; a++)
    {
        bool brackets = false;
        int i = 0;
        int c = 0;
        int opening = 0;
        int closing = 0;
        int optempsize = 0;
        double* digittemp = new double[digitstacksize];
        char* optemp = new char[opstacksize];
        for (i = opstacksize-1; i >= 0; i--)
        {
            if (opbuffer[i] == '(')
            {
                for (c = i; c < opstacksize; c++)
                {
                    if (opbuffer[c] == ')')
                    {
                        closing = c;
                        brackets = true;
                        break;
                    }
                }
                opening = i + 1;
                break;
            }
        }
        optempsize = closing - opening;
        c = 0;
        for (i = opening; i <= closing; i++) // <= потому что чисел всегда на 1 больше
        {
            digittemp[c] = digitbuffer[i];
            c++;
        }
        c = 0;
        for (i = opening; i < closing; i++)
        {
            optemp[c] = opbuffer[i];
            c++;
        }
        if (brackets == true)
        {
            digitbuffer[opening - 1] = calculation(digittemp, optemp, optempsize + 1, optempsize);

            for (i = opening; i < opstacksize - optempsize; i++)
            {
                digitbuffer[i] = digitbuffer[i + optempsize + 2];
            }

            for (i = opening - 1; i < opstacksize - optempsize; i++)
            {
                opbuffer[i] = opbuffer[i + optempsize + 2];
            }
        }
        else break;
    }
    result = calculation(digitbuffer, opbuffer, digitstacksize, opstacksize);
    return(result);
}
