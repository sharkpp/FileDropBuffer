#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMimeData>
#include <QDrag>
#include <QPainter>
#include <QPixmap>
#include <QFileInfo>
#include <QFileIconProvider>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setAcceptDrops(true);

    this->addToolBar(Qt::BottomToolBarArea, ui->toolBar);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->toolBar->addWidget(spacer);
    m_actTopMost = ui->toolBar->addAction(style()->standardIcon( QStyle::SP_ArrowUp ),
                           tr("top most"), this, SLOT(toggleTopMost()));
    ui->toolBar->addAction(style()->standardIcon( QStyle::SP_DialogDiscardButton ),
                           tr("clear"), this, SLOT(clearFileList()));

    toggleTopMost();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    qDebug() << event->mimeData()->formats();

    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        bool accepted = false;
        const QList<QUrl>& urlList = event->mimeData()->urls();
        for (int i = 0; i < urlList.size() && !accepted; ++i)
        {
            const QUrl& url = urlList.at(i);
            if (!url.toLocalFile().isEmpty() &&
                !m_pathList.contains(url))
            {
                accepted = true;
            }
        }

        if (accepted) {
            event->acceptProposedAction();
            ui->dropZone->setDropMode(true, m_pathList.size());
        }
    }
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent* event)
{
    ui->dropZone->setDropMode(m_pathList.isEmpty(), m_pathList.size());

    QMainWindow::dragLeaveEvent(event);
}

void MainWindow::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();

    // ファイルパスを含んでいるか？
    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();

        // ドロップ済みのファイル一覧へ追加
        for (int i = 0; i < urlList.size(); ++i)
        {
            const QUrl& url = urlList.at(i);
            if (!url.toLocalFile().isEmpty() &&
                !m_pathList.contains(url))
            {
                m_pathList.append(url);
            }
        }

        qDebug() << m_pathList;

        event->acceptProposedAction();

        ui->dropZone->setDropMode(m_pathList.isEmpty(), m_pathList.size());
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton &&
        ui->dropZone->geometry().contains(event->pos()) &&
        !m_pathList.empty())
    {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        // ドラッグ時に表示するアイコンを作成
        // ※ できれば look&feel を合わせるために OS 別に処理を実装したい

        QPixmap *img = new QPixmap(256, 256);
        img->fill(Qt::transparent);
        QPainter painter(img);

        QString fname = QFileInfo(m_pathList.first().toLocalFile()).fileName();
        QFileInfo info(m_pathList.first().toLocalFile());
        QFileIconProvider ip;
        QIcon icon=ip.icon(info);

        icon.paint(&painter, QRect(0,0,32,32), Qt::AlignCenter);

        QFont font = painter.font();

        QPen pen;
        pen.setWidth(1);
        pen.setStyle(Qt::SolidLine);
        pen.setBrush(Qt::transparent);
        painter.setPen(pen);

        QRect rect = QRect(32 + 2, 0, 256, 32);
        QRect boundingRect;
        painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, fname, &boundingRect);

        QColor highlightColor = palette().color(QPalette::Highlight);
        QColor highlightTextColor = palette().color(QPalette::HighlightedText);

        pen.setStyle(Qt::SolidLine);
        pen.setWidth(0);
        pen.setBrush(highlightColor);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(highlightColor);

        painter.setPen(pen);
        painter.setBrush(brush);
        boundingRect += QMargins(5, 0, 5, 0);
        boundingRect.moveTo(boundingRect.left() + 5, boundingRect.top());
        painter.drawRoundedRect(boundingRect, boundingRect.height()/2, boundingRect.height()/2);

        boundingRect -= QMargins(5, 0, 5, 0);

        pen.setBrush(highlightTextColor);
        painter.setPen(pen);
        painter.drawText(boundingRect, Qt::AlignLeft | Qt::AlignVCenter, fname, &boundingRect);

        painter.end();

        //mimeData->setText("aaaa");
        mimeData->setUrls(m_pathList);
        drag->setMimeData(mimeData);
        drag->setPixmap(*img);
       drag->setDragCursor(*img, Qt::MoveAction);

        Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);

        qDebug() << dropAction;
    }
}

void MainWindow::clearFileList()
{ // ドロップ済みのファイル一覧をクリア
    m_pathList.clear();
    ui->dropZone->setDropMode(true, m_pathList.size());
}

void MainWindow::toggleTopMost()
{ // 最前面の解除＆復帰
    bool isTopMost = 0 != (this->windowFlags() & Qt::WindowStaysOnTopHint);
    isTopMost = !isTopMost;

    m_actTopMost->setIcon(style()->standardIcon( isTopMost ? QStyle::SP_ArrowDown : QStyle::SP_ArrowUp ));

    this->setWindowFlags((this->windowFlags() & ~Qt::WindowStaysOnTopHint) |
                         (isTopMost ? Qt::WindowStaysOnTopHint : (Qt::WindowType)0));
    this->show();
}
