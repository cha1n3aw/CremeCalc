#include "mainwindow.h"
#include "errorwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QStack>
#include <string>
#include <cmath>
#include <QKeyEvent>
#include <QLabel>
#include <QDebug>
#include <QPainter>
#include <QtGui>

using namespace std;

double calculation(double* digitbuffer, char* opbuffer, int digitstacksize, int opstacksize); //прототип функции для вычисления
double brackets(double* digitbuffer, char* opbuffer, int digitstacksize, int opstacksize); //прототип функции для выделения выражения внутри скобок
static QStack <double> digitstack; //стек чисел
static QStack <char> opstack; //стек операторов
void firewall(string expression);
string stringremover(string expression); //прототип функции чистки строки от пробелов
void string_parsing(string); //прототип функции парсинга стринга
double stacktobuffer(); //прототип функции вычислений
void buttons();
void write(QString expression);
void errorwindow();
static QString buffer;
static bool powstatus = false;
static bool error = false;
static bool bracketstatus = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    setWindowTitle("CremeCalc");

    ui->savedresult->setWordWrap(true);
    ui->history_1->setWordWrap(true);
    ui->history_2->setWordWrap(true);
    ui->history_3->setWordWrap(true);
    ui->history_4->setWordWrap(true);

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
}

MainWindow::~MainWindow()
{
    delete ui;
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
        ui->savedresult->setText(temp1);
    }
    if (op == "MR") write(ui->savedresult->text());
    if (op == "MC") ui->savedresult->setText("");
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
        buffer = QString::fromStdString(stringremover(buffer.toLocal8Bit().constData()));
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
                    ui->history_4->setText(ui->history_3->text());
                    ui->history_3->setText(ui->history_2->text());
                    ui->history_2->setText(ui->history_1->text());
                    ui->history_1->setText(result);
                }
                else errorwindow();
        }
        else errorwindow();
    }
}

void MainWindow::write(QString action)
{
    switch (action.toLocal8Bit().constData()[0])
    {
        case '-': powstatus = false;
            break;
        case '+': powstatus = false;
            break;
        case '/': powstatus = false;
            break;
        case '*': powstatus = false;
            break;
        case 'C': powstatus = false;
            break;
        case '^': powstatus = true;
            break;
        case '=': result();
            break;
        case 'r':
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
                       && buffer[i] != '^')
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
        case 'g':
        {
            QString text;
            QString temp = ui->expression->text();
            int i = buffer.length() - 1;
            QString text1;
            while (i >= 0
                   && buffer[i] != '+'
                   && buffer[i] != '-'
                   && buffer[i] != '*'
                   && buffer[i] != '/'
                   && buffer[i] != '^'
                   && buffer[i] != 'r')
                {
                    text += buffer[i];
                    temp.resize(temp.length() - 1);
                    i--;
                }
            text1.resize(text.length());
            for (i = 0; i < text.length(); i++)
            {
                text1[text1.length() - 1 - i] = text[i];
            }
            ui->expression->setText(temp + "log<sub>" + text1 + "</sub>");
            buffer = buffer + "l";
            break;
        }

    }
    if (action != "=" && action != 'r' && action != 'g')
    {
        if (action == "⌫")
        {
            QString text = ui->expression->text();
            if(buffer[buffer.length() - 1] == 'r')
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
                if (text[(text.length() - 1)] != '>')
                {
                    qDebug() << "low cut triggered";
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
                    qDebug() << "high cut triggered";
                    buffer.resize(buffer.length() - 1);
                    text.resize(text.length() - 12);
                    ui->expression->setText(text);
                    powstatus = true;
                }
            }
            qDebug() << buffer;
            qDebug() << text;
        }
        else
        {
            if (action == "C")
            {
                buffer = "";
                ui->expression->setText("");
                powstatus = false;
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
                    else buffer += action;
                }
                else
                {
                    if (action == "*")
                    {
                        buffer = buffer + "*";
                        ui->expression->setText(ui->expression->text() + "×");
                    }
                    else
                    {
                        if (action == "/")
                        {
                            buffer = buffer + "/";
                            ui->expression->setText(ui->expression->text() + "÷");
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
                                buffer = buffer + action;
                                ui->expression->setText(ui->expression->text() + action);
                            }
                        }
                    }
                }
            }
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *pressedkey)
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
    case Qt::Key_Backspace: write("delete");
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
    case Qt::Key_Delete: write("erase");
        break;
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

string stringremover(string expression)
{
    for (unsigned long i = 0; i < expression.length(); i++)
    {
        if (expression[i] == ' ')
        {
            for (unsigned long c = i; c < expression.length(); c++)
            {
                expression[c] = expression[c + 1];
            }
            expression.resize(expression.length() - 1);
            i--;
        }
    }
    for (unsigned long i = 0; i < expression.length() - 1; i++)
    {
        if (expression[i] == '(' && (expression[i + 1] == '-' || expression[i + 1] == '+'))
        {
                expression.resize(expression.length() + 1);
                for (unsigned long long c = expression.length(); c > i; c--)
                {
                    expression[c] = expression [c - 1];
                }
                expression[i + 1] = '0';
                break;
        }
    }
    return expression;
}

double calculation(double* digitbuffer, char* opbuffer, int digitstacksize, int opstacksize)
{
    int i, c;

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
            digitbuffer[i] = sin(digitbuffer[i+1]*(atan(1.0) * 4)/180);
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
            digitbuffer[i] = cos(digitbuffer[i+1]*(atan(1.0) * 4)/180);
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
                digitbuffer[i] = tan(digitbuffer[i+1]*(atan(1.0) * 4)/180);
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
                digitbuffer[i] = 1/(tan(digitbuffer[i+1]*(atan(1.0) * 4)/180));
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
                if (!isdigit(expression[i-1]) && expression[i+3] != ')')
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
        qDebug() << "result =" << result;
        return(result);
    }
