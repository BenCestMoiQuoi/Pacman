#include <iostream>
#include "pacmanwindow.hpp"

using namespace std;

Qt::Key players_keys[4][4] =
{
    // Controles joueur 1
    {Qt::Key_Left,Qt::Key_Right,Qt::Key_Up,Qt::Key_Down},
    // Controles joueur 2
    {Qt::Key_Q,Qt::Key_D,Qt::Key_Z,Qt::Key_S},
    // Controles joueur 3
    {Qt::Key_J,Qt::Key_L,Qt::Key_I,Qt::Key_K},
    // Controles joueur 4
    {Qt::Key_4,Qt::Key_6,Qt::Key_8,Qt::Key_5}
};

PacmanWindow::PacmanWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{
    string path;
    QPixmap tile;

    // Taille des cases en pixels
    int largeurCase, hauteurCase;
    this->x_offset = 0;
    this->y_offset = 0;

    // Chargement des images
    for (int i=0;i<4;i++)
    {
        path = "./data/pacman_"+to_string(i)+".png";
        if(tile.load(path.c_str()) == false)
        {
            cout<<"Impossible d'ouvrir "+path<<endl;
            exit(-1);
        }
        pixmapPacman[i] = tile;
    }

    for (int i=0;i<16;i++)
    {
        path = "./data/fantome_"+to_string(i)+".png";
        if(tile.load(path.c_str()) == false)
        {
            cout<<"Impossible d'ouvrir "+path<<endl;
            exit(-1);
        }
        pixmapFantome[i] = tile;
    }

    if (pixmapFantome_Bleu.load("./data/fantome_bleu.png") == false)
    {
        cout<<"Impossible d'ouvrir fantome_bleu.png"<<endl;
        exit(-1);
    }

    for (int i=0;i<47;i++)
    {
        path = "./data/niveau/"+to_string(i)+".png";
        if(tile.load(path.c_str()) == false)
        {
            cout<<"Impossible d'ouvrir "+path<<endl;
            exit(-1);
        }
        level[i] = tile;
    }

    if (pixmapWIN.load("./data/win.png") == false)
    {
        cout<<"Impossible d'ouvrir win.png"<<endl;
        exit(-1);
    }

    if (pixmapLOOSE.load("./data/loose.png") == false)
    {
        cout<<"Impossible d'ouvrir loose.png"<<endl;
        exit(-1);
    }

    this->setStyleSheet("background-color:black;");

    jeu.init();

    largeurCase = level[0].width();
    hauteurCase = level[0].height();

    QLabel *label_fantome = new PacmanLabel("Fantomes",this);
    label_fantome->setGeometry(1 * largeurCase, 0, 4 * largeurCase, 2 * hauteurCase);
    label_fantome->setStyleSheet("font-weight: bold; color: white");

	QPushButton *btn_ajout_fantome = new PacmanButton(this);
	btn_ajout_fantome->setGeometry(1 * largeurCase, 2 * hauteurCase, 2 * largeurCase, 2 * hauteurCase);
    btn_ajout_fantome->setText("+");
    btn_ajout_fantome->setStyleSheet("QWidget {color: white; background-color: #505050}");
    connect(btn_ajout_fantome, &QPushButton::clicked, this, &PacmanWindow::clickBouton_ajout_fantome);

    QPushButton *btn_suppr_fantome = new PacmanButton(this);
    btn_suppr_fantome->setGeometry(3 * largeurCase, 2 * hauteurCase, 2 * largeurCase, 2 * hauteurCase);
    btn_suppr_fantome->setText("-");
    btn_suppr_fantome->setStyleSheet("QWidget {color: white; background-color: #505050}");
    connect(btn_suppr_fantome, &QPushButton::clicked, this, &PacmanWindow::clickBouton_suppr_fantome);


    QLabel *label_pacman = new PacmanLabel("Pacmans",this);
    label_pacman->setGeometry(6 * largeurCase + 2, 0, 4 * largeurCase, 2 * hauteurCase);
    label_pacman->setStyleSheet("font-weight: bold; color: white");

	QPushButton *btn_ajout_pacman = new PacmanButton(this);
	btn_ajout_pacman->setGeometry(6 * largeurCase, 2 * hauteurCase, 2 * largeurCase, 2 * hauteurCase);
    btn_ajout_pacman->setText("+");
    btn_ajout_pacman->setStyleSheet("QWidget {color: white; background-color: #505050}");
    connect(btn_ajout_pacman, &QPushButton::clicked, this, &PacmanWindow::clickBouton_ajout_pacman);

    QPushButton *btn_suppr_pacman = new PacmanButton(this);
    btn_suppr_pacman->setGeometry(8 * largeurCase, 2 * hauteurCase, 2 * largeurCase, 2 * hauteurCase);
    btn_suppr_pacman->setText("-");
    btn_suppr_pacman->setStyleSheet("QWidget {color: white; background-color: #505050}");
    connect(btn_suppr_pacman, &QPushButton::clicked, this, &PacmanWindow::clickBouton_suppr_pacman);


    QPushButton *btn_recom = new PacmanButton(this);
    btn_recom->setGeometry(14 * largeurCase, 2 * largeurCase, 6 * largeurCase, 2 * hauteurCase);
    btn_recom->setText("Recommencer");
    btn_recom->setStyleSheet("QWidget {color: white; background-color: #505050}");
    connect(btn_recom, &QPushButton::clicked, this, &PacmanWindow::clickBouton_recom);


    QLabel *label_score = new PacmanLabel("Score :",this);
    label_score->setGeometry(23 * largeurCase, 0, 4 * largeurCase, 2 * hauteurCase);
    label_score->setStyleSheet("font-weight: bold; color: white");

    label_score_value = new PacmanLabel("0",this);
    label_score_value->setGeometry(23 * largeurCase, 2 * hauteurCase, 4 * largeurCase, 1 * hauteurCase);
    label_score_value->setStyleSheet("font-weight: bold; color: white");


    level_input = new PacmanList(this);
    level_input->setGeometry(14 * largeurCase, 0 * largeurCase, 6 * largeurCase, 2 * hauteurCase);
    level_input->addItems({"base", "geant", "rapide"});
    level_input->setStyleSheet("QWidget {color: white; background-color: #505050}");

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PacmanWindow::handleTimer);
    timer->start(100);

    resize((jeu.getNbCasesX()+x_offset)*largeurCase, (jeu.getNbCasesY()+y_offset)*hauteurCase);
    set_x_offset(0);
    set_y_offset(4);
}

