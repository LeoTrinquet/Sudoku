#include <vector>
#include <string>
#include <fstream> // pour les lecture-ecriture dans les fichiers
#include <iostream>

using namespace std;

typedef struct
{
    string nom;
    vector<vector<int>> grille;
} Grille;

typedef struct
{
    size_t lig;
    size_t col;
} Indices;

/********************************************
*                  imprimeL                 *
********************************************/

/* Imprime une ligne de s�paration form�e du caract�re pass� en argument */
void imprimeL(char c)
{
    std::cout << ' ';
    for(size_t i = 0; i < 9; i += 1)
    {
        std::cout << c << c << c << c;
    }
    /* pour prendre en compte les espaces ajout�s pr�sb des '+' */
    std::cout << c << c << c << c << c << c;
    std::cout << endl;
}

/* suppose la grille bien formee: 9 lignes de 9 colonnes de chiffres de 0 � 9 */
void imprime(const Grille &g)
{
    for(size_t i = 0; i < 9; i += 1)
    {
        imprimeL(i % 3 == 0 ? '+' : '-'); // ligne d'en-tete
        for(size_t j = 0; j < 9; j += 1)
        {
            char c = '0' + g.grille.at(i).at(j);
            std::cout << (j % 3 == 0 ? " + " : " " ) << ' ' << c << ' ';
        }
        std::cout << "+" << endl;
    }
    imprimeL('+'); // ligne finale du bas
    std::cout << endl;
}


/***********************************************
*                  bienFormee                  *
***********************************************/

/* Renvoie true ssi la grille a la bonne forme et ne contient que des valeurs
 * de 0 � 9
 */
bool bienFormee(const Grille &g)
{
    if (g.grille.size() != 9)   //On regarde si la grille poss�de bien 9 lignes.
    {
        return false;
    }

    for (size_t n=0 ; n<g.grille.size() ; n++) //On regarde si la grille poss�de bien 9 colonnes.
    {
        if (g.grille.at(n).size() != 9)
        {
            return false;
        }
    }


    for (size_t i=0 ; i<g.grille.size() ; i++)     //On regarde dans chaque ligne,
    {
        for (size_t j=0 ; j<g.grille.size() ; j++)     //Puis dans chaque colonne de la ligne en question,
        {
            if (g.grille.at(i).at(j)<0 or g.grille.at(i).at(j)>9)   //Si la valeur de chacune des cases est bien comprise entre 0 et 9 inclus.
            {
                return false;
            }
        }
    }
    return true;    //Si apr�s avoir parcouru toutes les cases, aucune erreur n'a �t� constat�e, alors la grille est valide.
}

/***********************************************
*                    valide                    *
***********************************************/

/* Verifie si la grille est valide: chaque valeur de 1 � 9 apparait au plus
 * une fois dans chaque ligne, colonne et r�gion
 */
