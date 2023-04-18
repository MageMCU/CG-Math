#include <iostream>
#include <vector>
#include <string>

#include "../Mathematics/Vector2.h"

using namespace std;
using namespace gte;

int main()
{
    Vector2<float>v{0.5f, 0.25f};
    v[0] = 1.7f;
    cout << v[0] << endl;

}