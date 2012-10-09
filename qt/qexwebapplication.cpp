#include "qexwebapplication.h"

QExWebApplication::QExWebApplication(QObject *parent) :
    QObject(parent)
{
    audioPlayer = new QExAudio();
    view = new QWebView();

    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalStorageDatabaseEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    QWebSettings::globalSettings()->setOfflineStoragePath("./");

}

void QExWebApplication::loadAndShow(QString url) {
    QExWebPage * webPage = new QExWebPage();
    this->view->setPage(webPage);
    this->view->load(QUrl(url));
//    this->audioPlayer->init();
    this->addJavaScriptObject();
    this->view->show();

}

void QExWebApplication::addJavaScriptObject()
{
//    this->audioPlayer->init();
    this->view->page()->mainFrame()->addToJavaScriptWindowObject("audioplayer", this->audioPlayer);
}
