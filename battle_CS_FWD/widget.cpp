#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    //采用广播-订阅模式向连接上的客户端广播在线人
    chooseIdx=-1;
    nowIdx=qrand()%23333;
    matchCount=0;
    battleHomeSock=false;
    matchListSock=false;
    //这部分考虑用mysql或qsql来弄
   // QString tmpstr[5]={" 穆雷尔 "," 卡罗 "," 莉莉安·缄默 "," 丽娜思 "," 胭脂 "};
    characterNameTest<<QString("穆雷尔")<<QString("卡罗")<<QString("莉莉安·缄默")
                    <<QString("丽娜思")<<QString("胭脂");

    server = new QTcpServer();
    server->listen(QHostAddress::Any,23333);
    connect(server,&QTcpServer::newConnection,this,&Widget::serverNewConnect);
    connect(ui->sendTalkBtn,&QPushButton::clicked,this,&Widget::sendTalkToAll);



}

Widget::~Widget()
{
    delete ui;
}

void Widget::sendUserIdx(QTcpSocket *so, QString st)
{
    so->write(QString(ins.inst(ins.setIdx())+st).toUtf8());
    so->flush();

}

void Widget::serverNewConnect()
{
   user * tmpUser=new user();
   tmpUser->setMySocket(server->nextPendingConnection());  //  ---连接
   connect(tmpUser->getMySocket(),&QTcpSocket::disconnected,this,&Widget::soDisconnected);  //  --脱机
   tmpUser->setFather(this);    //  --关联服务器
   tmpUser->changeState(1);         // 登录状态
   tmpUser->setUserID("visitor"+QString::number(nowIdx));
   userList.insert("visitor"+QString::number(nowIdx),tmpUser);
   qDebug()<<"1";
   ui->logList->setText(ui->logList->toPlainText()+"\n"+QString(tmpUser->getUserID()+" 连接成功！ "));
   ui->onlineList->setText(ui->onlineList->toPlainText()+"\n"+tmpUser->getUserID());
   //tmpUser->linkWithReadData();
   connect(tmpUser->getMySocket(),&QTcpSocket::readyRead,tmpUser,&user::readDatas);

   tmpUser->getMySocket()->write(QString(ins.inst(ins.setIdx())+tmpUser->getUserID()).toUtf8());
   tmpUser->getMySocket()->flush();

   nowIdx++;
}

void Widget::soDisconnected()
{

}

