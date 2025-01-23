#include <stdio.h>
#include <string>
#include <algorithm>
#include <random>
#include <iostream>
#include <vector>


int main(){
    std::string nom1 = "joueur1";
    std::string nom2 = "joueur1 ";
    //les comparer en prenant seulement les 5 premiers caractères

    bool balle = nom1.substr(0,5)==nom2.substr(0,5);
    if(balle){
        printf("Les noms sont les mêmes\n");
    }
    else{
        printf("Les noms sont différents\n");
    }
    printf("%d\n", balle);
}