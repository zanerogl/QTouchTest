#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGestureEvent>    //手势事件
#include <QPanGesture>      //平移手势
#include <QPinchGesture>    //捏合手势
#include <QSwipeGesture>    //滑动手势
#include <QPainter>
#include <QDebug>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void setPicture(QImage & image);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    bool event(QEvent *event) override; //捕获手势事件
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *e) override;


public:
    void zoomIn();  //放大
    void zoomOut(); //缩小
    void zoom(float scale); //缩放 - scaleFactor：缩放的比例因子
    void translate(QPointF delta);  //平移

private:
    bool gestureEvent(QGestureEvent *event);
    bool gestureEvent(QEvent *event);
    void panTriggered(QPanGesture*);
    void pinchTriggered(QPinchGesture*);

    QImage m_currentImage;      //当前图片

    qreal m_horizontalOffset;   //水平偏移量
    qreal m_verticalOffset;     //垂直偏移量
    qreal m_scaleFactor;        //比例因子
    qreal m_currentStepScaleFactor;   //当前比例因子

    Qt::MouseButton m_translateButton;  //平移按钮
    bool m_bMouseTranslate;
    qreal m_zoomDelta;  //缩放的增量
    QPoint m_lastMousePos;  // 鼠标最后按下的位置
};
#endif // WIDGET_H
