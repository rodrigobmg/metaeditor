#include <QApplication>
#include <QMessageBox>

//Own
#include "Ui/mainwindow.h"

int main(int argc, char *argv[])
{
    try
    {
        QApplication application(argc, argv);
        MainWindow window;

        window.show();
        return application.exec();
    }
    catch(std::exception& ex)
    {
        QMessageBox errorMessage;
        errorMessage.critical(nullptr, "Ocorreu um erro...", QString::fromLocal8Bit(ex.what()));
        return 0;
    }

    return -1;
}
