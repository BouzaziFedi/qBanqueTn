#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("C:/Users/f.bouzazi/Desktop/Qt_Training/BanqueTN/assets/icone.png"));

    QFile styleSheetFile(":C:/Users/f.bouzazi/Desktop/Qt_Training/BanqueTN/styles/stylesheet.css");
    if(styleSheetFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&styleSheetFile);
        a.setStyleSheet(in.readAll());
    }

    QFontDatabase::addApplicationFont("C:/Users/f.bouzazi/Desktop/Qt_Training/BanqueTN/assets/Raleway-SemiBold.ttf");
    QFontDatabase::addApplicationFont("C:/Users/f.bouzazi/Desktop/Qt_Training/BanqueTN/assets/Raleway-Light.ttf");
    QFontDatabase::addApplicationFont("C:/Users/f.bouzazi/Desktop/Qt_Training/BanqueTN/assets/Montserrat-Regular.otf");
    QFontDatabase::addApplicationFont("C:/Users/f.bouzazi/Desktop/Qt_Training/BanqueTN/assets/Montserrat-Light.otf");

    MainWindow *w = new MainWindow;
    w->show();

    QTranslator qtTranslator;
        if (qtTranslator.load(QLocale::system(),
                    "qt", "_",
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        {
            qDebug() << "qtTranslator ok";
            a.installTranslator(&qtTranslator);
        }

        QTranslator qtBaseTranslator;
        if (qtBaseTranslator.load("qtbase_" + QLocale::system().name(),
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        {
            qDebug() << "qtBaseTranslator ok";
            a.installTranslator(&qtBaseTranslator);
        }
    return a.exec();
}
