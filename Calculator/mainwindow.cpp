#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<math.h>


int res = 0; // store the result
bool addButtonPressed = false; //whether + button is pressed or not
bool subButtonPressed = false; // whether - button is pressed or not

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    //initially, ui and Result text is set.
    ui->setupUi(this);
    ui->Result->setText("");

    // numbers from 0-A and clr buttton is connected to the Pressed() slot in this for loop.
    QPushButton *nums[17];
    for(int i = 0; i < 17; i++) {

        QString buttonName = "Button" +   QString::number(i); //button names
        nums[i] = MainWindow::findChild<QPushButton *>(buttonName);
        connect(nums[i], SIGNAL(released()), this, SLOT(Pressed()));

    }
    // - and + operator buttons is connected to the Operator() slot.
    connect(ui-> Button17, SIGNAL(released()), this, SLOT(Operator())); 
    connect(ui-> Button18, SIGNAL(released()), this, SLOT(Operator()));

    // = button is connected to Calculate() slot.
    connect(ui-> Button19, SIGNAL(released()), this, SLOT(Calculate()));
}

//destructor default code
MainWindow::~MainWindow()
{
    delete ui;
}

//pressed slot code
void MainWindow::Pressed() {

    QPushButton *button = (QPushButton *)sender(); // detect which button is pressed.
    QString buttonVal = button -> text(); // text on the button. "F" for example.
    QString displayVal = ui -> Result -> text(); // text on the screen.
    if(buttonVal == "Clr") {
        //if Clr is pressed, this code is basically delete the screen and set res to NULL.
        res = 0;
        ui -> Result ->setText(QString::number(res, 16).toUpper());
        return;
    }
    else{
        //if nothing on the screen set display text to button value, else add to display value.
        if(displayVal == "" || displayVal == "0") {
            
            ui -> Result ->setText("");
            ui -> Result ->setText(buttonVal);
        }
        else{
            QString newValue = displayVal + buttonVal;
            ui -> Result -> setText(newValue);
        }
    }
}

// parse string to integer value.
int Parse(QString str) {

    int result = 0;

    //if number has a unary operator, the index starts from 1.
    QString first = str[0];
    int i = 0;
    if(first == "-" || first == "+") i++;

    //parse hexadecimal string to integer number. 
    for(; i < str.capacity(); i++) {

        if(str[i].isDigit()) {
            result += str[i].digitValue() * pow(16, str.capacity() -1 - i);
        }
        //is the digit is A-F, then we use their ascii value and subtract 55 from their ascii value to get their actual hexadecimal value.
        else {
            result += (str[i].toUpper().toLatin1() - 55) * pow(16, str.capacity() -1 - i);
        }
    }

    //lastly if it has - unary opratr, we take its negative and return it.
    if(first == "-") {
        result *= -1;
    }

    return result;
}

//operator slot code
void MainWindow::Operator() {

    //get the current value on the screen.
    QString current  = ui -> Result -> text();

    QPushButton * btn = (QPushButton *)sender(); // detect which operator button is pressed.
    QString buttonText = btn ->text(); // text on the button. "+" or "-".

    if(current == "" || current == "+" || current == "-" || current == "0") {
        //if there is nothing on the screen, we basically use operator as a unary operator.
        //moreover, if there is only a unary operator on the screen, we can change it.
        ui -> Result -> setText(buttonText);
        return;
    }

    else{
        int currentVal = Parse(current); // parse to value on the screen.

        
        //if any operator button pressed, just do the math.
        if(addButtonPressed || subButtonPressed) {
           if(addButtonPressed) res+= currentVal;
           else res -= currentVal;
        }
        else {
            //if no operator button pressed, just set res to currentVal
            res = currentVal;
        }
        //we make sure that if there is a button that is pressed before become undepressed.
        addButtonPressed = false;
        subButtonPressed = false;

        //check which button is pressed.
        if(QString::compare(buttonText, "+", Qt::CaseInsensitive) == 0){
            addButtonPressed = true;
        } else {
            subButtonPressed = true;

        }
        //after an operator button is pressed for binary operation, we set the text on the screen empty and wait another operator.
        ui -> Result -> setText("");
    }
}

void MainWindow::Calculate() {

    //if there is no numeric value on the screen, do nothing.
    QString current  = ui -> Result -> text();
    if(current == "" || current == "+" || current == "-") return;

    //parse the valur on the screen
    int currentVal = Parse(current);
    //do the last math.
    if(addButtonPressed || subButtonPressed) {
        if(addButtonPressed){
           currentVal += res;
           addButtonPressed = false;
        }
        else {
            currentVal = res - currentVal;
            subButtonPressed = false;
        }
    }

    //set the screen text value and finish.
    ui -> Result ->setText(QString::number(currentVal, 16).toUpper());
    res = 0;
}


