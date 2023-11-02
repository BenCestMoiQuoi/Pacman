#ifndef JEU_HPP
#define JEU_HPP

#include <list>
#include <vector>

typedef enum {VIDE = 0, PAC_GUM = 1, SUPER_GUM = 2, MUR = 3} Case;
typedef enum {GAUCHE = 0, DROITE = 1, HAUT = 2, BAS = 3} Direction;
typedef enum {PACMAN, FANTOME} Entity_type;
typedef enum {ROUGE = 0, ROSE = 1, BLEU = 3, ORANGE = 4} Fantome_color;
typedef enum {sDEBUT = 0, sFIN = 1, sPAC_GUM = 2, sSUPER_GUM = 3, sMANGER = 4, sMORT = 5} Sons;

class Jeu;

class Entity
{
    friend class Jeu;

  private:
    int posX, posY;
    Direction dir;
    bool godmode;
    Entity_type type;
    int player; // Numero du joueur

  public:
    // constructeurs
    Entity();
    Entity(Entity_type,int,int,Direction,bool,int);
    Entity(const Entity &);

    Entity &operator=(const Entity &);

    // accesseurs
    Entity_type getType() const;
    int getPosX() const;
    int getPosY() const;
    Direction getDir() const;
    bool getGodmode() const;
    int getPlayer() const;

    // mutateurs
    void setType(Entity_type);
    void setPosX(int);
    void setPosY(int);
    void setDir(Direction);
    void setGodmode(bool);
    void setPlayer(int);
};

class Fantome_entity : public Entity
{
  private :
    Fantome_color couleur;

  public :
    // constructeurs
    Fantome_entity();
    Fantome_entity(int,int,Direction);

    // accesseur
    Fantome_color getColor() const;

    // mutateur
    void setColor(Fantome_color);
};

class Pacman_entity : public Entity
{
  public :
    // constructeurs
    Pacman_entity();
    Pacman_entity(int,int,Direction);
    Pacman_entity(int,int,Direction,int);
};

class Jeu
{
  protected:
    int *terrain;
    std::string niveau_terrain;
    int largeur, hauteur; // Nombre de cases en largeur et en hauteur
    int nb_pacmans, nb_fantomes; // Nombre de fantomes et de pacmans en debut de partie
    std::list<Fantome_entity> fantomes;
    std::list<Pacman_entity> pacmans;

    std::list<Sons> effets_sonors;

    int nb_pac_gum, nb_super_gum;
    bool fin_jeu, debut_jeu;
    int temps_bleus, pacman_super;

    int score;

  public:
    Jeu();
    Jeu(const Jeu &);
    ~Jeu();

    Jeu &operator=(const Jeu &);

    bool init();
    void evolue();

    // Retourne les dimensions (en nombre de cases)
    int getNbCasesX() const;
    int getNbCasesY() const;

    // Charge le niveau a partir d'un fichier
    std::vector< std::vector <int> > charger_niveau(const char[]) const;
    // Definie le numero du niveau a jouer
    void set_niveau_terrain(std::string);

    int getTempsBleus(void) const;
    int getScore(void) const;
    bool getDebut(void) const;
    bool getFin(void) const;


    // Retourne la case a une position donnee
    int getCaseIndex(int, int) const;
    Case getCase(int, int) const;

    // Definit l'index de la case
    bool setCaseIndex(int, int, int);
    // Definit le type de la case
    // (!!! tous les types de murs ne sont pas definis)
    bool setCase(int, int, Case);

    void AddPacmans();
    void RemPacmans();
    void AddFantomes();
    void RemFantomes();

    // Informe si la liste effets_sons est vide
    bool ListSonsVide();
    // Retourne le premier Sons de la liste effets_sons
    Sons PremierEffet_sons();
    // Supprime le premier son de la liste effets_sons
    void RemoveSons();

    // Retourne la liste de fantomes en lecture seule
    const std::list<Fantome_entity> &getFantomes() const;
    const std::list<Pacman_entity> &getPacmans() const;

    // Retourne la liste de fantomes
    std::list<Fantome_entity> &getFantomes();
    std::list<Pacman_entity> &getPacmans();

    // Indique si la case a une position donnee existe et est vide
    // (Pacman ou les fantomes peuvent l'occuper)
    bool posValide(int, int) const;

    // Deplace l'entite donnee selon sa direction
    bool deplace_entite(Entity &);

    // Place l'entite a des coordonnees aleatoires
    void placer_entite(Entity &);
    // Place l'entite a des coordonnees donnees
    bool placer_a_entite(int,int,Entity &);

    // Ajoute un fantome
    void ajout_fantome();
    // Supprime le dernier fantome de la liste
    void suppr_fantome();

    // Ajoute un pacman avec un numero de joueur choisi automatiquement
    void ajout_pacman();
    // Supprime le dernier pacman de la liste
    void suppr_pacman();
};

#endif
