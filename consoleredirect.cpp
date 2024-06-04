#include "window.h"
#include <QVBoxLayout>
#include <iostream>
#include <streambuf>
#include <QKeyEvent>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    m_textEdit = new QTextEdit(this);
    setCentralWidget(m_textEdit);
    m_textEdit->installEventFilter(this);

    // Redirect cout and cerr to QTextEdit
    cout.rdbuf(new ostreambuf_iterator<char>(m_textEdit));
    cerr.rdbuf(new ostreambuf_iterator<char>(m_textEdit));

    // Print example output
    cout << "Hello, console!" << endl;
    cerr << "Error message" << endl;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == m_textEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
            // Handle user input
            QString input = m_textEdit->toPlainText().trimmed();
            // Process the input, for example:
            if (input == "quit") {
                close();
            }
            // Clear the input field
            m_textEdit->clear();
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}
