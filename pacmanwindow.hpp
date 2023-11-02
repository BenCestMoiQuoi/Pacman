#ifndef PACMANWINDOW_HPP
#define PACMANWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>

#include "jeu.hpp"

class PacmanWindow : public QFrame
{
  protected:
    Jeu jeu;
    QPixmap pixmapPacman[4];
    QPixmap pixmapFantome[16];
    QPixmap pixmapFantome_Bleu;

    QPixmap level[47];

    QPixmap pixmapWIN, pixmapLOOSE;

    int x_offset, y_offset;

    QLabel *label_score_value;
    QComboBox *level_input;

  public:
    PacmanWindow(QWidget *pParent=0, Qt::WindowFlags flags=Qt::WindowFlags());
    void handleTimer();

    void set_x_offset(int);
    void set_y_offset(int);

    void clickBouton_ajout_fantome();
    void clickBouton_suppr_fantome();
    void clickBouton_ajout_pacman();
    void clickBouton_suppr_pacman();

    void clickBouton_recom();

  protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void clickList(QKeyEvent *);
};

class PacmanButton : public QPushButton
{
  public:
	  PacmanButton(QWidget *parent):QPushButton(parent){};
	  ~PacmanButton(){};
	  void keyPressEvent(QKeyEvent *);
};

class PacmanLabel : public QLabel
{
  public:
    PacmanLabel(const QString &text, QWidget *parent):QLabel(text, parent){};
    ~PacmanLabel(){};
    void keyPressEvent(QKeyEvent *);
};

class PacmanList : public QComboBox
{
  public:
    PacmanList(QWidget *parent):QComboBox(parent){};
    ~PacmanList(){};
    void keyPressEvent(QKeyEvent *);
};

#endif
