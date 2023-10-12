#include "mainwindow.h"
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), socket(new QTcpSocket(this)) {
    setupUI();
    setupConnections();
}

void MainWindow::connectToServer() {
    QString serverAddress = ipAddressLineEdit->text();
    quint16 serverPort = portLineEdit->text().toInt();

    socket->connectToHost(serverAddress, serverPort);
}

void MainWindow::readData() {
    while (socket->bytesAvailable() > 0) {
        QByteArray data = socket->readAll();
        QDateTime currentTime = QDateTime::currentDateTime();
        QString formattedTime = currentTime.toString("[hh:mm:ss] ");
        // Отображение полученных данных в текстовом поле
        textEdit->append(formattedTime + data);
    }
}

void MainWindow::sendMessage() {
    QString message = messageLineEdit->text();
    if (!message.isEmpty() && socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data = message.toUtf8();
        socket->write(data);
        socket->flush();

        // Log the sent command
        QDateTime currentTime = QDateTime::currentDateTime();
        QString formattedTime = currentTime.toString("[hh:mm:ss] ");
        logTextEdit->append("Sent: " + formattedTime + message);

        messageLineEdit->clear();
    }
    //messageLineEdit->clear();
}

void MainWindow::updateConnectionStatus(bool connected) {
    if (connected) {
        connectionStatusLabel->setText("Соединение установлено");
    } else {
        connectionStatusLabel->setText("Нет соединения");
    }
}

void MainWindow::setupUI() {
    connectionStatusLabel = new QLabel("Нет соединения");
    ipAddressLineEdit = new QLineEdit("127.0.0.1");
    portLineEdit = new QLineEdit("12345");
    connectButton = new QPushButton("Подключиться");

    textEdit = new QTextEdit;
    messageLineEdit = new QLineEdit;

    logTextEdit = new QTextEdit;
    logTextEdit->setReadOnly(true); // Make the log read-only

    sendButton = new QPushButton("Отправить");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(connectionStatusLabel);
    layout->addWidget(ipAddressLineEdit);
    layout->addWidget(portLineEdit);
    layout->addWidget(connectButton);
    layout->addWidget(messageLineEdit);
    layout->addWidget(sendButton);
    layout->addWidget(logTextEdit); // Add the log widget
    layout->addWidget(textEdit);


    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}
void MainWindow::toggleConnection() {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost(); // Disconnect when connected
    } else {
        connectToServer(); // Connect when disconnected
    }
}
void MainWindow::setupConnections() {
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::connectToServer);
    connect(sendButton, &QPushButton::clicked, this, &MainWindow::sendMessage);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readData);
    connect(socket, &QTcpSocket::connected, this, [this]() { updateConnectionStatus(true); });
    connect(socket, &QTcpSocket::disconnected, this, [this]() { updateConnectionStatus(false); });
    // Update the button text when connected or disconnected
    connect(socket, &QTcpSocket::connected, this, [this]() {
        updateConnectionStatus(true);
        connectButton->setText("Отключиться"); // Change button text to "Disconnect"
    });

    connect(socket, &QTcpSocket::disconnected, this, [this]() {
        updateConnectionStatus(false);
        connectButton->setText("Подключиться"); // Change button text to "Connect"
    });
}