void Widget::battleTurnEnd(battleThread *b)
{

    //线程的一个回合结束了
    qDebug()<<"线程的一个回合结束了";
    b->getPlayer1()->getMySocket()->write(QString(QString(b->getPlayer1()->getCharacterName()+"进行了"+QString(b->getPlayer1()->getStrategy())+"！")
                                                  +"\n"
                                                  +QString(b->getPlayer2()->getCharacterName()+"进行了"+QString(b->getPlayer2()->getStrategy())+"！")
                                                  ).toUtf8());

    b->getPlayer1()->getMySocket()->flush();

    bool flag_do=false;
     QTime time1=QTime::currentTime().addMSecs(200);
    while(QTime::currentTime()<time1)
   {
        QCoreApplication::processEvents();
        if(!flag_do)
        {
            flag_do=true;
            b->getPlayer2()->getMySocket()->write(QString(QString(b->getPlayer1()->getCharacterName()+"进行了"+QString(b->getPlayer1()->getStrategy())+"！")
                                                          +"\n"
                                                          +QString(b->getPlayer2()->getCharacterName()+"进行了"+QString(b->getPlayer2()->getStrategy())+"！")
                                                          ).toUtf8());

            b->getPlayer2()->getMySocket()->flush();

        }

    }


  b->getPlayer1()->getMySocket()->write(    QString(
                                                ins.inst(ins.MEHP())
                                               +QString::number(b->getPlayer1()->getHPNow()))
                                            .toUtf8()
              );
  b->getPlayer1()->getMySocket()->flush();

  flag_do=false;
  time1=QTime::currentTime().addMSecs(200);
    while(QTime::currentTime()<time1)
   {
        QCoreApplication::processEvents();
        if(!flag_do)
        {
            flag_do=true;
            b->getPlayer2()->getMySocket()->write(    QString(
                                                          ins.inst(ins.MEHP())
                                                         +QString::number(b->getPlayer2()->getHPNow()))
                                                      .toUtf8()
                        );
            b->getPlayer2()->getMySocket()->flush();

        }

    }


  b->getPlayer1()->getMySocket()->write(    QString(
                                                ins.inst(ins.OPHP())
                                               +QString::number(b->getPlayer2()->getHPNow()))
                                            .toUtf8()
              );
  b->getPlayer1()->getMySocket()->flush();

  flag_do=false;
  time1=QTime::currentTime().addMSecs(200);
      while(QTime::currentTime()<time1)
     {
          QCoreApplication::processEvents();
          if(!flag_do)
          {
              flag_do=true;
              b->getPlayer2()->getMySocket()->write(    QString(
                                                            ins.inst(ins.OPHP())
                                                           +QString::number(b->getPlayer1()->getHPNow()))
                                                        .toUtf8()
                          );
              b->getPlayer2()->getMySocket()->flush();
          }

      }


  b->getPlayer1()->getMySocket()->write(   QString(
                                                ins.inst(ins.NextTurn())
                                               +QString::number(0)
                                               )
                                           .toUtf8()
                                        );
  b->getPlayer1()->getMySocket()->flush();


//  b->getPlayer2()->getMySocket()->write(    QString(
//                                                ins.inst(ins.MEHP())
//                                               +QString::number(b->getPlayer2()->getHPNow()))
//                                            .toUtf8()
//              );
//  b->getPlayer2()->getMySocket()->flush();

//  b->getPlayer2()->getMySocket()->write(    QString(
//                                                ins.inst(ins.OPHP())
//                                               +QString::number(b->getPlayer1()->getHPNow()))
//                                            .toUtf8()
//              );
//  b->getPlayer2()->getMySocket()->flush();


  b->getPlayer2()->getMySocket()->write(   QString(
                                                ins.inst(ins.NextTurn())
                                               +QString::number(0)
                                               )
                                           .toUtf8()
                                        );
  b->getPlayer2()->getMySocket()->flush();

}

void Widget::sendWinner(user *w)
{
    //这个接收有点问题 名字一样就全胜利了---该改成userID  --只要初始化不一样就行了；
    w->getMySocket()->write(    QString(
                                    ins.inst(ins.Winner())
                                    +w->getCharacterName()
                                    )
                                .toUtf8()

                );
    w->getMySocket()->flush();

    w->getOppo()->getMySocket()->write(QString(
                                           ins.inst(ins.Winner())
                                           +w->getCharacterName()
                                           )
                                       .toUtf8()
                );
    w->getOppo()->getMySocket()->flush();

}
void Widget::readData(QString data)
{
    //读取消息时，进行正则分割
    QStringList stl=data.split(QRegExp("[ :|]"));
    QString userId=stl[0];
    qDebug()<<data<<endl;
    qDebug()<<stl[1];
    //后续用map和switch优化
    if(stl[1]==ins.ATK())
    {
        userList.value(userId)->setStrategy(QString("ATK"));
        userList.value(userId)->setBattleState(true);
//        if(userList.value(userId)->getBox()->wait()==false)
//            userList.value(userId)->getBox()->settle();
    }
    else if(stl[1]==ins.DEF())
    {
        userList.value(userId)->setStrategy(QString("DEF"));
        userList.value(userId)->setBattleState(true);
    }
    else if(stl[1]==ins.REC())
    {
        userList.value(userId)->setStrategy(QString("REC"));
        userList.value(userId)->setBattleState(true);
    }
    else if(stl[1]==ins.SendTalk())
    {
        QString tmps="";
        for(int i=2;i<stl.size();i++)
        {
            tmps+=stl[i];
        }

        sendTalkToAllP(QString(userId+":"+tmps));
        senTalk(userList.value(userId),userList.value(userId)->getObj(),tmps);
    }
    else if(stl[1]==ins.match())
    {
        matching(userList.value(userId));
    }
    else if(stl[1]==ins.stopMatch())
    {
        //服务器自动回执 --应该不用管
    }

    ui->logList->setText(ui->logList->toPlainText()+"\n"+data);


}

