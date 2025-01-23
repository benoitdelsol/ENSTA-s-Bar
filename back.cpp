#include <stdio.h>
#include <string>
#include <algorithm>
#include <random>
#include <iostream>
#include <vector>
#include <glib.h>

std::random_device rd;
std::mt19937 g(rd());

struct joueur
{
    std::string nom;
    std::vector<int> cartes;
    std::vector<gboolean> barrillet;
    // constructor
    joueur(std::string nom)
    {
        this->nom = nom;
        // créer un barrilet dont une valeur aléatoire est vraie
        for (int i = 0; i < 5; i++)
        {
            barrillet.push_back(false);
        }
        barrillet.push_back(true);
        std::shuffle(barrillet.begin(), barrillet.end(), g);
    }
};

void manche(std::vector<joueur> joueursEnVie)
{

    printf("Il reste %ld joueurs en vie\n", joueursEnVie.size());

    if(joueursEnVie.size() == 1){
        return;
    }

    std::vector<int> deck = {1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4};
    std::vector<int> defausse;
    // on choisit la carte à poser
    // choisir alléatoirement entre 1, 2 ou 3

    int carteMaitre = rand() % 3 + 1;
    printf("Carte à poser : %d\n", carteMaitre);

    // on mélange les cartes
    std::shuffle(deck.begin(), deck.end(), g);
    // on distribue les cartes
    int nbrJoueur = joueursEnVie.size();
    int nbrCartes = deck.size() / nbrJoueur;
    for (long unsigned int i = 0; i < joueursEnVie.size(); i++)
    {
        // supprimer les cartes du joueur
        joueursEnVie[i].cartes.clear();
        for (int j = 0; j < nbrCartes; j++)
        {
            joueursEnVie[i].cartes.push_back(deck[i * nbrCartes + j]);
        }
    }

    long unsigned int i = 0;

    while (joueursEnVie.size() > 1)
    {
        if (i >= joueursEnVie.size())
        {
            i = 0;
        }
        // si le joueur n'a plus de cartes il dit automatiquement que le joueur précédent a menti
        if (joueursEnVie[i].cartes.size() == 0)
        {
            printf("%s n'a plus de cartes\n", joueursEnVie[i].nom.c_str());
            // le joueur précédent a menti
            // on il se tire une balle
            if (i == 0)
            {
                printf("%s a menti !\n", joueursEnVie[joueursEnVie.size() - 1].nom.c_str());
                printf("%s tire !\n", joueursEnVie[joueursEnVie.size() - 1].nom.c_str());
                gboolean balle = joueursEnVie[joueursEnVie.size() - 1].barrillet[0];
                joueursEnVie[joueursEnVie.size() - 1].barrillet.erase(joueursEnVie[joueursEnVie.size() - 1].barrillet.begin());
                if (balle)
                {
                    printf("%s est mort\n", joueursEnVie[joueursEnVie.size() - 1].nom.c_str());
                    joueursEnVie.erase(joueursEnVie.begin() + joueursEnVie.size() - 1);
                }
                else
                {
                    printf("%s a survécu\n", joueursEnVie[joueursEnVie.size() - 1].nom.c_str());
                    //print le nombre de tentatives / 5
                    printf("Le joueur a survécu %ld fois\n", 5 - joueursEnVie[joueursEnVie.size() - 1].barrillet.size());
                }
            }
            else
            {
                printf("%s a menti !\n", joueursEnVie[i - 1].nom.c_str());
                printf("%s tire !\n", joueursEnVie[i - 1].nom.c_str());
                gboolean balle = joueursEnVie[i - 1].barrillet[0];
                joueursEnVie[i - 1].barrillet.erase(joueursEnVie[i - 1].barrillet.begin());
                if (balle)
                {
                    printf("%s est mort\n", joueursEnVie[i - 1].nom.c_str());
                    joueursEnVie.erase(joueursEnVie.begin() + i - 1);
                }
                else
                {
                    printf("%s a survécu\n", joueursEnVie[i - 1].nom.c_str());
                    //print le nombre de tentatives / 5
                    printf("Le joueur a survécu %ld fois\n", 5 - joueursEnVie[i - 1].barrillet.size());
                }
            }
        }
        // on print les joueurs en vie en commancant par le joueur actuel
        printf("Joueurs en vie : ");
        for (long unsigned int j = 0; j < joueursEnVie.size(); j++)
        {
            if (j + i >= joueursEnVie.size())
            {
                printf("%s ,", joueursEnVie[j + i - joueursEnVie.size()].nom.c_str());
            }
            else
            {
                printf("%s ,", joueursEnVie[i+j].nom.c_str());
            }
        }
        printf("\n");
        // on lui print ses cartes
        printf("%s a les cartes suivantes : ", joueursEnVie[i].nom.c_str());
        for (int carte : joueursEnVie[i].cartes)
        {
            printf("%d ", carte);
        }
        printf("\n");
        // on lui demande soit de choisir de poser une ou plusieurs cartes soit de traiter le joueur d'avant de menteur
        // si le joueur est traité de menteur, on vérifie si le joueur a menti ou pas

        if (defausse.size() > 0)
        {
            //print le nombre de cartes posé par le joueur précédent
            printf("Le joueur précédent a posé %ld cartes\n", defausse.size());
            // proposer de dire que le joueur précédent a menti
            printf("Voulez-vous dire que le joueur précédent a menti ? (oui/non)\n");
            char reponse[4] = "";
            while (strcmp(reponse, "oui") != 0 && strcmp(reponse, "non") != 0)
            {
                if (fgets(reponse, sizeof(reponse), stdin) != NULL)
                {
                    // Retirer le saut de ligne ajouté par fgets
                    reponse[strcspn(reponse, "\n")] = '\0';
                }
                else
                {
                    printf("Erreur lors de la lecture de votre réponse.\n");
                }
            }

            if (strcmp(reponse, "oui") == 0)
            {

                // pour vérifier si le joueur précédent à menti on vérifie que toutes les cartes de la défausse sont égales a la carte a poser

                for (int carte : defausse)
                {
                    if (carte != carteMaitre)
                    {
                        // le joueur précédent a menti
                        // on il se tire une balle
                        if (i == 0)
                        {
                            printf("%s a menti !\n", joueursEnVie[joueursEnVie.size() - 1].nom.c_str());
                            printf("%s tire !\n", joueursEnVie[joueursEnVie.size() - 1].nom.c_str());
                            gboolean balle = joueursEnVie[joueursEnVie.size() - 1].barrillet[0];
                            joueursEnVie[joueursEnVie.size() - 1].barrillet.erase(joueursEnVie[joueursEnVie.size() - 1].barrillet.begin());
                            if (balle)
                            {
                                printf("%s est mort\n", joueursEnVie[joueursEnVie.size() - 1].nom.c_str());
                                joueursEnVie.erase(joueursEnVie.begin() + joueursEnVie.size() - 1);
                            }
                            else
                            {
                                printf("%s a survécu\n", joueursEnVie[joueursEnVie.size() - 1].nom.c_str());
                                //print le nombre de tentatives / 5
                                printf("Le joueur a survécu %ld fois\n", 5 - joueursEnVie[joueursEnVie.size() - 1].barrillet.size());
                            }
                        }
                        else
                        {
                            printf("%s a menti !\n", joueursEnVie[i - 1].nom.c_str());
                            printf("%s tire !\n", joueursEnVie[i - 1].nom.c_str());
                            gboolean balle = joueursEnVie[i - 1].barrillet[0];
                            joueursEnVie[i - 1].barrillet.erase(joueursEnVie[i - 1].barrillet.begin());
                            if (balle)
                            {
                                printf("%s est mort\n", joueursEnVie[i - 1].nom.c_str());
                                joueursEnVie.erase(joueursEnVie.begin() + i - 1);
                            }
                            else
                            {
                                printf("%s a survécu\n", joueursEnVie[i - 1].nom.c_str());
                                //print le nombre de tentatives / 5
                                printf("Le joueur a survécu %ld fois\n", 5 - joueursEnVie[i - 1].barrillet.size());
                            }
                        }
                        manche(joueursEnVie);
                        return;
                    }
                }
                //print que le joueur précédent n'a pas menti
                printf("Le joueur précédent n'a pas menti\nOn tire une balle !\n");

                // le joueur précédent n'a pas menti
                // le joueur actuel se tire une balle
                gboolean balle = joueursEnVie[i].barrillet[0];
                joueursEnVie[i].barrillet.erase(joueursEnVie[i].barrillet.begin());
                if (balle)
                {
                    printf("%s est mort\n", joueursEnVie[i].nom.c_str());
                    // le joueur est mort
                    joueursEnVie.erase(joueursEnVie.begin() + i);
                }
                else
                {
                    printf("%s a survécu\n", joueursEnVie[i].nom.c_str());
                    //print le nombre de tentatives / 5
                    printf("Le joueur a survécu %ld fois\n", 5 - joueursEnVie[i].barrillet.size());
                }
                manche(joueursEnVie);
                return;
            }
            // si le joueur dit que le joueur précédent a menti, on vérifie si c'est vrai
            // si c'est vrai le joueur précédent tire un balle de son barrillet
            // si c'est faux le joueur tire une balle de son barrillet
            // si le joueur ne dit pas que le joueur précédent a menti, il doit poser une ou plusieurs cartes
        }
        defausse.clear();
        printf("Combien de cartes voulez-vous poser ? \n");
        long unsigned int nbrCartesAPoser = 0;
        // vérifier que c'est possible de poser autant de cartes
        while (nbrCartesAPoser == 0)
        {
            scanf("%ld", &nbrCartesAPoser);
            if (nbrCartesAPoser <= 0)
            {
                printf("Vous devez poser au moins une carte\n");
            }
            else if (nbrCartesAPoser > joueursEnVie[i].cartes.size())
            {
                printf("Vous ne pouvez pas poser autant de cartes\n");
                nbrCartesAPoser = 0;
            }
        }
        for (long unsigned int j = 0; j < nbrCartesAPoser; j++)
        {
            printf("Quelle carte voulez-vous poser ? \n");
            int carteAPoser = 0;
            while (carteAPoser == 0)
            {
                scanf("%d", &carteAPoser);
                if (carteAPoser <= 0 || carteAPoser > 4)
                {
                    printf("Vous devez poser une carte\n");
                }
                else if (std::find(joueursEnVie[i].cartes.begin(), joueursEnVie[i].cartes.end(), carteAPoser) == joueursEnVie[i].cartes.end())
                {
                    printf("Vous ne pouvez pas poser cette carte\n");
                    carteAPoser = 0;
                }
            }
            defausse.push_back(carteAPoser);
            joueursEnVie[i].cartes.erase(std::find(joueursEnVie[i].cartes.begin(), joueursEnVie[i].cartes.end(), carteAPoser));
        }
        i++;
    }
}

int main()
{
    std::string nom1 = "joueur1";
    std::string nom2 = "joueur2";
    std::string nom3 = "joueur3";
    std::string nom4 = "joueur4";
    joueur joueur1(nom1);
    joueur joueur2(nom2);
    joueur joueur3(nom3);
    joueur joueur4(nom4);
    std::vector<joueur> joueursEnVie;
    joueursEnVie.push_back(joueur1);
    joueursEnVie.push_back(joueur2);
    joueursEnVie.push_back(joueur3);
    joueursEnVie.push_back(joueur4);

    manche(joueursEnVie);
    printf("Le jeu est terminé\n");
    //print le joueur gagnant
    printf("Le joueur gagnant est %s\n", joueursEnVie[0].nom.c_str());
    return 0;
}
