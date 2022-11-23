#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QWidget>
#include <QString>
#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QPushButton>
#include <QDebug>
#include <QLabel>
#include <QRect>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QBrush>
#include <QList>
#include <QTime>
#include <QMap>
#include <QAbstractSocket>
#include <QNetworkProxy>
#include <QMessageBox>
#include <QMapIterator>
#include <QThread>
#include <QDebug>

#include "instruction.h"

#pragma execution_character_set("utf-8")
QT_BEGIN_NAMESPACE


namespace Ui { class Widget; }
QT_END_NAMESPACE

class user;

class battleThread;
class battleBox;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


    void sendUserIdx(QTcpSocket*,QString);
    void readData(QString data);
    void sendTalkToAll();
    void sendTalkToAllP(QString);
    void senTalk( user* Src, user* Des,QString str);
    void matching(user* u);
    void stopMatching(user* u);
    void matchSuccess(user*u);

    int findFreeThread();

private:



    Ui::Widget *ui;
    instruction ins;

    QTcpServer *server;
    QTcpSocket  *socket;
    //QList <user *> userList;
    QMap <QString,user*> userList;  //在线？-用户列表

    battleThread *bat;
    QList< battleThread *> batManage;

    //选中
    int chooseIdx;

    //游客id
    int nowIdx;

    //匹配相关
    //正在匹配人数
    int matchCount;
    QList <user *> matchList;
    bool matchListSock;

    //战斗相关
    QList<QList < QString > > battleHome;
    bool battleHomeSock;//锁1

    //资源相关--是否连接数据包

    //服务器更新可以从客户端读取资源包名称，前提是本地添加到目录里，然后向客户端传输资源包，再更新静态资源包名字
    //静态资源包--名字
    QList<QString > characterNameTest;   //先用在程序里设置的测试

    void paintEvent(QPaintEvent *event);



private slots:

    void serverNewConnect();
    void soDisconnected();
    void battleTurnEnd(battleThread*);
    void sendWinner(user*);

};




class user : public QObject
{
    Q_OBJECT
public:
    explicit user(QObject *parent = nullptr);

    void sendInformation(QString st);
    void linkWithReadData();
    QTcpSocket *getMySocket() const;
    void setMySocket(QTcpSocket *value);
    void changeState(int i);

    user *getOppo() const;
    void setOppo(user *value);

    QString getUserID() const;
    void setUserID(const QString &value);

    void setFather(Widget *fa);
    QString getStrategy() const;
    void setStrategy(const QString &value);

    int getHPNow() const;
    void setHPNow(int value);

    int getHPmax() const;
    void setHPmax(int value);

    user *getObj() const;
    void setObj(user *value);

    QString getCharacterName() const;
    void setCharacterName(const QString &value);

    bool getBattleState() const;
    void setBattleState(bool value);
    battleBox *getBox() const;
    void setBox(battleBox *value);

public slots:
    void readDatas();

private:

    battleBox * box;
    Widget *father;
    QTcpSocket *mySocket;
    user * oppo;
    //用户ID 作为唯一标识符
    QString userID;
    //用户状态  [0]离线 [1]在线 [2]战斗
    int userState;


    //战斗相关属性
    QString characterName;
    QString strategy;
    int HPNow;
    int HPmax;
        //用户状态 [0] 正在思考 [1]已完成
    bool battleState;




    //聊天相关
    user * obj;



signals:


private slots:



};















class battleThread : public QThread
{
    Q_OBJECT
public:
    explicit  battleThread(QObject *parent=nullptr);


    user *getPlayer1() const;
    void setPlayer1(user *value);

    user *getPlayer2() const;
    void setPlayer2(user *value);

private:
    user *player1;
    user *player2;

signals:
    void  battleEnd(user * winner);
    void  nextTurn(battleThread*);
protected:
    void run() override;
};






class battleBox :public QObject
{
   Q_OBJECT

public:
    battleBox(QObject *parent = nullptr): QObject(parent)
    {

    }

    void settle();

    bool wait();

    user *getPlayer1() const;
    void setPlayer1(user *value);

    user *getPlayer2() const;
    void setPlayer2(user *value);

private:
    user *player1;
    user *player2;

signals:
    void  battleEnd(user * winner);
    void  nextTurn(battleBox*);

};


















#endif // WIDGET_H
