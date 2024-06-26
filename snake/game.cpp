#include "game.h"
#include "ui_game.h"
#include<QPainter>
#include<QIcon>
#include<QPushButton>
#include<QFont>
#include"gamemenu.h"
#include<QSound>
Game::Game(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Game)
{
    ui->setupUi(this);

    QIcon icon(":/res/ico.png");
    this->setWindowTitle("贪吃蛇");
    this->setWindowIcon(icon);
    this->setFixedSize(1000,800);


    QPushButton* button=new QPushButton(this);
    button->setText("开始游戏");
    QFont font("宋体",20);
    button->setFont(font);
    button->move(400,560);
    button->setStyleSheet("QPushButton{border:0px;}");

    gamemenu* gmenu=new gamemenu;

    connect(button,&QPushButton::clicked,this,[=](){
        this->close();
        gmenu->setGeometry(this->geometry());
        gmenu->show();
        QSound::play(":/res/clicked.wav");
    });

}

Game::~Game()
{
    delete ui;
}

void Game::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPixmap pixmap(":/res/game_hall.png");

    painter.drawPixmap(0,0,this->width(),this->height(),pixmap);
}

