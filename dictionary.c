//Bismillahirrahmanirrahim.

// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include "dictionary.h"
#include <stdlib.h>

const unsigned int N = 27;

typedef struct node
{
    bool kelime_sonu;
    struct node *next[N];
}
node;

bool yükle(node *işlenen, char *kalan);
void sil(node *işlenen);
void sıfırla(char *kelime); //Sadece kelime için!
void yazdır(char *kelime); // Sadece kelime için!
void düğüm_yap(node *menaç);

node *ağaç;
int kelime_sayısı = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    node *geç = ağaç;
    for (int i = 0; i < LENGTH; i++)
    {
        if (word[i] == '\0')
        {
            return geç->kelime_sonu;
        }
        geç = geç->next[hash(&word[i])];
        if (geç == NULL)
        {
            return false;
        }
    }
    return true;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    if (*word == '\'')
    {
        return 26;
    }
    return toupper(*word) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *sözlük = fopen(dictionary, "r");
    if (sözlük == NULL)
    {
        printf("Sözlük dosyası açılamadı.\n");
        return false;
    }
    char kelime[LENGTH + 1];
    sıfırla(kelime);
    char harf;
    int syc = 0;
    node *yapı = malloc(sizeof(node));
    düğüm_yap(yapı);
    for (int i = 0; fread(&harf, 1, 1, sözlük); i++)
    {
        if (harf == '\n')
        {
            if (yükle(yapı, kelime) == false)
            {
                printf("Hata: yükle'de malloc hafıza ayıramadı.");
                return false;
            }
            sıfırla(kelime);
            kelime_sayısı++;
            syc = 0;
        }
        else
        {
            kelime[syc] = harf;
            syc++;
        }
    }
    ağaç = yapı;
    fclose(sözlük);
    return true;
}

bool yükle(node *işlenen, char *kalan)
{
    if (*kalan == 0)
    {
        işlenen->kelime_sonu = true;
        return true;
    }
    int diz = hash(kalan);
    kalan++;
    if (işlenen->next[diz] == NULL)
    {
        node *geç = malloc(sizeof(node));
        if (geç == NULL)
        {
            return false;
        }
        düğüm_yap(geç);
        işlenen->next[diz] = geç;
        return yükle(geç, kalan);
    }
    else
    {
        return yükle(işlenen->next[diz], kalan);
    }
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return kelime_sayısı;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    sil(ağaç);
    return true;
}

void sil(node *işlenen)
{
    if (işlenen == NULL)
    {
        return;
    }
    for (int i = 0; i < N; i++)
    {
        sil(işlenen->next[i]);
    }
    free(işlenen);
}

void sıfırla(char *kelime) //Sadece kelime için!
{
    for (int i = 0; i < LENGTH + 1; i++)
    {
        kelime[i] = 0;
    }
}

void düğüm_yap(node *menaç)
{
    for (int i = 0; i < N; i++)
    {
        menaç->next[i] = NULL;
    }
    menaç->kelime_sonu = false;
}