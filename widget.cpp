#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      m_horizontalOffset(0),
      m_verticalOffset(0),
      m_scaleFactor(1),
      m_currentStepScaleFactor(1),
      m_translateButton(Qt::LeftButton),
      m_bMouseTranslate(false),
      m_zoomDelta(0.2)
{
    /*通过点击设置焦点：点击当前窗口则当前窗口为焦点*/
    this->setFocusPolicy(Qt::ClickFocus);

    /*按标志抓取控件的手势*/
    grabGesture(Qt::PanGesture);    //平移手势
    grabGesture(Qt::PinchGesture);  //捏合手势
    grabGesture(Qt::SwipeGesture);  //滑动手势

    int a = 10;
    qreal b = 10.0;
    a = (int)b;
}

Widget::~Widget()
{
}

void Widget::setPicture(QImage &image)
{
    m_currentImage = image.convertToFormat(QImage::Format_RGB888);
    update();
}

bool Widget::event(QEvent *event)
{
    /*将事件交给自定义的手势事件处理*/
    if(event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));//Qt事件之间的转换要用 static_cast
    return QWidget::event(event);
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QImage image = m_currentImage;

    if(!image.isNull() /*&& 0.8 < m_scaleFactor && m_scaleFactor <= 4*/)
    {
        qDebug()<<"m_currentStepScaleFactor:"<<m_currentStepScaleFactor;
        qDebug()<<"m_scaleFactor:"<<m_scaleFactor;

        image = image.scaled(this->width() * m_currentStepScaleFactor * m_scaleFactor,
                             this->height()* m_currentStepScaleFactor * m_scaleFactor,
                             Qt::KeepAspectRatio,
                             Qt::SmoothTransformation);
    }

    const qreal iw = image.width();
    const qreal ih = image.height();
    const qreal wh = height();
    const qreal ww = width();

    painter.translate(ww/2, wh/2);
    painter.translate(m_horizontalOffset, m_verticalOffset);
    //painter.scale(currentStepScaleFactor * scaleFactor, currentStepScaleFactor * scaleFactor);
    painter.translate(-iw/2, -ih/2);
    painter.drawImage(0,0,image);
}

bool Widget::gestureEvent(QGestureEvent *event)
{
    if(QGesture *pan = event->gesture(Qt::PanGesture))  //平移
        panTriggered(static_cast<QPanGesture *>(pan));
    if(QGesture *pinch = event->gesture(Qt::PinchGesture))  //捏合手势
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    return true;
}

void Widget::panTriggered(QPanGesture *gesture)
{
//#ifndef QT_NO_CURSOR
    switch (gesture->state()) {
        case Qt::GestureStarted:
        case Qt::GestureUpdated:
            setCursor(Qt::SizeAllCursor);
            break;
        default:
            setCursor(Qt::ArrowCursor);
    }
//#endif
    QPointF delta = gesture->delta();
    m_horizontalOffset += delta.x();
    m_verticalOffset += delta.y();
    update();
}

void Widget::pinchTriggered(QPinchGesture *gesture)
{
    QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
    if(changeFlags & QPinchGesture::ScaleFactorChanged) //比例因子已经改变
    {
        m_currentStepScaleFactor = gesture->totalChangeFlags();
    }
    if(gesture->state()== Qt::GestureFinished)
    {
        m_scaleFactor *= m_currentStepScaleFactor;
        m_currentStepScaleFactor = 1;
    }
    update();
}

void Widget::resizeEvent(QResizeEvent *e)
{
    update();
    QWidget::resizeEvent(e);
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:
        translate(QPointF(0, -5));  //上移
        break;
    case Qt::Key_Down:
        translate(QPointF(0, 5));  //下移
        break;
    case Qt::Key_Left:
        translate(QPointF(-5, 0));  //左移
        break;
    case Qt::Key_Right:
        translate(QPointF(5, 0));  //右移
        break;
    case Qt::Key_Plus:  //放大
        zoomIn();
        break;
    case Qt::Key_Minus:  //缩小
        zoomOut();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
    QWidget::keyPressEvent(event);
}

// 平移
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bMouseTranslate){
        QPointF mouseDelta = event->pos() - m_lastMousePos;
        translate(mouseDelta);
    }

    m_lastMousePos = event->pos();

    QWidget::mouseMoveEvent(event);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == m_translateButton) {
        m_bMouseTranslate = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::OpenHandCursor);
    }

    QWidget::mousePressEvent(event);
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == m_translateButton)
    {
        m_bMouseTranslate = false;
        setCursor(Qt::ArrowCursor);
    }
    QWidget::mouseReleaseEvent(event);
}

// 放大/缩小
void Widget::wheelEvent(QWheelEvent *event)
{
    qDebug() << "Widget::wheelEvent";
    QPoint scrallAmount = event->angleDelta();
    if(scrallAmount.y() > 0){
        zoomIn();
    }
    else if(scrallAmount.y() < 0){
        zoomOut();
    }
    QWidget::wheelEvent(event);
}

void Widget::mouseDoubleClickEvent(QMouseEvent *)
{
    m_scaleFactor = 1;
    m_currentStepScaleFactor = 1;
    m_verticalOffset = 0;
    m_horizontalOffset = 0;
    update();
}

// 放大
void Widget::zoomIn()
{
    zoom(1 + m_zoomDelta);
}

// 缩小
void Widget::zoomOut()
{
    zoom(1 - m_zoomDelta);
}

// 缩放 - scaleFactor：缩放的比例因子
void Widget::zoom(float scale)
{
    if(0.8<= m_scaleFactor*scale && m_scaleFactor*scale<=4)
    {
        m_scaleFactor *= scale;
        update();
    }

}

// 平移
void Widget::translate(QPointF delta)
{
    m_horizontalOffset += delta.x();
    m_verticalOffset += delta.y();
    update();
}









