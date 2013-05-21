#ifndef COLLECTIONS_H
#define COLLECTIONS_H

#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

namespace Collections {


template<class T>
class DList
{
public:
    DList() { }
    DList(int d1, int d2, int d3)
    {
        this->setDimensions(d1,d2,d3);
    }

    virtual void set(T t, int i1, int i2=0, int i3=0) = 0;
    virtual T get(int i1, int i2=0, int i3=0) = 0;
    virtual void printList() = 0;
    int getDimensions() { return dimensions; }
    int getDimension1() { return d1; }
    int getDimension2() { return d2; }
    int getDimension3() { return d3; }

protected:
    int dimensions; //number of dimensions
    int d1, d2, d3; //dimension-sizes

    void setDimensions(int d1, int d2, int d3)
    {
        this->d1=d1;
        this->d2=d2;
        this->d3=d3;

        /*
        //ATTENTION: check if lower than 0 or d2=0 and d3>0!!!!
        if (d3!=0 && d2!=0)
            dimensions = 3;
        else if (d3==0 && d2!=0)
            dimensions = 2;
        else if (d3==0 && d2==0)
            dimensions = 1;
            */
    }
};


/**
 * @brief Defines the initialization value. INULL= NULL, INEW= new T(), IDEFAULT= T()
 *
 */
//enum InitWith {INULL=0, INEW=1, IDEFAULT=2};

template<class T>
struct is_pointer { static const bool value = false; };

template<class T>
struct is_pointer<T*> { static const bool value = true; };


template<class T>
class ArrayDList : public DList<T>
{
private:
    T*** arr;
    //InitWith iw;

    bool indexCorrect(int i1, int i2, int i3)
    {
        if (i1>=0 && i1<this->d1 &&
                i2>=0 && i2<this->d2 &&
                i3>=0 && i3<this->d3)
            return true;
        else
            return false;
    }

public:
    ArrayDList(int d1, int d2=1, int d3=1)
    {
        this->setDimensions(d1,d2,d3);
        //iw=init;

        arr = new T**[d1];
        //cout << "d1: " << d1 << " d2: " << d2 << " d3: " << d3 << endl;
        for (int i=0; i<d1; i++)
        {
            //cout << "at level 1, run " << i << endl;

            arr[i] = new T*[d2];
            for (int j=0; j<d2; j++)
            {
                //cout << "at level 2, run " << j << endl;

                arr[i][j] = new T[d3];
                for (int k=0; k<d3; k++)
                {
                    //cout << "at level 3, run " << k << endl;

                    if (is_pointer<T>::value)
                    {
                        //cout << "  is pointer, make new" << endl;
                        arr[i][j][k] = *(new T());
                    }
                    else arr[i][j][k]=T();

                }
            }
        }

    }

    ~ArrayDList()
    {
        for (int i=0; i<this->d1; i++)
        {
            for (int j=0; j<this->d2; j++)
            {
                delete arr[i][j];
            }
            delete arr[i];
        }
        delete [] arr;

    }


    void set(T t, int i1, int i2=0, int i3=0)
    {
        //std::cout << "insert at x/y/z: " << i1 << "/" << i2 << "/" << i3 << std::endl;
        if (indexCorrect(i1,i2,i3))
        {
            arr[i1][i2][i3] = t;
        }
    }



    T get(int i1, int i2=0, int i3=0)
    {
        if (indexCorrect(i1,i2,i3))
        {
            return arr[i1][i2][i3];
        }
        else return T();//*(new T());

    }

    void printList()
    {
        cout << "start print" << endl;
        for (int i=0; i<this->d1; i++)
        {

                for (int j=0; j<this->d2; j++)
                {

                        for (int k=0; k<this->d3; k++)
                        {
                            cout << "arr[" << i << "]:" << "[" << j << "]" << "[" << k << "]: " << arr[i][j][k] << endl;
                        }

                }


        }

    }

};

}




#endif // COLLECTIONS_H
