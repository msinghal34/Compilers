#include <iostream>
#include <list>
using namespace std;
int main()
{
    list<int> x;
    x.push_back(1);
    x.push_back(2);
    x.push_back(3);
    x.push_back(5);
    list<int> y;
    y.push_back(-111);
    y.push_back(-21);
    y.push_back(21);
    y.push_back(31);
    x.insert(x.end(), y.begin(), y.end());
    for (list<int>::iterator it = x.begin(); it != x.end(); ++it)
    {
        cout << *it << endl;
    }
}