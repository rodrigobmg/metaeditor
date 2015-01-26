#include "Ui/mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <DAO/databasemanager.h>

int main(int argc, char *argv[])
{
    int ret = -1;

    try
    {
        if( DatabaseManager::instance().openConnection() == true )
        {

            QApplication application(argc, argv);
            MainWindow window;
            window.show();

            ret = application.exec();
        }
        else
        {
            std::cout << "Error" << std::endl;
        }
    }
    catch(std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }

    return ret;
}
