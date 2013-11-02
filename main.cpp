#include <QtCore/QCoreApplication>
#include <btree.h>

bool menu(btree &btr);
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    btree b;
    bool exit = false;
    do
    {
        exit = menu(b);
    }while(!exit);
    return 0;
    return a.exec();
}

bool menu(btree &btr)
{
    int n;
    do
    {
        cout << "==============================" << endl;
        cout << "1 - per inserire valore" << endl;
        cout << "2 - per visualizzare BTree" << endl;
        cout << "3 - per cancellare elemento" << endl;
        cout << "4 - per cercare elemento" << endl;
        cout << "0 - per uscire" << endl;
        cout << "==============================" << endl;
        cin >> n;
    }while(n < 0 || n > 4);

    switch(n)
    {
    case 1:
        cout << "Inserisci valore da inserire in BTree:";
        cin >> n;
        btr.inserimento(n);
        return false;
    case 2:
        cout << "B-tree of order 5:" << endl ;
        btr.show();
        cout << endl;
        return false;
    case 3:
        cout << "Inserisci valore da cancellare:";
        cin >> n;
        btr.del(n);
        return false;
    case 4:
        cout << "Inserisci valore da cercare:";
        cin >> n;
        if(btr.cercaElemento(btr.root,n) == false)
        {
            cout << "Elemento " << n << " non trovato." << endl;
        }
        return false;
    case 0:
        return true;
    }
}
