#include "gamemenu.h"
#include<QIcon>
#include<QPainter>
#include<QPushButton>
#include"game.h"
#include<QSound>
#include"gameroom.h"
#include<QSound>
#include<QFile>
#include<QTextEdit>
#include<QTextStream>
gamemenu::gamemenu(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1000,800);
    this->setWindowTitle("游戏菜单");
    QIcon icon(":/res/ico.png");
    this->setWindowIcon(icon);

    QPushButton* button1=new QPushButton(this);
    QPushButton* button2=new QPushButton(this);
    QPushButton* button3=new QPushButton(this);
    QPushButton* button4=new QPushButton(this);
    QPushButton* button5=new QPushButton(this);

    button1->move(420,200);
    button2->move(420,300);
    button3->move(420,400);
    button4->move(420,500);
    button5->move(900,750);

    button1->setText("简单模式");
    button2->setText("正常模式");
    button3->setText("困难模式");
    button4->setText("历史战绩");
    button5->setIcon(QIcon(":/res/back.png"));

    QFont font("宋体",20);
    button1->setFont(font);
    button2->setFont(font);
    button3->setFont(font);
    button4->setFont(font);


    gameroom* gm=new gameroom();
    connect(button1,&QPushButton::clicked,[=](){
        this->close();
        gm->setGeometry(this->geometry());
        gm->show();
        QSound::play(":/res/clicked.wav");
        gm->setmovetime(400);
    });
    connect(button2,&QPushButton::clicked,[=](){
        this->close();
        gm->setGeometry(this->geometry());
        gm->show();
        QSound::play(":/res/clicked.wav");
        gm->setmovetime(200);
    });
    connect(button3,&QPushButton::clicked,[=](){
        this->close();
        gm->setGeometry(this->geometry());
        gm->show();
        QSound::play(":/res/clicked.wav");
        gm->setmovetime(50);
    });
    connect(button4,&QPushButton::clicked,[=](){

      QWidget* widget=new QWidget;
      widget->setWindowTitle("历史战绩");
      widget->setFixedSize(500,300);
      widget->setWindowIcon(QIcon(":/res/scores.png"));

     //选择文件
     QFile file("C:/Users/13911/Desktop/QT_greedy_snake/snake/1.txt");
     if(file.open(QIODevice::ReadOnly))
     {         //对文件进行只读操作
         QTextStream in(&file);//读取文件
        int data=in.readLine().toInt();//把文件内的数读取出来


      QTextEdit* edit=new QTextEdit(widget);
         edit->setFixedSize(500,300);
         edit->append("上局得分:");
         edit->append(QString::number(data));
         edit->setFont(font);

         widget->show();
         QSound::play(":/res/clicked.wav");
         file.close();
     }

    });
    connect(button5,&QPushButton::clicked,[=](){
        this->close();
        Game* game=new Game;
        game->show();
        QSound::play(":/res/clicked.wav");
    });


}

void gamemenu::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPixmap pixmap(":/res/game_select");

    painter.drawPixmap(0,0,this->width(),this->height(),pixmap);

}