bool valide(const Grille &g)
{
    size_t  compteur = 0;


//Test si toute valeur entre 1 et 9 appara�t exactement 1 fois dans chaque ligne

    for (size_t  l=0 ; l<g.grille.size() ; l++)     //On regarde dans chaque ligne.
    {
        for (int k=1; k<=9 ; k++)   //Pour chaque valeur (de 1 � 9) si elle apparait bien une seule fois dans chaque ligne.
        {
            for (size_t c=0 ; c<g.grille.size() ; c++)     //On parcourt donc chaque case d'une m�me ligne.
            {
                if (g.grille.at(l).at(c) == k)      //� chaque fois qu'on retrouve une valeur identique � k, on ajoute 1 au compteur.
                {
                    compteur += 1;
                }
            }
            if (compteur > 1)  //Si apr�s avoir parcouru toute la ligne, le compteur n'est pas �gal � 1, alors cela veut dire qu'il y a une erreur.
            {
                return false;
            }
            compteur = 0;   //On r�initialise le compteur apr�s avoir parcouru toute la ligne pour une valeur de k
        }                   //On r�p�te l'op�ration pour chaque valeur de k allant de 1 � 9.
    }                       //On fait ceci pour chaque ligne.


//Test si toute valeur entre 1 et 9 appara�t exactement 1 fois dans chaque colonne

//On fait la m�me chose que pr�c�demment en inversant les lignes et les colonnes.

    for (size_t c=0 ; c<g.grille.size() ; c++)
    {
        for (int k=1; k<=9 ; k++)
        {
            for (size_t l=0 ; l<g.grille.size() ; l++)
            {
                if (g.grille.at(l).at(c) == k)
                {
                    compteur += 1;
                }
            }
            if (compteur > 1)
            {
                return false;
            }
            compteur = 0;
        }
    }


//Test si toute valeur entre 1 et 9 appara�t exactement 1 fois dans chaque r�gion

    for (size_t x=0 ; x<=6 ; x = x+3)  //Permet de passer d'une r�gion � une autre verticalement
    {
        for (size_t y=0 ; y<=6 ; y = y+3) //Permet de passer d'une r�gion � une autre horizontalement
        {
            for (int k=1; k<=9 ; k++) //Pour chaque valeur allant de 1 � 9;
            {
                for (size_t l=0 ; l<3 ; l++) //On regarde dans chaque ligne d'une r�gion;
                {
                    for (size_t c=0 ; c<3 ; c++) //On regarde dans chaque case de chaque ligne d'une r�gion;
                    {
                        if (g.grille.at(l+x).at(c+y) == k) //� chaque fois qu'on retrouve dans une r�gion, une valeur identique � k, on ajoute 1 au compteur.
                        {
                            compteur += 1;
                        }
                    }
                }
                if (compteur > 1) //Si, apr�s avoir parcouru toutes les cases de la r�gion, le compteur est sup�rieur � 1, alors il y a une erreur.
                {
                    return false;
                }
                compteur = 0; //On r�initialise le compteur apr�s avoir parcouru toute les cases d'une r�gion pour une valeur de k.
            }                 //On r�p�te l'op�ration pour chaque valeur de k allant de 1 � 9.
        }                     //On passe � la r�gion suivante puis on r�it�re l'op�ration.
    }                         //On fait ceci pour toutes les r�gions, en les parcourant de gauche � droite et de haut en bas.

    return true;              //Si aucune erreur n'a �t� d�tect�e jusque l�, alors la grille est bien valide.
}

/**************************************************
*                    possibles                    *
**************************************************/

/* renvoie le vecteur des valeurs disponibles pour le couple d'indices ind,
 * compte-tenu des valeurs plac�es sur sa ligne, colonne et sa r�gion
*/


vector<int> possibles(const Grille &g, Indices ind)
{
    vector<int> res;
    res = {1,2,3,4,5,6,7,8,9};

//Retire les valeurs non disponibles dans la ligne

//On regarde toutes les cases d'une ligne entr�e en indice et on modifie par un 0 dans le tableau res les valeurs rencontr�es.

    for (size_t c=0 ; c<g.grille.size() ; c++)
    {
        if (g.grille.at(ind.lig).at(c) != 0)
        {
            res.at(g.grille.at(ind.lig).at(c)-1) = 0;
        }
    }

//Retire les valeurs non disponibles dans la colonne

//On fait la m�me chose avec une colonne entr�e en indice.

    for (size_t l=0 ; l<g.grille.size() ; l++)
    {
        if (g.grille.at(l).at(ind.col) != 0)
        {
            res.at(g.grille.at(l).at(ind.col)-1) = 0;
        }
    }

//Retire les valeurs non disponibles dans la r�gion

    size_t x,y;

//En fonction de la ligne et de la colonne entr�es en indice, on d�tecte la r�gion correspondante.
    if (ind.lig <= 2){x=0;}
    else if (ind.lig >= 3  and ind.lig <= 5){x=3;}
    else{x=6;}

    if (ind.col <= 2){y=0;}
    else if (ind.col >= 3 and ind.col <= 5){y=3;}
    else{y=6;}

//On regarde toutes les cases de la r�gion d�tect�e et on modifie par un 0 dans le tableau res les valeurs rencontr�es.
    for (size_t l=0 ; l<3 ; l++)
    {
        for (size_t c=0 ; c<3 ; c++)
        {
            if (g.grille.at(l+x).at(c+y) != 0)
            {
                res.at(g.grille.at(l+x).at(c+y)-1) = 0;
            }
        }
    }

//On copie dans le tableau res_final toutes les valeurs diff�rentes de 0 du tableau res.
    vector<int> res_final;
    for (size_t n=0 ; n<res.size() ; n++)
    {
        if (res.at(n) != 0)
        {
            res_final.push_back(res.at(n));
        }
    }

    return res_final;
}

