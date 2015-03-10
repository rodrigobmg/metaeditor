#include "Ui/mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <DAO/databasemanager.h>

int main(int argc, char *argv[])
{
    int ret = -1;

    try
    {
        DatabaseManager::instance().connect();

        Object* obj = DatabaseManager::instance().read("users", "rodrigo", "test");


        QApplication application(argc, argv);
        MainWindow window;
        window.show();

        delete obj;
        ret = application.exec();
    }
    catch(std::exception& ex)
    {
        //That catch is only for exceptions not treated
        std::cout << ex.what() << std::endl;
    }

    return ret;
}
