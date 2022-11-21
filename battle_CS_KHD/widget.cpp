#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle(QString("战斗测试客户端"));
//    setWindowIcon(QIcon)
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    userID="1";

    socket=new QTcpSocket();
    socket->setProxy(QNetworkProxy::NoProxy);
    tim=new QTimer();
    tim->setInterval(1000);
    connect(ui->linkBtn,SIGNAL(clicked()),this,SLOT(linkToServer()));

    btnL<<ui->ATK<<ui->DEF<<ui->REC<<ui->send<<ui->linkBtn<<ui->finishedTalkBtn<<ui->matchBtn;

    for (int i=0;i<btnL.size();i++) {
        btnL[i]->setStyleSheet(st.style_buttonLabel());
    }


//    ui->headLabel->setScaledContents(true);
//    ui->headLabel->setPixmap(headPic);







    //设置血条
    mineHPRect.setRect(20,20,400,50);
    oppoHPRect.setRect(780,20,400,50);

    mineHP=HPmax;
    oppoHP=HPmax;

    mineRect.setRect(30,80,237,420);
    oppoRect.setRect(903,80,237,420);
    mineHPMaxRect=mineHPRect;
    oppoHPMaxRect=oppoHPRect;

    timerLabel=new QLabel(this);
    timerLabel->setGeometry(500,0,200,110);
    timerLabel->setText(" 时间 ");
    timerLabel->setStyleSheet("font:50 50pt;background-color:argb(255,47,17,0);color:rgb(255,255,255);");


    ui->ATK->setEnabled(false);
    ui->DEF->setEnabled(false);
    ui->REC->setEnabled(false);
    ui->send->setEnabled(false);



    backgroundImage.load(QString(":/backgroundImage/background_"+QString::number(qrand()%6)+".png"));
    headPic.load(QString(":/characterImage/"+QString::number((qrand()%23)%2)+"_head.png"));
    headRect.setRect(ui->headLabel->x(),ui->headLabel->y(),ui->headLabel->width(),ui->headLabel->height());
    headBondRect.setRect(ui->headLabel->x()-10,ui->headLabel->y()-10,ui->headLabel->width()+30,ui->headLabel->height()+30);
    headBondPic.load(":/other/bond1.png");
    ui->headLabel->setVisible(false);
    ui->label->setStyleSheet("color:rgb(255,255,255);");









//    waitBox.setWindowTitle("正在连接中");
//    waitBox.setTextFormat(Qt::TextFormat(1));

   waitingBox=new waitingWidget(this);


    connect(ui->linkBtn,SIGNAL(clicked()),this,SLOT(linkToServer()));
    connect(ui->send,SIGNAL(clicked()),this,SLOT(sendTalk()));
    connect(tim,SIGNAL(timeout()),this,SLOT(onTimeOut()));
    connect(ui->ATK,SIGNAL(clicked()),this,SLOT(ATK_On()));
    connect(ui->DEF,SIGNAL(clicked()),this,SLOT(DEF_On()));
    connect(ui->REC,SIGNAL(clicked()),this,SLOT(REC_On()));
    connect(waitingBox,&waitingWidget::stopWaiting,this, &Widget::matchFailed);
    connect(ui->matchBtn,&QPushButton::clicked,this,&Widget::matchToPlayer);
    connect(socket,&QTcpSocket::readyRead,this,&Widget::readDataFromSever);




}


Widget::~Widget()
{
    delete ui;
}

void Widget::gameEnd(QString winner)
{

    if(winner==mineName)
    {
        QString tmpText="<h1>恭喜你获得本次战斗胜利！</h1>";
        tipBox.setText(tmpText);
        tipBox.setStandardButtons(QMessageBox::Ok);
        tipBox.setButtonText(QMessageBox::Ok,QString("区区蝼蚁不足挂齿"));
    }

    else {
        QString tmpText="<h1>很遗憾这次战斗的胜者是"+winner+"!</h1>";
        tipBox.setText(tmpText);
        tipBox.setStandardButtons(QMessageBox::Ok);
        tipBox.setButtonText(QMessageBox::Ok,QString("三十秒河东三十秒河西莫欺少年穷！"));
    }
    tipBox.show();
    resets();

}

