//#include "main_widget.h"
#include "init_dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Main_Widget *Main;
//    Main=new Main_Widget;
//    Main->show();
    SQL_Api SQL_object;
     Main_Widget *Main;
     Init_Dialog MDialog;
     if(SQL_object.Sql_data_find_dev_info(DATABASE) > 0){
         Main=new Main_Widget;
        Main->show();
     }else{
         MDialog.show();
     }
    return a.exec();
}
