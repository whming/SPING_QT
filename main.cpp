#include "main_widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    SQL_Api SQL_object;
    Main_Widget *Main;
    Main=new Main_Widget;
   Main->show();
    return a.exec();
}
