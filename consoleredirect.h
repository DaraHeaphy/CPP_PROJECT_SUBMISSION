#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H

#include <QMainWindow>
#include <QTextEdit>

class ConsoleWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ConsoleWindow(QWidget *parent = nullptr);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QTextEdit* m_textEdit;
};

#include "consolewindow.cpp" // Include the source file here for template instantiation

#endif // CONSOLEWINDOW_H
