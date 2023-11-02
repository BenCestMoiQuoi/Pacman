#include <iostream>
#include <cassert>
#include <fstream>
#include "jeu.hpp"

#define PAC_GUM_SCORE 10
#define SUPER_GUM_SCORE 20
#define MANGER_FANTOME_SCORE 50

using namespace std;


Jeu::Jeu()
{
    terrain = nullptr;
    niveau_terrain = "base";

    largeur = 0; hauteur = 0;

    nb_pacmans = 2, nb_fantomes = 1;
    nb_pac_gum = 0, nb_super_gum = 0;

    fin_jeu = false, debut_jeu = false;

    temps_bleus = 0, pacman_super = -1;

    score = 0;
}

Jeu::Jeu(const Jeu &jeu):fantomes(jeu.fantomes)
{
    largeur = jeu.largeur;
    hauteur = jeu.hauteur;

    nb_pacmans = jeu.nb_pacmans;
    nb_fantomes = jeu.nb_fantomes;

    nb_pac_gum = jeu.nb_pac_gum;
    nb_super_gum = jeu.nb_super_gum;

    fin_jeu = jeu.fin_jeu;
    debut_jeu = jeu.debut_jeu;

    temps_bleus = jeu.temps_bleus;
    pacman_super = jeu.pacman_super;

    score = jeu.score;

    niveau_terrain = jeu.niveau_terrain;
    if (jeu.terrain!=nullptr)
    {
        terrain = new int[largeur*hauteur];
        for (int c=0; c<largeur*hauteur; c++)
            terrain[c] = jeu.terrain[c];
    }
    else
        terrain = nullptr;
}

Jeu::~Jeu()
{
    if (terrain!=nullptr)
        delete[] terrain;
}

Jeu &Jeu::operator=(const Jeu &jeu)
{
    if (terrain!=nullptr)
        delete[] terrain;
    niveau_terrain = jeu.niveau_terrain;

    largeur = jeu.largeur;
    hauteur = jeu.hauteur;

    pacmans = jeu.pacmans;
    fantomes = jeu.fantomes;

    nb_pacmans = jeu.nb_pacmans;
    nb_fantomes = jeu.nb_fantomes;

    nb_pac_gum = jeu.nb_pac_gum;
    nb_super_gum = jeu.nb_super_gum;

    fin_jeu = jeu.fin_jeu;
    debut_jeu = jeu.debut_jeu;

    temps_bleus = jeu.temps_bleus;
    pacman_super = jeu.pacman_super;

    score = jeu.score;

    if (jeu.terrain!=nullptr)
    {
        terrain = new int[largeur*hauteur];
        for (int c=0; c<largeur*hauteur; c++)
            terrain[c] = jeu.terrain[c];
    }
    else
        terrain = nullptr;
    return *this;
}

bool Jeu::init()
{
	int x, y;
	list<Fantome_entity>::iterator itFantome;
    list<Pacman_entity>::iterator itPacman;

    // Initialisation des variables de deroulement du jeu
    fin_jeu = false;
    debut_jeu = false;
    nb_pac_gum = 0;
    nb_super_gum = 0;
    temps_bleus = 0;
    pacman_super = -1;
    score = 0;

    // Acquisition du niveau
    string path = "./data/niveau/"+niveau_terrain+".txt";
	vector< vector <int> > terrain_defaut = charger_niveau(path.c_str());

	largeur = terrain_defaut[0].size();
	hauteur = terrain_defaut.size();

	// Stockage du niveau dans l'objet jeu
	terrain = new int[largeur*hauteur];
	for(y=0;y<hauteur;++y)
		for(x=0;x<largeur;++x)
        {
            terrain[y*largeur+x] = terrain_defaut[y][x];
            if (terrain[y*largeur+x] == PAC_GUM)
                nb_pac_gum++;
            else if (terrain[y*largeur+x] == SUPER_GUM)
                nb_super_gum++;
        }

    fantomes.resize(0);
    pacmans.resize(0);

    // Creation du nombre de fantomes donnes
    for (int i = 0;i<nb_fantomes;i++) ajout_fantome();

    // Creation du nombre de pacmans donnes
    for (int i = 0;i<nb_pacmans;i++) ajout_pacman();

    // Placement des fantomes
	for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        placer_entite(*itFantome);
        itFantome->dir = (Direction)(rand()%4);
    }

    // Placement des pacmans
    for (itPacman=pacmans.begin(); itPacman!=pacmans.end(); itPacman++)
    {
        placer_entite(*itPacman);
        itPacman->dir = (Direction)(rand()%4);
    }

    effets_sonors.resize(sDEBUT);
    return true;
}