/**********************************************
*                    force                    *
**********************************************/

/* recoit en argument le vecteur des valeurs possibles pour la case
 * d'indices ij. Parcourt les valeurs en cherchant si pour l'une d'entre
 * elles il existe un seul emplacement valide dans la r�gion.
 * Dans ce cas, stocke la valeur dans la case et renvoie true.
 * Renvoie false si pour chaque valeur possible il existe au moins deux
 * cases o� on pourrait potentiellement la ranger. Dans ce cas ne
 * modifie pas la grille.
 */
bool force(Grille &g, Indices ij, vector<int> vpossibles)
{
    size_t x,y;
    bool trouve = 0;

//En fonction de la ligne et de la colonne entr�es en indice, on d�tecte la r�gion correspondante.
    if (ij.lig <= 2){x=0;}
    else if (ij.lig >= 3  and ij.lig <= 5){x=3;}
    else{x=6;}

    if (ij.col <= 2){y=0;}
    else if (ij.col >= 3 and ij.col <= 5){y=3;}
    else{y=6;}

//Si la case ne poss�de qu'une et une seule valeur possible, alors on la remplace directement par cette valeur et on arr�te le programme.

    if (g.grille.at(ij.lig).at(ij.col) == 0 and vpossibles.size() == 1)
    {
        g.grille.at(ij.lig).at(ij.col) = vpossibles.at(0);
        return true;
    }

//Pour chaque valeur du vecteur possible entr� en param�tre
//On regarde pour toutes les cases qui contiennent un 0 hormi la case entr�e en indice
//Si elles contiennent la m�me valeur du vecteur possible, parmi leurs valeurs autoris�es.
//Si c'est le cas, on continue jusqu'� trouver une valeur qui n'est pr�sente � aucun autre endroit dans la r�gion, de sorte � modifier la case de la grille par cette valeur.
//Si toutes les valeurs sont pr�sentes plusieurs fois dans la r�gion, alors la grille reste inchang�e.
    for (size_t k=0 ; k<vpossibles.size() ; k++)
    {
        for (size_t l=0 ; l<3 ; l++)
        {
            if (trouve) //Si on retrouve la valeur de k comme possibilit� dans une autre case de la r�gion, on passe directement au k suivant.
            {
                break;
            }
            for (size_t c=0 ; c<3 ; c++)
            {
                if (trouve) //Si on retrouve la valeur de k comme possibilit� dans une autre case de la r�gion, on passe directement au k suivant.
                {
                    break;
                }

                if (g.grille.at(l+x).at(c+y) == 0 and (l+x != ij.lig or c+y != ij.col)) //S'il s'agit d'une case contenant un 0 et que ce n'est pas celle entr�e en param�tre.
                {
                    for (size_t n=0 ; n<possibles(g, {l+x, c+y}).size() ; n++) //On parcourt les valeurs autoris�es des autres cases vides de la r�gion.
                    {
                        if (vpossibles.at(k) < possibles(g, {l+x, c+y}).at(n)) //Si k est inf�rieur � la plus petite valeur autoris�e de la case concern�e,
                        {                                                      //alors il est inutile de continuer, on passe � la prochaine case.
                            break;
                        }
                        else if (vpossibles.at(k) == possibles(g, {l+x, c+y}).at(n)) //Si une valeur autoris�e d'une case est bien identique � k, alors on passe au k suivant
                        {                                                            //en utilisant plusieurs break, de sorte � ne pas faire tourner le programme dans le vide.
                            trouve = true;
                            break;
                        }
                    }
                }
            }
        }
//Si apr�s avoir parcouru toutes les cases pour une valeur k, il n'existe aucune autre
//possibilit� de la placer dans la r�gion, on modifie la case de la grille par cette valeur.
        if (!trouve)
        {
            g.grille.at(ij.lig).at(ij.col) = vpossibles.at(k);
            return true;
        }
        trouve = false; //On r�initialise la variable trouve avant de r�p�ter l'op�ration pour la prochaine valeur de k.
    }

    return false; //Si le programme n'a pas �t� arr�t� jusque l�, c'est que toutes les valeurs autoris�es de la case
}                 //entr�e en param�tre sont pr�sentes plusieurs fois dans la r�gion. La grille n'a donc pas �t� modifi�e.

