#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
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
#include <QAbstractSocket>
#include <QNetworkProxy>
#include <QMessageBox>
#include <QTime>
#include <QDebug>
#include <QIcon>

#include "instruction.h"
#include "waitingwidget.h"
#include "mystyle.h"

#pragma execution_character_set("utf-8")
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT




public:
    // 这是客户端本体

    Widget(QWidget *parent = nullptr);
    ~Widget();



    void gameEnd(QString winner);       //本地接收到获胜者的名字
    void showHP();                      //展示血量
    void nextTurn();                    //下一回合  ---对本地而言 是计时器重新打开、按钮恢复
    void initBattle();                  //初始化战斗模块
    void timerStop();                   //计时停止时触发的行为

    void resets();                      //重置 --战斗结束后进行重置
private:

    Ui::Widget *ui;

    myStyle st;
    instruction ins;
    const int port=23333;                 //固定对应服务器端口

    int turnNumber=0;                      //没实装的回合数计数器
    //链接内容
    QTcpSocket *socket;

    //战斗结果弹窗
    QMessageBox tipBox;                     //战斗结果速递
    QMessageBox waitBox;                    //匹配等待盒子   --已更换成下面那个玩意
    waitingWidget *waitingBox;              //


    //ui
    QList<QPushButton *> btnL;              //为设置style而创建的list




    //实体内容
    QRect mineRect;
    QRect oppoRect;


    QRect mineHPRect;
    QRect oppoHPRect;
    QRect mineHPMaxRect;
    QRect oppoHPMaxRect;

    QRect headRect;
    QRect headBondRect;

    QLabel* timerLabel;

    const int HPmax=5;
    int mineHP;
    int oppoHP;

    QString mineName;
    QString oppoName;


    QString mineImageName;
    QString oppoImageName;
    QString backgroundName;
    QPixmap mineImage;
    QPixmap oppoImage;
    QPixmap backgroundImage;
    QPixmap headPic;
    QPixmap headBondPic;
    int turnTime;
    QTimer *tim;

    QString lastStrategy;

    QString userID;
protected:
    void paintEvent(QPaintEvent *event);



private slots:

    void onTimeOut();
    void linkToServer();
    void sendTalk();
    void readDataFromSever();
    void matchToPlayer();
    void matchSuccessed();
    void matchFailed();
    void ATK_On();
    void DEF_On();
    void REC_On();
    void on_informationShow_textChanged();                      //用于每次消息刷新时把消息框拉到最底
};
#endif // WIDGET_H
