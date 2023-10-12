#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

class QLineEdit;
class QPushButton;
class QTextEdit;
class QLabel;
class QTcpSocket;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void connectToServer();
    void readData();
    void sendMessage();
    void updateConnectionStatus(bool connected);

private:
    void setupUI();
    void setupConnections();
    void toggleConnection();

    QLineEdit *ipAddressLineEdit;
    QLineEdit *portLineEdit;
    QLineEdit *messageLineEdit;
    QPushButton *connectButton;
    QPushButton *sendButton;
    QTextEdit *textEdit;
    QLabel *connectionStatusLabel;
    QTcpSocket *socket;
    QTextEdit *logTextEdit;
};

#endif // MAINWINDOW_H