void Widget::sendTalkToAll()
{
    QString st=ui->talkText->toPlainText();
    for (QMap<QString,user*>::const_iterator it = userList.constBegin(); it != userList.constEnd(); it++) {
           qDebug() << it.key() << ": " << it.value();
            it.value()->getMySocket()->write(QString("服务器："+st).toUtf8());
                it.value()->getMySocket()->flush();
                ui->logList->setText(ui->logList->toPlainText()+QString("\n服务器To")+it.value()->getUserID()+":"+st);
       }

}

void Widget::sendTalkToAllP(QString s)
{
    QString st= s;
    for (QMap<QString,user*>::const_iterator it = userList.constBegin(); it != userList.constEnd(); it++) {
           qDebug() << it.key() << ": " << it.value();
            it.value()->getMySocket()->write(QString(""+st).toUtf8());
                it.value()->getMySocket()->flush();
                ui->logList->setText(ui->logList->toPlainText()+QString("\n服务器To")+it.value()->getUserID()+":"+st);
       }




}




void Widget::senTalk(user *Src, user *Des, QString str)
{




}


void Widget::matching(user *u)
{
    //是否在等待序列
    //不是则添加  然后进入等待
    //等待结束退出函数

    if(!matchList.contains(u))
    {
        matchList<<u;

        QTime time=QTime::currentTime().addMSecs(10500);
        while(QTime::currentTime()<time&&matchList.contains(u))
        {
            QCoreApplication::processEvents();
            if(!battleHomeSock&&!matchListSock) //如果没上锁就上锁 然后添加修改
            {
                battleHomeSock=true;
                matchListSock=true;

                if(matchList.size()>=2)
                {

                    QList <QString > battleUserID;
                    battleUserID<<matchList[0]->getUserID()<<matchList[1]->getUserID();

                    //先行执行内容
                    matchList[0]->setOppo(matchList[1]);     //初始化对手
                    matchList[1]->setOppo(matchList[0]);

                    int tmp1=qrand()%characterNameTest.size();
                    int tmp2=qrand()%characterNameTest.size();

                    while(tmp1==tmp2) tmp2=qrand()%characterNameTest.size();

                    matchList[0]->setCharacterName(characterNameTest[tmp1]);      //初始化角色名称
                    matchList[1]->setCharacterName(characterNameTest[tmp2]);
                    bat=new battleThread();
                    bat->setPlayer1(matchList[0]);
                    bat->setPlayer2(matchList[1]);



                    matchSuccess(matchList[0]);     //回执 匹配成功
                    matchSuccess(matchList[1]);

                      matchList.pop_front();        //移除匹配序列
                      matchList.pop_front();

                    battleHome<<battleUserID;
                    connect(bat,&battleThread::nextTurn,this,&Widget::battleTurnEnd);
                    connect(bat,&battleThread::battleEnd,this,&Widget::sendWinner);
                    bat->start();                      //移入战斗序列
                }
                matchListSock=false;
                battleHomeSock=false;

                QTime time3=QTime::currentTime().addMSecs(30);
                while(QTime::currentTime()<time3)
                {
                }
            }else
            {       //该循环需要等待30ms
                 QTime time2=QTime::currentTime().addMSecs(30);
                 while(QTime::currentTime()<time2)
                 {
                 }

            }
        }
        //结束循环后，判断是否匹配成功 --依据是队列里是否含有user
        bool flagBat=false;
        for(int i=0;i<battleHome.size();i++)
        {
            if(battleHome[i].contains(u->getUserID()))
            {
                flagBat=true;
                break;
            }
        }
        if(flagBat)//进入了战斗序列
        {
            //应该是不用管了
        }
        else{
            //否则 让用户停止匹配（顺带着表明匹配失败）
              while(matchList.contains(u))
              {
                  QCoreApplication::processEvents();
                  if(!matchListSock)
                  {
                      matchListSock=true;

                      matchList.removeOne(u);
                      stopMatching(u);
                      matchListSock=false;
                  }else
                  {
                      QTime time4=QTime::currentTime().addMSecs(30);
                      while(QTime::currentTime()<time4)
                      {
                      }
                  }
              }
        }
    }
}

void Widget::stopMatching(user *u)
{
    //回执匹配失败信号
    //这个好像在客户端里实现了 应该可以不用管了
}