void Jeu::evolue()
{
	list<Fantome_entity>::iterator itFantome; 
    list<Pacman_entity>::iterator itPacman;

    if (fin_jeu == false and debut_jeu == true) {

        // Comportement des fantomes
        for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
        {
            // Deplacement des fantomes
            while(!deplace_entite(*itFantome))
            {
                itFantome->dir = (Direction)(rand()%4);
            }

            // Comportement au contact des pacmans
            for (itPacman=pacmans.begin(); itPacman!=pacmans.end(); itPacman++)
            {
                if(itFantome->posX == itPacman->posX && itFantome->posY == itPacman->posY)
                {
                    // En cas d'effet de super gum
                    if (temps_bleus > 0 && itFantome->godmode == false) {
                            fantomes.erase(itFantome--);
                            score += MANGER_FANTOME_SCORE;
                            effets_sonors.push_back(sMANGER);
                    }

                    // En cas de contact meutrier
                    else if (itPacman->godmode == false) {
                        pacmans.erase(itPacman--);
                        effets_sonors.push_back(sMORT);
                        if (pacmans.empty())
                        {
                                fin_jeu = true; // fin du jeu si le dernier pacman est mange
                                effets_sonors.push_back(sFIN);
                                break;
                        }    
                    }
                }
            }
        }

        // Gestion du temps de vunerabilite des fantomes
        if (temps_bleus > 0)
        {
            temps_bleus--;
            if (temps_bleus == 0) {
                pacman_super = -1;
                for (itPacman=pacmans.begin(); itPacman!=pacmans.end(); itPacman++) itPacman->setGodmode(false);
                for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++) itFantome->setGodmode(true);
            }
        }
    }
}

int Jeu::getNbCasesX() const
{
    return largeur;
}

int Jeu::getNbCasesY() const
{
    return hauteur;
}

int Jeu::getCaseIndex(int x, int y) const
{
    assert(x>=0 && x<largeur && y>=0 && y<hauteur);
    return terrain[y*largeur+x];
}

Case Jeu::getCase(int x, int y) const
{
    assert(x>=0 && x<largeur && y>=0 && y<hauteur);
    if (terrain[y*largeur+x] == VIDE) return VIDE;
    else if (terrain[y*largeur+x] == PAC_GUM) return PAC_GUM;
    else if (terrain[y*largeur+x] == SUPER_GUM) return SUPER_GUM;
    else return MUR;
}

bool Jeu::setCaseIndex(int x, int y, int ind) {
    if (x>=0 && x<largeur && y>=0 && y<hauteur){
        terrain[y*largeur+x] = ind;
        return true;
    }
    else return false;
}

bool Jeu::setCase(int x, int y, Case C) {
    if (x>=0 && x<largeur && y>=0 && y<hauteur){
        terrain[y*largeur+x] = C;
        return true;
    }
    else return false;
}

void Jeu::AddPacmans()
{
    this->nb_pacmans++;
}

void Jeu::RemPacmans()
{
    if (!pacmans.empty()) this->nb_pacmans--;
}


void Jeu::AddFantomes()
{
    this->nb_fantomes++;
}

void Jeu::RemFantomes()
{
    if (!fantomes.empty()) this->nb_fantomes--;
}

bool Jeu::ListSonsVide()
{
    return effets_sonors.empty();
}

Sons Jeu::PremierEffet_sons()
{
    return effets_sonors.front();
}

void Jeu::RemoveSons()
{
    std::list<Sons>::iterator it;

    if (!effets_sonors.empty()) 
    {
        it = this->effets_sonors.begin();
        this->effets_sonors.erase(it);
    }
}

const std::list<Fantome_entity> &Jeu::getFantomes() const
{
    return fantomes;
}

const std::list<Pacman_entity> &Jeu::getPacmans() const
{
    return pacmans;
}

std::list<Fantome_entity> &Jeu::getFantomes()
{
    return fantomes;
}

std::list<Pacman_entity> &Jeu::getPacmans()
{
    return pacmans;
}

int Jeu::getTempsBleus(void) const
{
    return temps_bleus;
}

int Jeu::getScore(void) const
{
    return score;
}

bool Jeu::getDebut(void) const
{
    return debut_jeu;
}

bool Jeu::getFin(void) const
{
    return fin_jeu;
}

