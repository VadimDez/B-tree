#include "btree.h"

btree::btree()
{
    root = NULL ;
    file.open("file",ios::binary | ios::in | ios::out);
}
void btree::inserimento( int val )
{
    int i ;
    btnode *c, *n ;
    int flag ;
    flag = insValore( val, root, &i, &c ); // se inserisce valore ritorna 1;
    if ( flag )
    {
        n = new btnode ;
        n->count = 1 ;
        n->elements[1] = i ;
        n->child[0] = root ;
        n->child[1] = c ;
        root = n ;

    }
    file.seekp(0,ios_base::end);
    file.write((const char*)&n,sizeof(n));
}
int btree::insValore( int val, btnode *n, int *p, btnode **c )
{
    int k ;
    if ( n == NULL )
    {
        *p = val ;
        *c = NULL ;
        return 1;
    }
    else
    {
        if( cercaNodo( val, n, &k ) ) // controllo se valore esisteva di gia
        {
            cout << "\nValore gia' esiste." << endl ;
        }
        if ( insValore( val, n->child[k], p, c ) ) // controllo se posso inserire valore nel nodo
        {
            if ( n->count < MAX )
            { //
                riempiNodo( *p, *c, n, k );
                return 0 ;
            }
            else
            { // foglia
                split( *p, *c, n, k, p, c ) ;
                return 1 ;
            }
        }
        return 0 ;
    }
}
btnode * btree::cerca( int val, btnode *root, int *pos )
{
    if ( root == NULL )
        return NULL ;
    else
    {
        if ( cercaNodo( val, root, pos ) )
            return root ;
        else
            return cerca ( val, root->child[*pos], pos ) ;
    }
}
int btree::cercaNodo( int val, btnode *n, int *pos )
{
    if ( val < n->elements[1] )
    { // primo puntatore a sinista
        *pos = 0 ;
        return 0 ;
    }
    else
    {
        *pos = n->count ;
        while( ( val < n->elements[*pos] ) && *pos > 1 )
        {
            (*pos)--;
        }
        if ( val == n->elements[*pos] )
        {
            return 1 ;
        }
        else
        {
            return 0 ;
        }
    }
}
void btree::riempiNodo( int val, btnode *c, btnode *n, int k )
{
    int i ;
    for ( i = n->count ; i > k ; i-- )
    {
        n->elements[i + 1] = n->elements[i] ;
        n->child[i + 1] = n->child[i] ;
    }
    n->elements[k + 1] = val ;
    n->child[k + 1] = c ;
    n->count++ ;
}
void btree::split(int val, btnode *c, btnode *n, int k, int *y, btnode **newnode )
{
    int i, mid ;

    if ( k <= MIN )
        mid = MIN ;
    else
        mid = MIN + 1 ;

    *newnode = new btnode ;

    for ( i = mid + 1 ; i <= MAX ; i++ )
    {
        ( *newnode )->elements[i - mid] = n->elements[i] ;
        ( *newnode )->child[i - mid] = n->child[i] ;
    }

    ( *newnode )->count = MAX - mid ;
    n->count = mid ;

    if ( k <= MIN )
        riempiNodo ( val, c, n, k ) ;
    else
        riempiNodo ( val, c, *newnode, k - mid ) ;

    *y = n->elements[n->count] ;
    ( *newnode )->child[0] = n->child[n->count] ;
    n->count-- ;
}
void btree::del( int val )
{
    btnode * temp ;

    if( ! delhelp ( val, root ) )
    {
        cout << endl << "Valore " << val << " non trovato." << endl;
    }
    else
    {
        if ( root->count == 0 )
        {
            temp = root ;
            root = root->child[0] ;
            delete temp ;
        }
    }
}
int btree::delhelp( int val, btnode *root )
{
    int i ;
    int flag ;

    if ( root == NULL )
        return 0 ;
    else
    {
        flag = cercaNodo( val, root, &i ) ;
        if ( flag )
        {
            if ( root->child[i - 1] )
            {
                copysucc ( root, i ) ;
                flag = delhelp ( root->elements[i], root->child[i] ) ;
                if ( !flag )
                    cout << endl << "Valore " << val << " non trovato." ;
            }
            else
                clear ( root, i ) ;
        }
        else
            flag = delhelp ( val, root->child[i] ) ;
        if ( root->child[i] != NULL )
        {
            if ( root->child[i]->count < MIN )
                restore ( root, i ) ;
        }
        return flag ;
    }
}
void btree::clear( btnode *root, int k )
{
    int i ;
    for ( i = k + 1 ; i <= root->count ; i++ )
    {
        root->elements[i - 1] = root->elements[i] ;
        root->child[i - 1] = root->child[i] ;
    }
    root->count-- ;
}
void btree::copysucc ( btnode *root, int i )
{
    btnode *temp = root->child[i] ;

    while ( temp->child[0] )
        temp = temp->child[0] ;

    root->elements[i] = temp->elements[1] ;
}
void btree::restore( btnode *root, int i )
{
    if ( i == 0 )
    {
        if ( root->child [1]->count > MIN )
            sinistra ( 1 ) ;
        else
            merge ( 1 ) ;
    }
    else
    {
        if ( i == root->count )
        {
            if ( root->child[i - 1]->count > MIN )
                destra ( i ) ;
            else
                merge ( i ) ;
        }
        else
        {
            if ( root->child[i - 1]->count > MIN )
                destra ( i ) ;
            else
            {
                if ( root->child[i + 1]->count > MIN )
                    sinistra ( i + 1 ) ;
                else
                    merge ( i ) ;
            }
        }
    }
}
void btree::destra ( int k ) // shift a destra
{
    int i ;
    btnode *temp ;

    temp = root->child[k] ;

    for ( i = temp->count ; i > 0 ; i-- )
    {
        temp->elements[i + 1] = temp->elements[i] ;
        temp->child[i + 1] = temp->child[i] ;
    }
    temp->child[1] = temp->child[0] ;
    temp->count++ ;
    temp->elements[1] = root->elements[k] ;
    temp = root->child[k - 1] ;
    root->elements[k] = temp->elements[temp->count] ;
    root->child[k]->child [0] = temp->child[temp->count] ;
    temp->count-- ;
}
void btree::sinistra ( int k ) // shift a sinistra
{
    btnode *temp ;

    temp = root->child[k - 1] ;
    temp->count++ ;
    temp->elements[temp->count] = root->elements[k] ;
    temp->child[temp->count] = root->child[k]->child[0] ;
    temp = root->child[k] ;
    root->elements[k] = temp->elements[1] ;
    temp->child[0] = temp->child[1] ;
    temp->count-- ;
    for ( int i = 1 ; i <= temp->count ; i++ )
    {
        temp->elements[i] = temp->elements[i + 1] ;
        temp->child[i] = temp->child[i + 1] ;
    }
}
void btree::merge( int k )
{
    btnode *temp1, *temp2 ;
    temp1 = root->child[k] ;
    temp2 = root->child[k - 1] ;
    temp2->count++ ;
    temp2->elements[temp2->count] = root->elements[k];
    temp2->child[temp2->count] = root->child[0];
    for ( int i = 1 ; i <= temp1->count ; i++ )
    {
        temp2->count++ ;
        temp2->elements[temp2->count] = temp1->elements[i];
        temp2->child[temp2->count] = temp1->child[i];
    }
    for ( int i = k ; i < root->count ; i++ )
    {
        root->elements[i] = root->elements[i + 1] ;
        root->child[i] = root->child[i + 1] ;
    }
    root->count-- ;
    delete temp1 ;
}
void btree::show() // visualizzare elementi
{
    visualizza(root) ;
}
// per visualizzare tutti gli elementi
void btree::visualizza( btnode *root )
{
    if ( root != NULL )
    {
        int i;
        for ( i = 0 ; i < root->count;i++ )
        {
            visualizza( root->child[i] ) ;
            cout << root->elements[i + 1] << endl ;
        }
        visualizza(root->child[i]);
    }
}
// per cancellare tutto albero
void btree::deltree ( btnode *root )
{
    if ( root != NULL )
    {
        int i;
        for( i = 0 ; i < root->count ; i++ )
        {
            deltree( root->child[i] );
            delete( root->child[i] );
        }
        deltree( root->child[i] );
        delete( root->child[i] );
    }
}

btree::~btree( )
{
    deltree(root);
    file.close();
}

bool btree::cercaElemento( btnode *root , int val )
{
    if ( root != NULL )
    {
        int i;
        for ( i = 0 ; i < root->count;i++ )
        {
            visualizza( root->child[i] ) ;
            if(root->elements[i + 1] == val)
            {
                cout << "Valore: " << root->elements[i + 1] << " esiste" << endl;
                return true;
            }
        }
        visualizza(root->child[i]);
    }
}