void Widget::matchSuccess(user *u)
{
    //回执匹配成功 --对战斗数据初始化；
    u->setHPmax(5);
    u->setHPNow(5);
    qDebug()<<"初始化了！";
    u->getMySocket()->write(    QString(ins.inst(ins.matchSuccess())
                                    +ins.inst(u->getCharacterName())
                                    +u->getOppo()->getCharacterName())
                                    .toUtf8()   );
    u->getMySocket()->flush();


}

int Widget::findFreeThread()
{



return 0;
}

void Widget::paintEvent(QPaintEvent *event)
{

}

/**********************************************************/

user::user(QObject *parent) : QObject(parent)
{
    mySocket=new QTcpSocket();
    userState=0;
    battleState=false;
    HPNow=5;
    HPmax=5;



}

void user::sendInformation(QString st)
{
    qDebug()<<st;
    mySocket->write(st.toUtf8());
    mySocket->flush();
}

void user::linkWithReadData()
{
   // connect(this->getMySocket(),&QTcpSocket::readyRead,this,&user::readDatas);
}

QTcpSocket *user::getMySocket() const
{
    return mySocket;
}

void user::setMySocket(QTcpSocket *value)
{
    mySocket = value;
}

void user::changeState(int i)
{
    userState=i;
}

user *user::getOppo() const
{
    return oppo;
}

void user::setOppo(user *value)
{
    oppo = value;
}

QString user::getUserID() const
{
    return userID;
}

void user::setUserID(const QString &value)
{
    userID = value;
}

void user::setFather( Widget *fa)
{
    father=fa;
}

QString user::getStrategy() const
{
    return strategy;
}

void user::setStrategy(const QString &value)
{
    strategy = value;
    qDebug()<<"成功设置！";
}

int user::getHPNow() const
{
    return HPNow;
}

void user::setHPNow(int value)
{
    HPNow = value;
}

int user::getHPmax() const
{
    return HPmax;
}

void user::setHPmax(int value)
{
    HPmax = value;
}

user *user::getObj() const
{
    return obj;
}

void user::setObj(user *value)
{
    obj = value;
}

QString user::getCharacterName() const
{
    return characterName;
}

void user::setCharacterName(const QString &value)
{
    characterName = value;
}

bool user::getBattleState() const
{
    return battleState;
}

void user::setBattleState(bool value)
{
    battleState = value;
}

void user::readDatas()
{
    QByteArray buffer;
    buffer=mySocket->readAll();
    qDebug()<<buffer<<endl;
    if(!buffer.isEmpty())
    {
        QString data= tr(buffer);

    data=this->userID+":"+data;
    father->readData(data);
  }



}

battleBox *user::getBox() const
{
    return box;
}

void user::setBox(battleBox *value)
{
    box = value;
}



/*****************************************************/



/*********************************************/

battleThread::battleThread(QObject*parent) :QThread(parent)
{

}

user *battleThread::getPlayer1() const
{
    return player1;
}

void battleThread::setPlayer1(user *value)
{
    player1 = value;
}

user *battleThread::getPlayer2() const
{
    return player2;
}

void battleThread::setPlayer2(user *value)
{
    player2 = value;
}

