#ifndef DATABASE_H
#define DATABASE_H

#include "model.h"

namespace database
{
bool openConnection();
}

namespace utils {

/*
 * le prototype ET la définition d'une fonction template doivent
 * obligatoirement se trouver dans un fichier d'en-tête.
 * le type que l'on passe à la fonction doit posséder un operator>
 * si la classe de type T possede un operator> don c'est ok sinon ne marche pas
 * Les contraintes dépendent des fonctions que vous écrivez
 */
template<typename T>
T maximum(const T &a, const T &b)
{
    if(a> b)
        return a;
    return b;
}

// fonction special pour le tupe string
// L'ordre des fonctions est important
// la fonction générique
// les fonctions spécialisées

/*
template <>
std::string maximum<std::string>(const std::string& a, const std::string& b)
{
    if(a.size()>b.size())
        return a;
    else
        return b;
}
*/
}

#endif // DATABASE_H
