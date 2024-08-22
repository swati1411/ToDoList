#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize PostgreSQL connection
    try {
        conn = new pqxx::connection("dbname=todo_list user=postgres password=xp2#$kal1 hostaddr=127.0.0.1 port=5432");
        if (conn->is_open()) {
            qDebug() << "Connected to database.";
        } else {
            qDebug() << "Failed to connect to database.";
        }
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Database Error", e.what());
    }

    // Load tasks from the database
    loadTasks();
}


MainWindow::~MainWindow()
{
    delete conn;  // Clean up the connection
    delete ui;
}

void MainWindow::on_btnAdd_clicked()
{
    QString taskText = ui->txtAdd->text();
    if (!taskText.isEmpty()) {
        try {
            pqxx::work W(*conn);
            W.exec("INSERT INTO todo_list (task, done) VALUES (" + W.quote(taskText.toStdString()) + ", 'False')");
            W.commit();

            QListWidgetItem *item = new QListWidgetItem(taskText, ui->listWidget);
            item->setFlags(item->flags() | Qt::ItemIsEditable);

            ui->txtAdd->clear();
            ui->txtAdd->setFocus();
        } catch (const std::exception &e) {
            QMessageBox::critical(this, "Database Error", e.what());
        }
    }
}


void MainWindow::on_btnRemove_clicked()
{
    int row = ui->listWidget->currentRow();
    if (row >= 0) {
        QListWidgetItem *item = ui->listWidget->takeItem(row);
        QString taskText = item->text();
        delete item;

        try {
            pqxx::work W(*conn);
            W.exec("DELETE FROM todo_list WHERE task = " + W.quote(taskText.toStdString()));
            W.commit();
        } catch (const std::exception &e) {
            QMessageBox::critical(this, "Database Error", e.what());
        }
    }
}


void MainWindow::on_btnRemoveAll_clicked()
{
    ui->listWidget->clear();
    try {
        pqxx::work W(*conn);
        W.exec("DELETE FROM todo_list");
        W.commit();
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Database Error", e.what());
    }
}

void MainWindow::loadTasks()
{
    try {
        pqxx::work W(*conn);
        pqxx::result R = W.exec("SELECT task FROM todo_list");

        for (auto row : R) {
            QString taskName = QString::fromStdString(row[0].as<std::string>());
            QListWidgetItem *item = new QListWidgetItem(taskName, ui->listWidget);
            item->setFlags(item->flags() | Qt::ItemIsEditable);
        }
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Database Error", e.what());
    }
}

