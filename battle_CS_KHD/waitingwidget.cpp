#include "waitingwidget.h"

waitingWidget::waitingWidget(QWidget *parent) : QWidget(parent)
{



    resize(400,325);                                //这是那个动图的大小  --实际上也是懒得查自动适应大小的函数（用了这么多次也没记住，不愧是复制党）
    lb=new QLabel(this);                            //
    lb->setGeometry(0,0,400,325);
    setWindowFlag(Qt::FramelessWindowHint);         //设置无边框  --记是记不住的 下次要用的时候要么百度查要么会回来翻这个代码
    setAttribute(Qt::WA_TranslucentBackground);     //透明



    returnBtn=new QPushButton(this);
    returnBtn->setGeometry(0,0,400,325);
    returnBtn->setVisible(false);
    returnBtn->setAttribute(Qt::WA_TranslucentBackground);
    returnBtn->setStyleSheet("background-color:rgba(255,255,255,0);");



    int frmX=width();
    int frmY=height();

    QDesktopWidget w;
    int deskW=w.width();
    int deskH=w.height();

    QPoint moveP(deskW/4-frmX/4,deskH/4-frmY/4);         //父类窗体的4分点为左上角
    move(moveP);

    lose.load(":/other/waitinglose.png");                //预加载静态资源的方式真不错
    success.load(":/other/waitingsuccess.png");


    movie =new QMovie(":/other/wait.gif");
    lb->setMovie(movie);
    movie->start();
    tim=new QTimer();
    tim->setInterval(1000);

    idx=0;
    connect(returnBtn,&QPushButton::clicked,this,&waitingWidget::stopWait);
    connect(tim,&QTimer::timeout,this,&waitingWidget::waitingCount);

    close();                                             //到时候在主窗体的构造函数中进行new 用的时候再show出来
}

void waitingWidget::start()
{
    movie->start();
    tim->start();                                       //开始计时   ---想起来最初版本的文本逐字加载是返回了timer在主函数中调用 www成长了
}

void waitingWidget::linkLose()
{
    lb->setPixmap(lose);                                //匹配失败了T T
    returnBtn->setVisible(true);
}

void waitingWidget::linkSuccess()                       //0V0 有人！匹配成功了！
{
    lb->setPixmap(success);
    idx=0;
    tim->stop();
   // returnBtn->setVisible(true);
}

void waitingWidget::stopWait()
{

    emit stopWaiting();
    close();
}

void waitingWidget::waitingCount()
{
    idx++;
    if(idx>=10)
    {
        idx=0;
        linkLose();
        tim->stop();

    }
}


