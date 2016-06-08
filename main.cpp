#include <iostream>
#include "node.h"
#include <iomanip>
using namespace std;

int main()
{
    Surface s(6, 6);
    s.print_surface();

//    for (size_t i = 0; i < 20; i++)
//    {
////        cout.precision(10);
////        cout << std::fixed << gsch_ab(0, s.get_all_nodes()) << endl;
//    }
//    cout << "Инициализация: " << endl;
//    s.initial_distribution(8,1);

//    cout << "Адсорбция: "<< endl;
//    s.adsorption(2);
//    s.print_surface();

//    cout << "Десорбция: "<< endl;
//    s.desorption();
//    s.print_surface();

//    for(size_t i = 0; i < 50; i++)
//    {
//        cout << "Миграция:  " << i << endl;

//        s.migration();
//        s.print_surface();
//        s.selection_process_for_surface();
//        s.selection_process_for_node();
//    }


    return 0;
}
