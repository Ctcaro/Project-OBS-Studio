#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
namespace Ui {
	class LoginWindow;
}

class LoginWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit LoginWindow(QWidget *parent = 0);
	~LoginWindow();
	QJsonObject makePostRequest(QString str);
	void handleAuthError(QString str);
	void handleFileError(QString str);
	void writeJsonFile(QString filepath, QString data);
	private slots:
	void on_webView_urlChanged(const QUrl &arg1);
signals:
	void mySignal();
private:
	Ui::LoginWindow *ui;
};

#endif // MAINWINDOW_H