bool Jeu::posValide(int x, int y) const
{
    return (x>=0 && x<largeur && y>=0 && y<hauteur && (terrain[y*largeur+x]<MUR));
}

void Jeu::ajout_fantome(void)
{
    Fantome_entity fantome;
    placer_entite(fantome);
    fantome.dir = (Direction)(rand()%4);
    fantome.setColor((Fantome_color)(rand()%4));

    fantomes.push_back(fantome);
}

void Jeu::suppr_fantome(void)
{
    if (!fantomes.empty()) {
        fantomes.pop_back();
    }
}

void Jeu::ajout_pacman(void)
{
    Pacman_entity pacman;
    placer_entite(pacman);
    pacman.dir = (Direction)(rand()%4);
    pacman.setPlayer(pacmans.size());

    pacmans.push_back(pacman);
}

void Jeu::suppr_pacman(void)
{
    if (!pacmans.empty()) pacmans.pop_back();
}

void Jeu::set_niveau_terrain(string niveau)
{
    this->niveau_terrain = niveau;
}

vector< vector <int> > Jeu::charger_niveau(const char path[]) const
{
    int lx = 0;
    string sLine;
    ifstream infile(path);

    vector<int> v;
    vector<vector <int> > level;

    if (infile.good())
    {
        while (getline(infile, sLine))
        {
            lx = sLine.size();

            for (int i=0;i<lx+1;i+=3)
            {
                v.push_back((sLine[i]-48)*10+(sLine[(i+1)]-48));
            }

            level.push_back(v);
            v.clear();
        }
    }

    else cout << "Impossible d'ouvrir : " << path << endl;

    return level;
}

Entity::Entity()
{
    type = FANTOME;
    posX = 0;
    posY = 0;
    dir = DROITE;
    godmode = false;
    player = 0;
}

Entity::Entity(Entity_type t,int x,int y,Direction direction,bool god, int p)
{
    type = t;
    posX = x;
    posY = y;
    dir = direction;
    godmode = god;
    player = p;
}

Entity::Entity(const Entity &entity)
{
    type = entity.type;
    posX = entity.posX;
    posY = entity.posY;
    dir = entity.dir;
    godmode = entity.godmode;
    player = entity.player;
}

Entity &Entity::operator=(const Entity &entity)
{
    type = entity.type;
    posX = entity.posX;
    posY = entity.posY;
    dir = entity.dir;
    godmode = entity.godmode;
    player = entity.player;

    return *this;
}

Entity_type Entity::getType() const
{
    return type;
}

int Entity::getPosX(void) const
{
    return posX;
}

int Entity::getPosY(void) const
{
    return posY;
}

Direction Entity::getDir(void) const
{
    return dir;
}

bool Entity::getGodmode(void) const
{
    return godmode;
}

int Entity::getPlayer() const
{
    return player;
}

void Entity::setType(Entity_type t)
{
    type = t;
}

void Entity::setPosX(int x)
{
    posX = x;
}

void Entity::setPosY(int y)
{
    posY = y;
}

void Entity::setDir(Direction direction)
{
    dir = direction;
}

void Entity::setGodmode(bool god)
{
    godmode = god;
}

void Entity::setPlayer(int p)
{
    player = p;
}