/****************************************************
*                    userSuggest                    *
****************************************************/
//On demande � l'utilisateur s'il veut saisir une valeur :
//�Si oui, on lui demande la valeur et l'indice de la case � modifier, puis on v�rifie si la case n'est pas vide et si elle poss�de bien la valeur entr�e comme valeur possible.
//�Si non, on arr�te le programme.
//�Si autre r�ponse, on affiche une erreur et on arr�te le programme.

bool userSuggest(Grille &g)
{
    size_t ind_lig,ind_col;
    int valeur;
    string choix;

    std::cout<<"Souhaitez-vous suggerer une valeur ? (oui ou non)"<<endl;
    std::cin>>choix;
    if (choix == "oui" or choix == "Oui" or choix == "OUI")
    {
        std::cout<<"Veuillez entrer une valeur a ajouter dans la grille"<<endl;
        std::cin>>valeur;
        std::cout<<"Veuillez maintenant entrer l'indice de la case dans laquelle vous voulez entrer cette valeur"<<endl;
        std::cout<<"De la forme suivante : n ligne n colonne"<<endl;
        std::cout<<"Exemple : 3 5 pour la case associee a la 3 eme ligne et la 5 eme colonne de la grille"<<endl;
        std::cin>>ind_lig>>ind_col;

        if (g.grille.at(ind_lig-1).at(ind_col-1) != 0) //Erreur si la case n'est pas vide.
        {
            std::cout<<"Erreur, la case choisie contient d�j� une valeur"<<endl;
            return false;
        }
        for (size_t n=0 ; n<possibles(g, {ind_lig-1, ind_col-1}).size() ; n++) //On parcourt les valeurs possibles de cette case.
        {
            if (possibles(g, {ind_lig-1, ind_col-1}).at(n) == valeur) //Si la valeur entree par l'utilisateur correspond bien a une valeur
            {                                                         //possible de la case, alors on remplace la case par cette valeur.
                g.grille.at(ind_lig-1).at(ind_col-1) = valeur;
                return true;
            }
        }
        std::cout<<"Impossible, la valeur suggeree ne peut etre remplacee dans cette case"<<endl;
        return false;
    }
    else if (choix == "non" or choix == "Non" or choix == "NON")
    {
        return false;
    }
    else
    {
        std::cout<<"Erreur...Veuillez repondre par oui ou par non"<<endl;
        return false;
    }
}

/*********************************************
*                    joue                    *
*********************************************/

