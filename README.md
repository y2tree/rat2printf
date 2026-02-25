*Ce projet a été réalisé dans le cadre du cursus 42 par ylerges.*

# ft_printf

## Description

Réimplémentation de la fonction printf() de la libc. L'objectif est de comprendre le fonctionnement des fonctions variadiques en C en construisant une bibliothèque qui gère les conversions de format les plus courantes. Le résultat est compilé sous forme de bibliothèque statique `libftprintf.a`.

Conversions supportées : `%c` `%s` `%p` `%d` `%i` `%u` `%x` `%X` `%%`

## Instructions

```bash
make        # compile libftprintf.a
make clean  # supprime les fichiers .o
make fclean # supprime les .o et la bibliothèque
make re     # recompile tout
```

Pour l'utiliser dans un projet :
```bash
cc main.c -L. -lftprintf -o mon_programme
```

Exemple rapide :
```c
#include "ft_printf.h"

int main(void)
{
    int n = 42;
    ft_printf("nombre: %d, hex: %x, ptr: %p\n", n, n, &n);
    return (0);
}
```

## Algorithme et structure de données

La fonction parcourt la chaîne de format caractère par caractère. Quand elle rencontre un `%`, elle lit le caractère suivant pour déterminer quelle conversion appliquer, puis appelle la fonction correspondante.

Pas de buffer — chaque caractère ou nombre est écrit directement sur stdout via `write()`. La valeur de retour comptabilise le nombre total de caractères affichés.

Pour les nombres, une seule fonction `ft_putnbr_base()` gère toutes les conversions numériques (décimal, unsigned, hex minuscule/majuscule) en prenant le nombre et la base sous forme de chaîne. Ca évite de dupliquer la logique. Les nombres négatifs sont traités dans `ft_putnbr()` en castant en `long` avant la négation, pour éviter l'overflow sur `INT_MIN`. Les pointeurs sont castés en `unsigned long` avant d'être passés au convertisseur hex.

## Ressources

- `man 3 printf` — documentation de printf
- `man stdarg` — fonctionnement de va_list, va_start, va_arg, va_end
- https://cppreference.com/w/c/io/fprintf — référence des spécificateurs de conversion

**Utilisation de l'IA :** L'IA a été utilisée pour aider à rédiger ce README et pour vérifier les cas limites (INT_MIN, pointeur NULL pour %s). Le code a été écrit manuellement.
