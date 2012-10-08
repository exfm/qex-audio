#include <QApplication>
#include <qexwebapplication.h>
#include <QtWebKit>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QExWebApplication * thisApp = new QExWebApplication();
    thisApp->loadAndShow("qrc:/index.html");

    QObject::connect(thisApp->view->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), thisApp, SLOT(addJavaScriptObject()));

    // QObject::connect(view->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), webPage, SLOT(addJavaScriptObject(QWebView*)))


    return app.exec();
}
