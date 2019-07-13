#include "mainwindow.h"
#include "errorwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <stack>
#include <string>
#include <cmath>
#include <QKeyEvent>
#include <QLabel>
#include <QDebug>
#include <QPainter>
#include <QtGui>
//#include <QApplication>
//#include <QWidget>
//#include <QtWidgets>

using namespace std;

double calculation(double* digitbuffer, char* opbuffer, int digitstacksize, int opstacksize); //прототип функции для вычисления
double brackets(double* digitbuffer, char* opbuffer, int digitstacksize, int opstacksize); //прототип функции для выделения выражения внутри скобок
static stack <double> digitstack; //стек чисел
static stack <char> opstack; //стек операторов
bool firewall(string expression);
string stringremover(string expression); //прототип функции чистки строки от пробелов
void string_parsing(string); //прототип функции парсинга стринга
double stacktobuffer(); //прототип функции вычислений
void erase();
void write(QString expression);
static QString buffer;
static QString substring;
static bool powstatus = false;
const int offset_x = 1;
const int offset_y = 24;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
          QFont tahoma("MS Sans Serif", 15, QFont::Monospace);
          event->type();
          QPainter powdraw(this);
          QPen powPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap);
          powdraw.setFont(tahoma);
          powdraw.setPen(powPen);
          powdraw.drawText(offset_x, offset_y, substring);
          update();
}

void MainWindow::result ()
{
    QString result;
    //QString to string
    buffer = QString::fromStdString(stringremover(buffer.toLocal8Bit().constData()));
    string expression = buffer.toLocal8Bit().constData();
    //string to QString
    //if (firewall(expression) == true)
    //  {
            string_parsing(expression);
            result = QString::fromStdString(to_string(stacktobuffer()));
     // }
    //else
      //{
         //   ui->expression->setText("");
      //      substring = "";
      //      buffer = "";
     //       update();
     //       ErrorWindow errorwindow;
     //       errorwindow.setModal(true);
     //       errorwindow.exec();
     // }
    int i=result.length() - 1;
    while (result[i] == '0' && i>0)
    {
       if (result[i-1] == '.') result.resize(i-1);
        else result.resize(i);
       i--;
    }
    if (buffer != "invalid") ui->expression->setText(ui->expression->text() + " = " + result);

}

void MainWindow::write(QString action)
{
    if (action == "delete")
    {
       // qDebug() << "buffer " << buffer;
      //  qDebug() << "buffer length " << buffer.length();
        if (buffer[buffer.length() - 1] == '^')
        {
            buffer.resize(buffer.length() - 1);
            powstatus = false;
        }
        else
        {
            buffer.resize(buffer.length() - 1);
            substring.resize(substring.length() - 1);
            QString text = ui->expression->text();
            text.resize(text.length() - 1);
            ui->expression->setText(text);
            update();
        }
    }
    else
    {
        if (action == "erase")
        {
            buffer = "";
            substring = "";
            ui->expression->setText("");
            powstatus = false;
            update();
        }
        else
            if (powstatus == false)
            {
                        if (action != "√")
                        {
                            buffer = buffer + action;
                            ui->expression->setText(ui->expression->text() + action);
                            substring = substring + QChar(0x2007); //QChar(0x2001) + QChar(0x2001)
                            update();
                        }
                        else
                        {
                            buffer = buffer + "r";
                            ui->expression->setText(ui->expression->text() + action);
                            substring = substring + QChar(0x2007); //QChar(0x2001) + QChar(0x2001)
                            update();
                        }
            }
            else
            {
                if(action != "^")
                {
                    buffer = buffer + action;
                    substring = substring + action;
                    ui->expression->setText(ui->expression->text() + QChar(0x2007)); //QChar(0x2001) + QChar(0x2001)
                    update();
                }
                else
                {
                    buffer = buffer + action;
                    update();
                }
            }
    }
   // qDebug() << "buffer reworked " << buffer;
   // qDebug() << "buffer reworked length " << buffer.length();
}

void MainWindow::drawroot(int xs, int xe)
{
    QPainter painter(this);
    QPen myPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap);
    painter.setPen(myPen);
    painter.drawLine(offset_x + xs, offset_y - 20, offset_x + xs + 10, offset_y);
    painter.drawLine(offset_x + xs + 10, offset_y, offset_x + xs + 20, offset_y - 30);
    painter.drawLine(offset_x + xs + 20, offset_y - 30, offset_x + xe, offset_y - 30);
}

void MainWindow::erase()
{
    write("delete");
}

void MainWindow::on_bt1_clicked()
{
  // buffer = buffer + "1"; update();
    write("1");
}

void MainWindow::on_bt2_clicked()
{
    write("2");
}