void PacmanWindow::set_x_offset(int x)
{
    int largeurCase, hauteurCase;
    largeurCase = level[0].width();
    hauteurCase = level[0].height();

    this->x_offset = x;

    resize((jeu.getNbCasesX()+this->x_offset)*largeurCase, (jeu.getNbCasesY()+this->y_offset)*hauteurCase);
}

void PacmanWindow::set_y_offset(int y)
{
    int largeurCase, hauteurCase;
    largeurCase = level[0].width();
    hauteurCase = level[0].height();

    this->y_offset = y;

    resize((jeu.getNbCasesX()+this->x_offset)*largeurCase, (jeu.getNbCasesY()+this->y_offset)*hauteurCase);
}

void PacmanWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int x, y;
    int largeurCase, hauteurCase;

    largeurCase = level[0].width();
    hauteurCase = level[0].height();

    // Affiche le score
    QString str_score(to_string(jeu.getScore()).c_str());
    label_score_value->setText(str_score);

    // Dessine les cases
    for (y=0;y<jeu.getNbCasesY();y++)
        for (x=0;x<jeu.getNbCasesX();x++)
            if (jeu.getCaseIndex(x,y)!=0 and jeu.getCaseIndex(x,y)!=44)
                painter.drawPixmap((x+x_offset)*largeurCase, (y+y_offset)*hauteurCase, level[jeu.getCaseIndex(x,y)]);

    // Dessine Pacman
    const list<Pacman_entity> &pacmans = jeu.getPacmans();
    list<Pacman_entity>::const_iterator itPacman;
    for (itPacman=pacmans.begin(); itPacman!=pacmans.end(); itPacman++)
        painter.drawPixmap((itPacman->getPosX()+x_offset)*largeurCase-(pixmapPacman[itPacman->getDir()].width()/4), (itPacman->getPosY()+y_offset)*hauteurCase-(pixmapPacman[itPacman->getDir()].height()/4), pixmapPacman[itPacman->getDir()]);

    // Dessine les fantomes
    const list<Fantome_entity> &fantomes = jeu.getFantomes();
    list<Fantome_entity>::const_iterator itFantome;
    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
        if (jeu.getTempsBleus() == 0)
            painter.drawPixmap((itFantome->getPosX()+x_offset)*largeurCase-(pixmapFantome[itFantome->getDir()].width()/4), (itFantome->getPosY()+y_offset)*hauteurCase-(pixmapFantome[itFantome->getDir()].height()/4), pixmapFantome[itFantome->getDir()+itFantome->getColor()*4]);
        else
            painter.drawPixmap((itFantome->getPosX()+x_offset)*largeurCase-(pixmapFantome_Bleu.width()/4), (itFantome->getPosY()+y_offset)*hauteurCase-(pixmapFantome_Bleu.height()/4), pixmapFantome_Bleu);

    // Dessine les textes de victoire et defaite
    if (jeu.getFin()) {
        if (jeu.getPacmans().empty())
            painter.drawPixmap((jeu.getNbCasesX()/2 + x_offset)*largeurCase - pixmapLOOSE.width()/2, (jeu.getNbCasesY()/2 + y_offset)*hauteurCase - pixmapLOOSE.height()/2 - hauteurCase/2, pixmapLOOSE);
        else
            painter.drawPixmap((jeu.getNbCasesX()/2 + x_offset)*largeurCase - pixmapWIN.width()/2, (jeu.getNbCasesY()/2 + y_offset)*hauteurCase - pixmapWIN.height()/2 - hauteurCase/2, pixmapWIN);

    }
}

