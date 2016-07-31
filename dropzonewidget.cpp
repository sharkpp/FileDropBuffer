#include "dropzonewidget.h"
#include <QApplication>
#include <QStyle>
#include <QFontMetrics>
#include <QPainter>
#include <QPen>
#include <algorithm>

int AdjustFontSize(QFont* font, QString text, QRect rect, int fontSizeMin, int fontSizeMax) {
    if (fontSizeMax < fontSizeMin) {
        return -1;
    }
    int fontSize = fontSizeMin + (fontSizeMax - fontSizeMin) / 2;

    font->setPixelSize(fontSize);
    QFontMetrics fm(*font);
    int w = fm.width(text);
    int h = fm.height();

    if (rect.width()  < w ||
        rect.height() < h) {
        return AdjustFontSize(font, text, rect,  fontSizeMin, fontSize - 1);
    } else if (w < rect.width() &&
               h < rect.height()) {
        return AdjustFontSize(font, text, rect, fontSize + 1, fontSizeMax);
    } else {
        return fontSize;
    }
}

DropZoneWidget::DropZoneWidget(QWidget *parent)
    : QWidget(parent)
    , m_dropMode(true)
    , m_droppedCount(0)
{

}

void DropZoneWidget::setDropMode(bool yes, int num)
{
    m_dropMode = yes;
    m_droppedCount = num;

    repaint();
}

void DropZoneWidget::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);

    QRect contentRect = this->rect();

    if (m_dropMode)
    { // ファイルのドロップエリアを描画
        int lineWidth = 5;
        if (contentRect.width() < contentRect.height()) {
            int virtCenter = contentRect.top() + contentRect.height() / 2;
            contentRect.setTop(virtCenter - contentRect.width() / 2);
            contentRect.setBottom(virtCenter - contentRect.width() / 2 + contentRect.width());
        } else {
            int horzCenter = contentRect.left() + contentRect.width() / 2;
            contentRect.setLeft(horzCenter - contentRect.height() / 2);
            contentRect.setRight(horzCenter - contentRect.height() / 2 + contentRect.height());
        }
        contentRect -= QMargins((lineWidth+1)/2, (lineWidth+1)/2, (lineWidth+1)/2, (lineWidth+1)/2);
        int roundSize = 25;//(std::max)(25, (std::max)(contentRect.width(), contentRect.height()) / 5);

        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(lineWidth);
        pen.setBrush(Qt::gray);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);

        // 点線枠を描画
        painter.setPen(pen);
        painter.drawRoundRect(contentRect, roundSize, roundSize);

        QFont font = painter.font();
        font.setPixelSize(32);
        font.setBold(true);
        painter.setFont(font);

        // 中の文字を描画
        QRect rectangle = contentRect;
              rectangle -= QMargins(lineWidth, lineWidth, lineWidth, lineWidth);
        QRect boundingRect;
        painter.drawText(rectangle, Qt::AlignCenter | Qt::TextWordWrap, tr("Drop file here"), &boundingRect);
    }
    else
    { // ドロップ済み状態を描画
        QIcon icon = style()->standardIcon( QStyle::SP_FileIcon );
        QSize iconSize = icon.actualSize(contentRect.size());

        QRect contentRect2 = contentRect;
              contentRect2.moveTopLeft(QPoint(10, 10));
        icon.paint(&painter, contentRect, Qt::AlignCenter);
        icon.paint(&painter, contentRect2, Qt::AlignCenter);

        QSize badgeSize = QSize(30, 30);
        QRect rectangle = contentRect;
              rectangle.setRect(contentRect.center().x() - badgeSize.width() / 2,
                                contentRect.center().y() - badgeSize.height() / 2,
                                badgeSize.width(), badgeSize.height());
              rectangle.moveCenter(QPoint(contentRect.center().x() - iconSize.width() / 2 + iconSize.width(),
                                          contentRect.center().y() - iconSize.height() / 2 + badgeSize.height() / 2));

        QPen pen;
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(1);
        pen.setBrush(Qt::red);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::red);

        // 背景を描画
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.drawRoundedRect(rectangle, badgeSize.width()/4, badgeSize.width()/4);

        QString badgeText = QString("%1").arg(m_droppedCount);
        QFont font = painter.font();

        rectangle -= QMargins(2, 2, 2, 2);
        int fontSize = AdjustFontSize(&font, badgeText, rectangle, 10, 60);
        font.setPixelSize(fontSize);
        painter.setFont(font);

        pen.setWidth(1);
        pen.setBrush(Qt::white);
        painter.setPen(pen);

        // ドロップ済みのファイル数を描画
        QRect boundingRect;
        painter.drawText(rectangle, Qt::AlignCenter | Qt::TextWordWrap, badgeText, &boundingRect);

    }
}