void MainWindow::on_bt3_clicked()
{
    write("3");
}

void MainWindow::on_bt4_clicked()
{
    write("4");
}

void MainWindow::on_bt5_clicked()
{
    write("5");
}

void MainWindow::on_bt6_clicked()
{
    write("6");
}

void MainWindow::on_bt7_clicked()
{
    write("7");
}

void MainWindow::on_bt8_clicked()
{
    write("8");
}

void MainWindow::on_bt9_clicked()
{
    write("9");
}

void MainWindow::on_bt0_clicked()
{
    write("0");
}

void MainWindow::on_btdot_clicked()
{
    write(".");
}

void MainWindow::on_btpl_clicked()
{
    powstatus = false;
    write("+");
}

void MainWindow::on_btmin_clicked()
{
    powstatus = false;
    write("-");
}

void MainWindow::on_btmult_clicked()
{
    powstatus = false;
    write("*");
}

void MainWindow::on_btdiv_clicked()
{
    powstatus = false;
    write("/");
}

void MainWindow::on_btpow_clicked()
{
    powstatus = true;
    write("^");
}

void MainWindow::on_btroot_clicked()
{
    write("√");
}

void MainWindow::on_btp_clicked()
{
    write("pi");
}

void MainWindow::on_bte_clicked()
{
    write("exp");
}

void MainWindow::on_btclr_clicked()
{
    powstatus = false;
    write("erase");
}

void MainWindow::on_btdel_clicked()
{
    write("delete");
}

void MainWindow::on_btopen_clicked()
{
    write("(");
}

void MainWindow::on_btclose_clicked()
{
   write(")");
}

void MainWindow::on_btlog_clicked()
{
    write("log");
}

void MainWindow::on_btln_clicked()
{
    write("ln");
}

void MainWindow::on_btsin_clicked()
{
    write("sin");
}

void MainWindow::on_btcos_clicked()
{
    write("cos");
}

void MainWindow::on_bttan_clicked()
{
    write("tan");
}

void MainWindow::on_btctan_clicked()
{
    write("ctan");
}

void MainWindow::on_sqroot_clicked()
{
    powstatus = true;
    write("2");
    powstatus = false;
    write("√");
}

void MainWindow::on_btms_clicked()
{
    ui->savedresult->setText(buffer); update();
}

void MainWindow::on_btmr_clicked()
{
    buffer = buffer + ui->savedresult->text(); update();
}

void MainWindow::on_btmc_clicked()
{
    ui->savedresult->setText(""); update();
}

void MainWindow::on_bteq_clicked()
{
    result(); update();
}

void MainWindow::keyPressEvent(QKeyEvent *pressedkey)
{
    switch (pressedkey->key())
    {
    case Qt::Key_1: buffer = buffer + "1"; update();
        break;
    case Qt::Key_2: buffer = buffer + "2"; update();
        break;
    case Qt::Key_3: buffer = buffer + "3"; update();
        break;
    case Qt::Key_4: buffer = buffer + "4"; update();
        break;
    case Qt::Key_5: buffer = buffer + "5"; update();
        break;
    case Qt::Key_6: buffer = buffer + "6"; update();
        break;
    case Qt::Key_7: buffer = buffer + "7"; update();
        break;
    case Qt::Key_8: buffer = buffer + "8"; update();
        break;
    case Qt::Key_9: buffer = buffer + "9"; update();
        break;
    case Qt::Key_0: buffer = buffer + "0"; update();
        break;
    case Qt::Key_Minus: buffer = buffer + "-"; update();
        break;
    case Qt::Key_Plus: buffer = buffer + "+"; update();
        break;
    case Qt::Key_Slash: buffer = buffer + "/"; update();
        break;
    case Qt::Key_Asterisk: buffer = buffer + "*"; update();
        break;
    case Qt::Key_Equal: result(); update();
        break;
    case Qt::Key_Backspace: erase();
        break;
    case Qt::Key_Space: buffer = buffer + " "; update();
        break;
    case Qt::Key_S: buffer = buffer + "sin"; update();
        break;
    case Qt::Key_C: buffer = buffer + "cos"; update();
        break;
    case Qt::Key_T: buffer = buffer + "tan"; update();
        break;
    case Qt::Key_E: buffer = buffer + "exp"; update();
        break;
    case Qt::Key_P: buffer = buffer + "pi"; update();
        break;
    case Qt::Key_Period: buffer = buffer + "."; update();
        break;
    case Qt::Key_AsciiCircum: buffer = buffer + "^"; update();
        break;
    case Qt::Key_BracketLeft: buffer = buffer + "("; update();
        break;
    case Qt::Key_BracketRight: buffer = buffer + ")"; update();
        break;
    case Qt::Key_Return: result(); update();
        break;
    case Qt::Key_Delete: buffer.resize(0); update();
        break;
    case Qt::Key_M: ui->savedresult->setText(buffer); update();
        break;
    case Qt::Key_R: buffer = buffer + ui->savedresult->text(); update();
        break;
    }
}

