#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <stdlib.h>
#include <malloc.h>
#include <windows.h>
#include <fstream>
using namespace std;

const int MAX = 4 ;
const int MIN = 2 ;

struct btnode
{
    int count ;
    int elements[MAX + 1] ; // vettore per valori
    btnode *child[MAX + 1] ;
};

class btree
{
public :
    fstream file;
    btnode *root;
    btree();
    void inserimento(int val);
    int insValore(int val, btnode *n, int *p, btnode **c );
    static btnode * cerca ( int val, btnode *root, int *pos );
    static int cercaNodo ( int val, btnode *n, int *pos );
    void riempiNodo ( int val, btnode *c, btnode *n, int k );
    void split( int val, btnode *c, btnode *n, int k, int *y, btnode **newnode);
    void del(int val);
    int delhelp( int val, btnode *root);
    void clear( btnode *root, int k );
    void copysucc( btnode *root, int i);
    void restore( btnode *root, int i );
    void destra( int k );
    void sinistra( int k );
    void merge( int k );
    void show();
    static void visualizza( btnode *root );
    static void deltree( btnode *root );
    bool cercaElemento( btnode *root , int val );
    ~btree();
} ;

#endif // BTREE_H