void battleThread::run()
{

    while(1)
    {

        bool flag_end=false;
        while (player1->getBattleState()==false||player2->getBattleState()==false)
        {
            QTime time2=QTime::currentTime().addMSecs(300);
            while(QTime::currentTime()<time2)
            {
            }
        qDebug()<<"正在等待";
        }

        if(player1->getStrategy()=="ATK")
        {
            if(player2->getStrategy()=="DEF")
                player1->setHPNow(player1->getHPNow()-1);
            else if(player2->getStrategy()=="ATK")
                ;
            else if(player2->getStrategy()=="REC")
                player2->setHPNow(player2->getHPNow()-1);
        }
        else if(player1->getStrategy()=="DEF")
        {
            if(player2->getStrategy()=="DEF")
                    ;
            else if(player2->getStrategy()=="ATK")
                player2->setHPNow(player2->getHPNow()-1);
            else if(player2->getStrategy()=="REC")
            {
                player2->setHPNow(player2->getHPNow()+1);
                if(player2->getHPNow()>=player2->getHPmax())
                     player2->setHPNow(player2->getHPmax());
            }
        }
        else if(player1->getStrategy()=="REC")
        {
            if(player2->getStrategy()=="DEF")
            {
               player1->setHPNow(player1->getHPNow()+1);
               if(player1->getHPNow()>=player1->getHPmax())
               player1->setHPNow(player1->getHPmax());
            }
            else if(player2->getStrategy()=="ATK")
                player1->setHPNow(player1->getHPNow()-1);
            else if(player2->getStrategy()=="REC")
            {
                player1->setHPNow(player1->getHPNow()+1);
                if(player1->getHPNow()>=player1->getHPmax())
                player1->setHPNow(player1->getHPmax());
                player2->setHPNow(player2->getHPNow()+1);
                if(player2->getHPNow()>=player2->getHPmax())
                player2->setHPNow(player2->getHPmax());

            }
        }

        //判断血量是否为0 --
        if(player1->getHPNow()<=0)
        {
            qDebug()<<"结束结束结束结束·····";
            flag_end=true;
            player1->setBattleState(false);
            player2->setBattleState(false);
            emit battleEnd(player2);
        }
        else if(player2->getHPNow()<=0)
        {
            qDebug()<<"结束结束结束结束·····";
            flag_end=true;
            player1->setBattleState(false);
            player2->setBattleState(false);
            emit battleEnd(player1);
        }


        if(flag_end) {
            player1->setHPNow(player1->getHPmax());
            player2->setHPNow(player2->getHPmax());
            break;}
        else
        {
            player1->setBattleState(false);
            player2->setBattleState(false);
            qDebug()<<"下一回合了！";

            emit nextTurn(this);
        }
    }



}










void battleBox::settle()
{
    bool flag_end=false;


    if(player1->getStrategy()=="ATK")
    {
        if(player2->getStrategy()=="DEF")
            player1->setHPNow(player1->getHPNow()-1);
        else if(player2->getStrategy()=="ATK")
            ;
        else if(player2->getStrategy()=="REC")
            player2->setHPNow(player2->getHPNow()-1);
    }
    else if(player1->getStrategy()=="DEF")
    {
        if(player2->getStrategy()=="DEF")
                ;
        else if(player2->getStrategy()=="ATK")
            player2->setHPNow(player2->getHPNow()-1);
        else if(player2->getStrategy()=="REC")
        {
            player2->setHPNow(player2->getHPNow()+1);
            if(player2->getHPNow()>=player2->getHPmax())
                 player2->setHPNow(player2->getHPmax());
        }
    }
    else if(player1->getStrategy()=="REC")
    {
        if(player2->getStrategy()=="DEF")
        {
           player1->setHPNow(player1->getHPNow()+1);
           if(player1->getHPNow()>=player1->getHPmax())
           player1->setHPNow(player1->getHPmax());
        }
        else if(player2->getStrategy()=="ATK")
            player1->setHPNow(player1->getHPNow()-1);
        else if(player2->getStrategy()=="REC")
        {
            player1->setHPNow(player1->getHPNow()+1);
            if(player1->getHPNow()>=player1->getHPmax())
            player1->setHPNow(player1->getHPmax());
            player2->setHPNow(player2->getHPNow()+1);
            if(player2->getHPNow()>=player2->getHPmax())
            player2->setHPNow(player2->getHPmax());

        }
    }

    //判断血量是否为0 --
    if(player1->getHPNow()<=0)
    {
        flag_end=true;
        emit battleEnd(player2);
    }
    else if(player2->getHPNow()<=0)
    {
        flag_end=true;
        emit battleEnd(player1);
    }


    if(flag_end) {
                    ;
        }
    else
    {
        player1->setBattleState(false);
        player2->setBattleState(false);
        emit nextTurn(this);
    }










}

bool battleBox::wait()
{
    if(player1->getBattleState()==false||player2->getBattleState()==false)
        return true;
    else return false;

}

user *battleBox::getPlayer1() const
{
    return player1;
}

void battleBox::setPlayer1(user *value)
{
    player1 = value;
}

user *battleBox::getPlayer2() const
{
    return player2;
}

void battleBox::setPlayer2(user *value)
{
    player2 = value;
}