bool firewall(string expression)
{
    bool prevdot = false; //это объявление
    bool prevop = false;
    int open = 0;
    int close = 0;
    for (unsigned long long i = 0; i < expression.length(); i++)
    {
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
            if (prevdot == true) return false;
            else
            {
                prevdot = true;
                prevop = true;
                continue;
            }
        }
        if (isdigit(expression[i]))
        {
            if (i < expression.length() && (expression[i + 1] == '(' || expression[i + 1] == ')')) return false;
            else prevop = false;
            continue;
        }
        if (expression[i] == '+'
            && expression[i] == '-'
            && expression[i] == '*'
            && expression[i] == '/'
            && expression[i] == '^')
        {
            if (prevop == true) return false;
            else
                {
                 prevop = true;
                 return true;
                }
        }

    }
    if (open == close) return true;
    else return false;
}

string stringremover(string expression)
{
    if (expression[0] == '*' || expression[0] == '/' || expression[0] == '+') //на случай если в начале будет * или / или +
    {
        for (unsigned long long i = 0; i < expression.length(); i++)
        {
            expression[i] = expression[i + 1]; //стринг сдвигается на 1 позицию влево
        }
        expression.resize(expression.length() - 1);
    }
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

    return expression;
}

double calculation(double* digitbuffer, char* opbuffer, int digitstacksize, int opstacksize)
{
    int i, c;

    for (i = 0; i < opstacksize; i++)
    {
        if (opbuffer[i] == 'l')
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
        if (opbuffer[i] == 'k')
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
    //string root = "√";
    unsigned long long i = 0; //переменная для счетчика
    double number = 0; //число которое записывается в стек
    double fraction = 1; //счетчик десятичной дроби в числе
    if (expression[0] == '-') //на случай в начале будет -
    {
        expression.resize(expression.length() + 1);
        for (i = expression.length(); i > 0; i--)
        {
            expression[i] = expression[i - 1]; //стринг увеличивается и сдвигается на 1 позицию
        }
        expression[0] = '0'; //а в нулевую записывается 0
    }
    for (i = 0; i < expression.length(); i++)
    {
        if ((expression[i] == 'l' || expression[i] == 'L') && (expression[i+1] == 'o' || expression[i+1] == 'O') && (expression[i+2] == 'g' || expression[i+2] == 'G'))
        {
            digitstack.push(0);
            opstack.push('l');
            fraction = 1;
            i+=2;
            continue;
        }
        if ((expression[i] == 'l' || expression[i] == 'L') && (expression[i+1] == 'n' || expression[i+1] == 'N'))
        {
            digitstack.push(0);
            opstack.push('n');
            fraction = 1;
            i++;
            continue;
        }
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
        if ((expression[i] == 's' || expression[i] == 'S') && (expression[i+1] == 'i' || expression[i+1] == 'I') && (expression[i+2] == 'n' || expression[i+2] == 'N'))
        {
            digitstack.push(0);
            opstack.push('s');
            fraction = 1;
            i+=2;
            continue;
        }
        if ((expression[i] == 'c' || expression[i] == 'C') && (expression[i+1] == 'o' || expression[i+1] == 'O') && (expression[i+2] == 's' || expression[i+2] == 'S'))
        {
            digitstack.push(0);
            opstack.push('c');
            fraction = 1;
            i+=2;
            continue;
        }
        if ((expression[i] == 't' || expression[i] == 'T') && (expression[i+1] == 'a' || expression[i+1] == 'A') && (expression[i+2] == 'n' || expression[i+2] == 'N'))
        {
            digitstack.push(0);
            opstack.push('t');
            fraction = 1;
            i+=2;
            continue;
        }
        if ((expression[i] == 'c' || expression[i] == 'C') && (expression[i+1] == 't' || expression[i+1] == 'T') && (expression[i+2] == 'a' || expression[i+2] == 'A') && (expression[i+3] == 'n' || expression[i+3] == 'N'))
        {
            digitstack.push(0);
            opstack.push('k');
            fraction = 1;
            i+=3;
            continue;
        }
        if (expression[i] == 'r')
        {
            opstack.push('r');
            fraction = 1;
            continue;
        }
        if ((expression[i] == 's' || expression[i] == 'S') && (expression[i+1] == 'q' || expression[i+1] == 'Q') && (expression[i+2] == 'r' || expression[i+2] == 'R') && (expression[i+3] == 't' || expression[i+3] == 'T'))
        {
            digitstack.push(2);
            opstack.push('r');
            fraction = 1;
            i+=3;
            continue;
        }
        if ((expression[i] == 'r' || expression[i] == 'R') && (expression[i+1] == 'o' || expression[i+1] == 'O') && (expression[i+2] == 'o' || expression[i+2] == 'O') && (expression[i+3] == 't' || expression[i+3] == 'T'))
        {
            opstack.push('r');
            fraction = 1;
            i+=3;
            continue;
        }
        if (expression[i] == '(' || expression[i] == ')')
        {
            digitstack.push(0);
            opstack.push(expression[i]);
            fraction = 1;
            continue;
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
        qDebug() << "result = " << result;
        return(result);
    }
/*




//QPixmap pi;
//  QPainter p(&pi);
//  p.setRenderHint(QPainter::Antialiasing);
//  p.setPen(QPen(Qt::black, 12, Qt::DashDotLine, Qt::RoundCap));
//  p.drawLine(0, 0, 200, 200);
//    p.end(); // Don't forget this line!
/////////////////////
//QLabel l;
//ui->history1->setPixmap(pi);
//l.show();
//drawroot(10, 1000);




   // QPainter painter(this); //create a QPainter and pass a pointer to the device.
                            //A paint device can be a QWidget, a QPixmap or a QImage
    // painter.drawLine(100,100,100,1000); //a simple line
    //create a black pen that has solid line and the width is 2.
   // QPen myPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap);
    //painter.setPen(myPen);
   // drawroot(expression_length, expression_length + 30);
    //expression_length += 30;
   // drawroot(100, 130);
   // drawroot(150, 200);
    //drawline(100, 100);

    //draw a point
   // myPen.setColor(Qt::black);
  //  myPen.setWidth(5);
   // painter.setPen(myPen);
  //  painter.drawPoint(90,200);
    //painter.drawText(myPen, "5");
    //draw a polygon
   // QPolygon polygon;
   // polygon << QPoint(130, 140) << QPoint(180, 170)
   //          << QPoint(180, 140) << QPoint(220, 110)
   //          << QPoint(140, 100);
   //  painter.drawPolygon(polygon);

     //draw an ellipse
     //The setRenderHint() call enables antialiasing, telling QPainter to use different
     //color intensities on the edges to reduce the visual distortion that normally
     //occurs when the edges of a shape are converted into pixels
    // painter.setRenderHint(QPainter::Antialiasing, true);
    // painter.setPen(QPen(Qt::black, 3, Qt::DashDotLine, Qt::RoundCap));
    // painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
    // painter.drawEllipse(200, 80, 400, 240);


   // myPen.setWidth(2);
   // painter.setPen(myPen);
  //  QPoint baseline(100, 90);
  //  painter.drawText(baseline, "12345");
bool isoperator()
{
    string root = "√";
    if (!isdigit(buffer.toLocal8Bit().constData()[buffer.length()])
        && !isalpha(buffer.toLocal8Bit().constData()[buffer.length()])
        && buffer[buffer.length()] != '.'
        && buffer[buffer.length()] != '+'
        && buffer[buffer.length()] != '-'
        && buffer[buffer.length()] != '*'
        && buffer[buffer.length()] != '/'
        && buffer[buffer.length()] != '^'
        && buffer[buffer.length()] != '('
        && buffer[buffer.length()] != ')'
        && buffer[buffer.length()] != root[0]
        && (buffer[buffer.length() - 1] != 'l' && buffer[buffer.length()] != 'n')
        && (buffer[buffer.length() - 2] != 's' && buffer[buffer.length() - 1] != 'i' && buffer[buffer.length()] != 'n') //sin
        && (buffer[buffer.length() - 2] != 'c' && buffer[buffer.length() - 1] != 'o' && buffer[buffer.length()] != 's') //cos
        && (buffer[buffer.length() - 2] != 'l' && buffer[buffer.length() - 1] != 'o' && buffer[buffer.length()] != 'g') //log
        && (buffer[buffer.length() - 2] != 't' && buffer[buffer.length() - 1] != 'a' && buffer[buffer.length()] != 'n') //tan
        && (buffer[buffer.length() - 3] != 'c' && buffer[buffer.length() - 2] != 't' && buffer[buffer.length() - 1] != 'a' && buffer[buffer.length()] != 'n') //ctan
        && (buffer[buffer.length() - 3] != 's' && buffer[buffer.length() - 2] != 'q' && buffer[buffer.length() - 1] != 'r' && buffer[buffer.length()] != 't')) //sqrt
        return false;
    else
        return true;
}


*/
