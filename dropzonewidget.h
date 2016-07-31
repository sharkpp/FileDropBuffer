#ifndef DROPZONEWIDGET_H
#define DROPZONEWIDGET_H

#include <QWidget>
#include <QPaintEvent>

class DropZoneWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DropZoneWidget(QWidget *parent = 0);

    void paintEvent(QPaintEvent* event);

    void setDropMode(bool yes, int num);

signals:

public slots:

private:
    bool m_dropMode;
    int m_droppedCount;
};

#endif // DROPZONEWIDGET_H