void PacmanWindow::keyPressEvent(QKeyEvent *event)
{
    list<Pacman_entity> &pacmans = jeu.getPacmans();
    list<Pacman_entity>::iterator itPacman;
    int p;

    if (!jeu.getFin()) {
        for (itPacman=pacmans.begin(); itPacman!=pacmans.end(); itPacman++)
        {
            p = itPacman->getPlayer();

            if (event->key()==players_keys[p][0])
            {
                itPacman->setDir(GAUCHE);
                jeu.deplace_entite(*itPacman);
            }
            else if (event->key()==players_keys[p][1])
            {
                itPacman->setDir(DROITE);
                jeu.deplace_entite(*itPacman);
            }
            else if (event->key()==players_keys[p][2])
            {
                itPacman->setDir(HAUT);
                jeu.deplace_entite(*itPacman);
            }
            else if (event->key()==players_keys[p][3])
            {
                itPacman->setDir(BAS);
                jeu.deplace_entite(*itPacman);
            }
        }
        update();
    }
}

void PacmanWindow::handleTimer()
{
    jeu.evolue();
    update();
}

void PacmanWindow::clickBouton_ajout_fantome()
{
    if (!jeu.getDebut()) {
        jeu.AddFantomes();
        jeu.ajout_fantome();
    }
}

void PacmanWindow::clickBouton_suppr_fantome()
{
    if (!jeu.getDebut()) {
        jeu.RemFantomes();
        jeu.suppr_fantome();
    }
}

void PacmanWindow::clickBouton_ajout_pacman()
{
    if (!jeu.getDebut() && !(jeu.getPacmans().size()>=4)) {
        jeu.AddPacmans();
        jeu.ajout_pacman();
    }
}

void PacmanWindow::clickBouton_suppr_pacman()
{
    if (!jeu.getDebut() && !(jeu.getPacmans().size()<=1)) {
        jeu.RemPacmans();
        jeu.suppr_pacman();
    }
}


void PacmanWindow::clickBouton_recom()
{
    int largeurCase = level[0].width();
    int hauteurCase = level[0].height();

    string niveau = level_input->currentText().toStdString();

    jeu.set_niveau_terrain(niveau);

    jeu.init();
    resize((jeu.getNbCasesX()+this->x_offset)*largeurCase, (jeu.getNbCasesY()+this->y_offset)*hauteurCase);
}

void PacmanButton::keyPressEvent(QKeyEvent *e)
{
	if (parent()!=nullptr)
		QCoreApplication::sendEvent(parent(), e);
}

void PacmanLabel::keyPressEvent(QKeyEvent *e)
{
    if (parent()!=nullptr)
        QCoreApplication::sendEvent(parent(), e);
}

void PacmanList::keyPressEvent(QKeyEvent *e)
{
    if (parent()!=nullptr)
        QCoreApplication::sendEvent(parent(), e);
}

void PacmanWindow::clickList(QKeyEvent *e)
{
    cout<<"oui"<<endl;
}
