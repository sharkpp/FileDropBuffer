#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDropEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void dragEnterEvent(QDragEnterEvent* event);
    void dragLeaveEvent(QDragLeaveEvent* event);
    void dropEvent(QDropEvent* event);

    void mousePressEvent(QMouseEvent *event);

protected:

public slots:
    void clearFileList();
    void toggleTopMost();

private:
    Ui::MainWindow *ui;
    QList<QUrl> m_pathList;
    QAction* m_actTopMost;
};

#endif // MAINWINDOW_H