void Widget::showHP()
{
    QString tmpstr= ui->informationShow->toPlainText();
    tmpstr+=QString("\nMe:"+QString::number(mineHP)+"\nOp:"+QString::number(oppoHP));
    ui->informationShow->setText(tmpstr);

}

void Widget::nextTurn()
{
    turnTime=31;

    ui->ATK->setEnabled(true);
    //if(lastStrategy!="DEF")
    ui->DEF->setEnabled(true);
    ui->REC->setEnabled(true);

    tim->start();
}

void Widget::initBattle()
{
        //客户端的战斗初始化，只需要让血条和立绘初始化即可
    mineHP=HPmax;
    oppoHP=HPmax;
    mineImage.load(QString(":/characterImage/"+mineName+"_lh.png"));
    oppoImage.load(QString(":/characterImage/"+oppoName+"_lh.png"));
//    backgroundImage.load(QString(":/backgroundImage/background_"+QString::number(qrand()%6)+".png"));
//    headPic.load(QString(":/characterImage/"+QString::number(qrand()%2)+"_head.png"));
//    ui->headLabel->setScaledContents(true);
//    ui->headLabel->setPixmap(headPic);
    qDebug()<<mineImage.isNull()<<"   "<<oppoImage.isNull();
    QString tmpStr="镜花缘世界的战斗又一次打响了，这次是"+mineName+"对战"+oppoName+"。\n桂冠究竟花落谁家呢，让我们拭目以待！";
    ui->informationShow->setText( ui->informationShow->toPlainText()+"\n"+tmpStr);
    nextTurn();
}

void Widget::timerStop()
{
    tim->stop();
    timerLabel->setText(QString("等待中"));
}

void Widget::resets()
{
    timerLabel->setText(QString("时间"));
    mineImage.load("");
    oppoImage.load("");
    mineHP=5;
    oppoHP=5;
    QSize si1((mineHPMaxRect.width()*mineHP)/HPmax,mineHPMaxRect.height());
    mineHPRect.setSize(si1);
    QSize si2((oppoHPMaxRect.width()*oppoHP)/HPmax,oppoHPMaxRect.height());
    oppoHPRect.setSize(si2);
    update();




}

void Widget::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);




    painter.drawPixmap(this->rect(),backgroundImage);

    QPen pen;

    pen.setColor(Qt::darkRed);
    pen.setWidth(3);
    painter.setPen(pen);

    painter.drawPixmap(headRect,headPic);
    painter.drawPixmap(headBondRect,headBondPic);



   // painter.drawRect(mineRect);
   // painter.drawRect(oppoRect);
    painter.drawPixmap(mineRect,mineImage);
    painter.drawPixmap(oppoRect,oppoImage);

    painter.drawRect(mineHPMaxRect);
    painter.drawRect(oppoHPMaxRect);

    painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
    painter.drawRect(mineHPRect);
    painter.drawRect(oppoHPRect);


    painter.end();

}

void Widget::onTimeOut()
{
    if(turnTime>=0)
    {
        turnTime--;
        timerLabel->setText(QString::number(turnTime));
    }else
    {
        timerStop();
        ui->ATK->setEnabled(false);
        ui->DEF->setEnabled(false);
        ui->REC->setEnabled(false);
        socket->write(QString(ins.inst(ins.ATK())).toUtf8());
        socket->flush();
    }
}

void Widget::linkToServer()
{
    QString IP;
    IP=ui->serverPortText->toPlainText();
    socket->abort();
    socket->connectToHost(QHostAddress(IP),port);

    if(!socket->waitForConnected(10000))
    {
        ui->informationShow->setText("Connection failed!");
            return;
    }
    ui->informationShow->setText("Connect successfully!");
    ui->label->setText(QString("连接成功！"));
    ui->send->setEnabled(true);



}

void Widget::sendTalk()
{
    socket->write(QString(ins.inst(ins.SendTalk())+ui->talkText->toPlainText()).toUtf8());
    socket->flush();    //清空缓存把消息发送到服务端
}

