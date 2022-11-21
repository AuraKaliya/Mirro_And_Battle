#ifndef WAITINGWIDGET_H
#define WAITINGWIDGET_H

#include <QWidget>
#include <QDesktopWidget>
#include <QPoint>
#include <QMovie>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <QPixmap>
#pragma execution_character_set("utf-8")
class waitingWidget : public QWidget
{
    Q_OBJECT
public:

    //这是一个等待界面，采用QMovie和QLabel充满窗体作为等待界面动图，通过设置覆盖QPixmap作为匹配成功或失败的标志
    //为了增加自动性，这个界面调用start（）后会开启计时器，预设置了等待时间，当等待超过这个时间后会发送停止等待的信号，然后自动加载匹配失败的界面
    //但如果在这个时间里匹配成功，也是可以进入的
    //匹配成功后会加载匹配成功的信号并在1.2s后自动关闭窗体，所以如果匹配超时的时候匹配成功了然后在1.2s内关闭了窗体，消息框内会同时提示匹配失败和匹配成功，但最终是匹配成功了
    //这属于设计失误，现在想来应该完全由服务器发送指令来改变客户端的这个状态而不是让双端都对这个状态有控制力，对于这方面的考虑还需要仔细推敲一下
    //不过这属于学习性的制作，最终是要迁移到《缘亦不圆》的联网对战功能中，到时候再自习考虑反复测试吧，该作品的学习目的已经达成了。

    explicit waitingWidget(QWidget *parent = nullptr);
    void start();
    void linkLose();
    void linkSuccess();


private:

    int idx;                                                    //计数器 --关联计时器等待时间
    QPushButton *returnBtn;                                     //用btn的点击来发信号实现主动关闭的功能，设置为透明的就行了（实际上是懒得再做一个可点击的label类）
    QPixmap lose;                                               //匹配失败时候加载的图
    QPixmap success;                                            //匹配成功时加载的图
    QLabel* lb;                                                 //用于填充整个窗体的label 承载动图
    QMovie *movie;                                              //动图
    QTimer* tim;                                                //计时器

signals:
     void stopWaiting();

private slots:
    void stopWait();
    void waitingCount();
};

#endif // WAITINGWIDGET_H