bool Jeu::deplace_entite(Entity &entity)
{
    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};
    int testX, testY;

    list<Pacman_entity>::iterator itPacman;
    list<Fantome_entity>::iterator itFantome;
    Case case_position;

    bool a_bouge = false;

    // Debut du jeu après le premier deplacement
    if (debut_jeu == false) 
    {
        debut_jeu = true;
        effets_sonors.push_back(sDEBUT);
    }

    if (fin_jeu == false) {

        testX = entity.getPosX() + depX[entity.getDir()];
        testY = entity.getPosY() + depY[entity.getDir()];

        // Deplacement dans les tunnels
        if (testX < 0) {
            entity.setPosX(largeur-1);
            a_bouge = true;
        }
        else if (testX > largeur-1) {
            entity.setPosX(0);
            a_bouge = true;
        }
            if (testY < 0) {
            entity.setPosY(hauteur-1);
            a_bouge = true;
        }
        else if (testY > hauteur-1) {
            entity.setPosY(0);
            a_bouge = true;
        }

        // Deplacement normal
        else if (posValide(testX, testY))
        {
            entity.setPosX(testX);
            entity.setPosY(testY);
            a_bouge = true;
        }

        // Comportement en cas d'entite de type pacman
        if (entity.getType() == PACMAN) {

            // Elimination des pacgums au contact des pacmans
            case_position = getCase(entity.getPosX(), entity.getPosY());

            if (case_position == PAC_GUM)
            {
                setCase(entity.getPosX(), entity.getPosY(), VIDE);
                nb_pac_gum--;
                score += PAC_GUM_SCORE*(fantomes.size()+1);
                if (nb_pac_gum == 0) fin_jeu = true;
                effets_sonors.push_back(sPAC_GUM);
            }
            else if (case_position == SUPER_GUM)
            {
                setCase(entity.getPosX(), entity.getPosY(), VIDE);
                nb_super_gum--;
                score += SUPER_GUM_SCORE*(fantomes.size()+1);
                temps_bleus = 50;
                pacman_super = entity.getPlayer();
                for (itPacman=pacmans.begin(); itPacman!=pacmans.end(); itPacman++) itPacman->setGodmode(true);
                for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++) itFantome->setGodmode(false);
                effets_sonors.push_back(sSUPER_GUM);
            }

            // Comportement au contact des fantomes
            for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
            {
                if(itFantome->posX == entity.getPosX() && itFantome->posY == entity.getPosY())
                {
                    // En cas d'effet de super gum
                    if (temps_bleus > 0 && itFantome->godmode == false) {
                            fantomes.erase(itFantome--);
                            score += MANGER_FANTOME_SCORE;
                            effets_sonors.push_back(sMANGER);
                    }

                    // En cas de contact mortel
                    else if (entity.getGodmode() == false) {

                        // Recherche et elimination du pacman correspondant
                        for (itPacman=pacmans.begin(); itPacman!=pacmans.end(); itPacman++)
                        {
                            if (itPacman->player == entity.getPlayer())
                            {
                                pacmans.erase(itPacman--);
                                effets_sonors.push_back(sMORT);
                                if (pacmans.empty())
                                {
                                    fin_jeu = true; // fin du jeu si le dernier pacman est mange
                                    effets_sonors.push_back(sFIN);
                                }

                                break;
                            }
                        }
                    }
                }
            }

            // Elimination des pacmans malchanceux
            if (entity.getPlayer() == pacman_super)
            {
                for (itPacman=pacmans.begin(); itPacman!=pacmans.end(); itPacman++)
                {
                    if(itPacman->posX == entity.getPosX() && itPacman->posY == entity.getPosY() && itPacman->player != entity.getPlayer())
                    {
                        pacmans.erase(itPacman--);
                        effets_sonors.push_back(sMANGER);
                    }
                }
            }
        }
    }

    return a_bouge;
}

void Jeu::placer_entite(Entity &entity)
{
    int x,y;

    do {
        x = rand()%largeur;
        y = rand()%hauteur;
    } while (!posValide(x, y));

    entity.setPosX(x);
    entity.setPosY(y);
}

bool Jeu::placer_a_entite(int x,int y, Entity &entity)
{
    if(posValide(x, y))
    {
        entity.setPosX(x);
        entity.setPosY(y);
        return true;
    }

    else return false;
}

Fantome_entity::Fantome_entity()
{
    this->setType(FANTOME);
    this->setPosX(0);
    this->setPosY(0);
    this->setDir(DROITE);
    this->setGodmode(true);
    this->setPlayer(1);

    couleur = ROUGE;
}

Fantome_entity::Fantome_entity(int x,int y,Direction direction)
{
    this->setType(FANTOME);
    this->setGodmode(true);
    this->setPlayer(1);

    this->setPosX(x);
    this->setPosY(y);
    this->setDir(direction);

    couleur = ROUGE;
}


Fantome_color Fantome_entity::getColor() const
{
    return couleur;
}

void Fantome_entity::setColor(Fantome_color c)
{
    couleur = c;
}

Pacman_entity::Pacman_entity()
{
    this->setType(PACMAN);
    this->setPosX(0);
    this->setPosY(0);
    this->setDir(DROITE);
    this->setGodmode(false);
    this->setPlayer(1);
}

Pacman_entity::Pacman_entity(int x,int y,Direction direction)
{
    this->setType(PACMAN);
    this->setGodmode(false);
    this->setPlayer(1);

    this->setPosX(x);
    this->setPosY(y);
    this->setDir(direction);
}

Pacman_entity::Pacman_entity(int x,int y,Direction direction, int p)
{
    this->setType(PACMAN);
    this->setGodmode(false);

    this->setPosX(x);
    this->setPosY(y);
    this->setDir(direction);
    this->setPlayer(p);
}