void Widget::readDataFromSever()
{
    QByteArray buffer;
    buffer=socket->readAll();
    qDebug()<<"resect:"<<buffer;
    if(!buffer.isEmpty())
    {
        QString tmpStr= tr(buffer);
        qDebug()<<"resect:"<<tmpStr;
        QStringList tmpStList=tmpStr.split(QRegExp("[| ]"));
        if(tmpStList[0]==ins.MEHP())
        {
            mineHP=tmpStList[1].toInt();
        }
        else if (tmpStList[0]==ins.OPHP())
        {
            oppoHP=tmpStList[1].toInt();
        }
        else if (tmpStList[0]==ins.NextTurn())
        {
            showHP();
            QSize si1((mineHPMaxRect.width()*mineHP)/HPmax,mineHPMaxRect.height());
            mineHPRect.setSize(si1);
            QSize si2((oppoHPMaxRect.width()*oppoHP)/HPmax,oppoHPMaxRect.height());
            oppoHPRect.setSize(si2);
            update();
            turnNumber=tmpStList[1].toInt();
            nextTurn();
        }
        else if(tmpStList[0]==ins.Winner())
        {
            QString tmpStr=QString("战斗结束！");
            ui->informationShow->setText( ui->informationShow->toPlainText()+"\n"+tmpStr);
            gameEnd(tmpStList[1]);
        }
        else if(tmpStList[0]==ins.initBattle())
        {
            mineName=tmpStList[1];
            oppoName=tmpStList[2];
            initBattle();
        }else if(tmpStList[0]==ins.matchSuccess())
        {
            mineName=tmpStList[1];
            oppoName=tmpStList[2];
            matchSuccessed();
            update();
        }
        else if(tmpStList[0]==ins.setIdx())
        {
            userID=tmpStList[1];
            ui->label->setText(userID);
        }
        else
        {
           ui->informationShow->setText( ui->informationShow->toPlainText()+"\n"+tmpStr);
        }

        socket->write(QString(userID+" received ").toUtf8()+buffer);
        socket->flush();

    }
}

void Widget::matchToPlayer()
{
    //点击匹配，弹出一个消息框选择人物；---下次一定
    //向服务器发送一个请求后本端进入等待，直到服务器发来匹配成功的命令
    //随即初始化战斗双方

    socket->write(QString(ins.inst(ins.match())).toUtf8());
    socket->flush();
    waitingBox->start();
    waitingBox->show();



}

void Widget::matchSuccessed()
{
    //匹配成功 消息框取消 在text中显示对手名字
    waitingBox->linkSuccess();
    QString tmpStr="匹配成功！";
    ui->informationShow->setText( ui->informationShow->toPlainText()+"\n"+tmpStr);

    QTime time=QTime::currentTime().addMSecs(1200);
    while(QTime::currentTime()<time)
    {
        QCoreApplication::processEvents();
    }

    waitingBox->close();
    initBattle();
}

void Widget::matchFailed()
{
    //匹配失败 消息框取消 text中显示提示
    socket->write(QString(ins.inst(ins.stopMatch())).toUtf8());
    socket->flush();
    QString tmpStr="匹配失败！";
    ui->informationShow->setText( ui->informationShow->toPlainText()+"\n"+tmpStr);

}

void Widget::ATK_On()
{
    socket->write(QString(ins.inst(ins.ATK())).toUtf8());
    socket->flush();

    lastStrategy="ATK";
    ui->ATK->setEnabled(false);
    ui->DEF->setEnabled(false);
    ui->REC->setEnabled(false);
    timerStop();
}

void Widget::DEF_On()
{
    socket->write(QString(ins.inst(ins.DEF())).toUtf8());
    socket->flush();

    lastStrategy="DEF";
    ui->ATK->setEnabled(false);
    ui->DEF->setEnabled(false);
    ui->REC->setEnabled(false);
     timerStop();
}

void Widget::REC_On()
{
    socket->write(QString(ins.inst(ins.REC())).toUtf8());
    socket->flush();

    lastStrategy="REC";
    ui->ATK->setEnabled(false);
    ui->DEF->setEnabled(false);
    ui->REC->setEnabled(false);
    timerStop();
}


void Widget::on_informationShow_textChanged()
{
    ui->informationShow->moveCursor(QTextCursor::End);
}