bool joue(Grille &g)
{
    if (!bienFormee(g) or !valide(g))
    {
        std::cout<<"Erreur, grille "<<g.nom<<" non valide"<<endl;
        return false;
    }

    std::cout << "Debut du traitement de la grille " << g.nom << endl << endl;
    std::cout << "Grille initiale :"<<endl<<endl;
    imprime(g);

    size_t compteur = 0;
    size_t compteurtest = 0, compteurtour = 0; //Affichage console
    bool cherche = true;
    bool termine, reponse;
    string choix;

    while(cherche == true)
    {
        compteurtest = 0; // Affichage console
        std::cout<<"Tour numero "<<compteurtour+1<<":"<<endl;
        termine = true;
        for (size_t k1=0 ; k1<g.grille.size() ; k1++) //Dans chaque ligne.
        {
            for (size_t k2=0 ; k2<g.grille.size() ; k2++) //Dans chaque colonne.
            {
                Indices endroit = {k1,k2};
                vector<int> valpossibles = possibles(g, endroit ); //Contient les valeurs possible de la case en cours
                bool testforce = force(g, endroit, valpossibles); //Teste s'il est possible de remplir cette case par une valeur. Si oui, la remplie.
                if (testforce) // Affichage console
                {
                    std::cout<<"Case ("<<k1+1<<", "<<k2+1<<") <- "<<g.grille.at(k1).at(k2)<<endl; //
                    compteurtest += 1; //
                }

                if(!testforce) //Si plusieurs possibilit�s pour chaque valeur et donc aucune case modifi�e : compteur+1
                {
                    compteur += 1;
                }
                if (g.grille.at(k1).at(k2) == 0) //Si au moins une case est vide, grille pas termin�e
                {
                    termine = false;
                }
            }
        }
        if (termine == true) //Si apr�s avoir parcouru toutes les cases, grille termin�e, affiche la grille finale et arr�te le programme.
        {
            std::cout<<"Cases remplies automatiquement : "<<compteurtest<<endl;
            std::cout<<endl<<endl<<"La grille a ete resolue !"<<endl<<endl;
            std::cout<<"Grille Resolue :"<<endl<<endl;
            imprime(g);
            return true;
        }
        else if (compteur == 81) //Si apr�s avoir parcouru toutes les cases, aucune case n'est modifi�e mais que grille non termin�e, stop la recherche.
        {
            cherche = false;
        }

        compteur = 0;
        std::cout<<"Cases remplies automatiquement : "<<compteurtest<<endl;
        imprime(g);
        compteurtour += 1;
    }

    std::cout<<endl<<endl<<"Voici l'avancement de la grille jusqu'ici :"<<endl<<endl;
    imprime(g);

//Tant que l'utilisateur souhaite sugg�rer une valeur, il peut continuer � en proposer jusqu'� ce qu'il veuille arr�ter.
    do
    {
        if (!userSuggest(g))
        {
            reponse = false;
        }
        else
        {
            imprime(g);
        }
    } while(reponse == true);


//Une fois qu'il a fini d'en sugg�rer, il peut d�cider de faire continuer la grille ou non.
//Si non, si la grille n'est pas r�solue, retourne false, sinon, retourne true.
    for (size_t l=0 ; l<g.grille.size() ; l++)
    {
        for (size_t c=0 ; c<g.grille.size() ; c++)
        {
            if (g.grille.at(l).at(c) == 0)
            {
                std::cout<<"Souhaitez-vous que nous tentions de completer la grille a nouveau ? (oui ou non)"<<endl;
                std::cin>>choix;
                if (choix == "oui" or choix == "Oui" or choix == "OUI")
                {
                    if (joue(g))
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else if (choix == "non" or choix == "Non" or choix == "NON")
                {
                    std::cout<<"La grille n'a pas ete resolue, dommage..."<<endl;
                    return false;
                }
                else
                {
                    std::cout<<"Erreur...Veuillez repondre par oui ou par non"<<endl;
                    std::cout<<"La grille n'a pas ete resolue."<<endl;
                    return false;
                }
            }
        }
    }

    std::cout<<"La grille a ete resolue, bravo !"<<endl;
    imprime(g);
    return true;
}

/*********************************************
*                    main                    *
*********************************************/

/* quelques grilles pr�d�finies */
int main()
{
    // Les grilles G2 � G4 peuvent se r�soudre sans aide de l'utilisateur
        Grille erreur1 = { "erreur1",
        {   { 5, 3, 0, 0, 7, 0, 0, 0, 0 },
            { 6, 0, 0, 1, 9, 5, 0, 0, 0 },
            { 0, 9, 8, 0, 0, 0, 0, 6, 0 },
            { 8, 0, 0, 10, 6, 0, 0, 0, 3 },
            { 4, 0, 0, 8, 0, 3, 0, 0, 1 },
            { 7, 0, 0, 0, 2, 0, 0, 0, 6 },
            { 0, 6, 0, 0, 0, 0, 2, 8, 0 },
            { 0, 0, 0, 4, 1, 9, 0, 0, 5 },
          //{ 0, 0, 0, 0, 8, 0, 0, 7, 9 }
        }
    };

        Grille erreur2 = { "erreur2",
        {   { 5, 5, 0, 0, 7, 0, 0, 0, 0 },
            { 6, 0, 0, 1, 9, 5, 0, 0, 0 },
            { 0, 9, 8, 0, 0, 0, 0, 6, 0 },
            { 8, 0, 0, 0, 6, 0, 0, 0, 3 },
            { 4, 0, 0, 8, 0, 3, 0, 0, 1 },
            { 7, 0, 0, 0, 2, 0, 0, 0, 6 },
            { 0, 6, 0, 0, 0, 0, 2, 8, 0 },
            { 0, 0, 0, 4, 1, 9, 0, 0, 5 },
            { 0, 0, 0, 0, 8, 0, 0, 7, 9 }
        }
    };

    Grille g2 = { "G2",
        {   { 5, 3, 0, 0, 7, 0, 0, 0, 0 },
            { 6, 0, 0, 1, 9, 5, 0, 0, 0 },
            { 0, 9, 8, 0, 0, 0, 0, 6, 0 },
            { 8, 0, 0, 0, 6, 0, 0, 0, 3 },
            { 4, 0, 0, 8, 0, 3, 0, 0, 1 },
            { 7, 0, 0, 0, 2, 0, 0, 0, 6 },
            { 0, 6, 0, 0, 0, 0, 2, 8, 0 },
            { 0, 0, 0, 4, 1, 9, 0, 0, 5 },
            { 0, 0, 0, 0, 8, 0, 0, 7, 9 }
        }
    };
    Grille g3 = { "G3",
        {   { 0, 2, 7, 0, 6, 5, 0, 4, 9 },
            { 3, 5, 0, 0, 0, 9, 0, 0, 8 },
            { 8, 9, 0, 0, 0, 0, 0, 0, 0 },
            { 2, 0, 0, 0, 9, 0, 0, 0, 0 },
            { 0, 0, 0, 6, 8, 7, 0, 0, 0 },
            { 0, 0, 0, 0, 3, 0, 0, 0, 5 },
            { 0, 0, 0, 0, 0, 0, 0, 6, 3 },
            { 7, 0, 0, 2, 0, 0, 0, 8, 4 },
            { 4, 8, 0, 9, 7, 0, 1, 5, 0}
        }
    };
    Grille g4 = { "G4",
        {   { 8, 0, 0, 0, 0, 0, 1, 0, 0 },
            { 0, 3, 4, 0, 2, 0, 6, 0, 0 },
            { 0, 9, 0, 4, 0, 0, 0, 0, 2 },
            { 5, 1, 0, 0, 4, 2, 9, 6, 0 },
            { 0, 0, 0, 0, 6, 0, 0, 0, 0 },
            { 0, 6, 7, 1, 5, 0, 0, 2, 3 },
            { 9, 0, 0, 0, 0, 4, 0, 3, 0 },
            { 0, 0, 2, 0, 1, 0, 7, 4, 0 },
            { 0, 0, 8, 0, 0, 0, 0, 0, 1 }
        }
    };
    // cette grille admet deux solutions diff�rentes mais demande une aide
    // de l'utilisateur si on se limite aux deux autres strat�gies d�finies
    Grille g5 = { "G5",
        {   { 0, 0, 8, 0, 0, 0, 2, 0, 0 },
            { 0, 0, 4, 3, 0, 0, 0, 0, 1 },
            { 0, 5, 0, 0, 0, 7, 0, 8, 9 },
            { 0, 0, 5, 7, 4, 0, 0, 0, 2 },
            { 0, 0, 0, 1, 0, 3, 0, 0, 0 },
            { 4, 0, 0, 0, 2, 0, 6, 0, 0 },
            { 2, 6, 0, 5, 0, 0, 0, 7, 0 },
            { 5, 0, 0, 0, 0, 2, 9, 0, 0 },
            { 0, 0, 1, 0, 0, 0, 5, 0, 0 }
        }
    };

    joue(g5);

    return 0;
}