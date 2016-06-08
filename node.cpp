#include "node.h"

using namespace std;

int Node::num_free_first_neighbors() //количество первых свободных соседей
{
    int idx = 0;
    for (auto &i: first_neighbors)
    {
        if (i->its_node_state == Surface::free_place)
            ++idx;
    }
    return idx;
}

void Node::print_first_neighbors()
{
    for (size_t i = 0; i < first_neighbors.size(); i++)
        std::cout << first_neighbors[i] << ' ';
    std::cout << std::endl;
}



void Node::velocity_calculation() //Эта функция для расчета скоростей для каждого узла по отдельности, то есть по 5-му алгоритму
{
//    cout << "-------------------------------------velocity calc begin-----------------------------------------------" << endl;
    unsigned int num_first_neighbors = 0;//количество занятых мест среди первых соседей
    if (get_node_state() != Surface::do_not_use && \
            get_node_state() != Surface::free_place)
    {
            for (size_t f = 0; f < 3; f++)
            {
                if (get_first_neighbors()[f]->get_node_state() != Surface::free_place &&
                    get_first_neighbors()[f]->get_node_state() != Surface::do_not_use)
                {
                    num_first_neighbors++;
                }
            }
//            if (num_first_neighbors == 3)
//                cout << "num_neigh = " << num_first_neighbors << endl;
    }
//        }
//    }
    //если текущая ячейка не свободна, то адсорбции нет
    if (get_node_state() != Surface::free_place)
        v_adsorpion = 0;
    else
        v_adsorpion = k_adsorpion;

//    cout << "V_ads = " << v_adsorpion << endl;
//    v_desorpion = k_desorpion * num_particles; // = 0
//    cout << "V_des = " << v_desorpion << endl;
//    v_migration = k_migration * num_first_neighbors;
//    v_migration = k_migration * exp(-1 * (E_0 + num_first_neighbors * delta_eps_1)/RT);
    unsigned num_first_neighbors_right = 0;
    unsigned num_first_neighbors_left = 0;
    unsigned num_first_neighbors_up = 0;
//    cout << "for [" << x_index << "][" << y_index << "] state = " << its_node_state << ":" << endl;
    //если все соседи заняты или текущая ячейка свободна или запретное место, то миграция не идет
    if (num_first_neighbors == 3 || get_node_state() == Surface::free_place || get_node_state() == Surface::do_not_use)
    {
        v_migration_all = 0;

//        cout <<"state = " << get_node_state() << " num_first = " << num_first_neighbors
//            << " v_ad = " << v_adsorpion << " v_all = 0" << endl;
    }
    else
    {
        //Считаем количество занятых соседей
        for (size_t f = 0; f < 3; f++)//это не оптимально, можно проще.
        {
            if (get_first_neighbors()[0]->get_first_neighbors()[f]->get_node_state() != Surface::free_place &&
                get_first_neighbors()[0]->get_first_neighbors()[f]->get_node_state() != Surface::do_not_use)//right
            {
                if (get_first_neighbors()[0]->get_first_neighbors()[f]->get_x_index() != get_x_index() &&
                    get_first_neighbors()[0]->get_first_neighbors()[f]->get_y_index() != get_y_index())//исключаем текущую ноду
                {
                    num_first_neighbors_right++;
                }
            }
            if (get_first_neighbors()[1]->get_first_neighbors()[f]->get_node_state() != Surface::free_place &&
                get_first_neighbors()[1]->get_first_neighbors()[f]->get_node_state() != Surface::do_not_use)//left
            {
                if (get_first_neighbors()[1]->get_first_neighbors()[f]->get_x_index() != get_x_index() &&
                    get_first_neighbors()[1]->get_first_neighbors()[f]->get_y_index() != get_y_index())
                {
                    num_first_neighbors_left++;
                }
            }
            if (get_first_neighbors()[2]->get_first_neighbors()[f]->get_node_state() != Surface::free_place &&
                get_first_neighbors()[2]->get_first_neighbors()[f]->get_node_state() != Surface::do_not_use)//up
            {
                if (get_first_neighbors()[2]->get_first_neighbors()[f]->get_x_index() != get_x_index() &&
                    get_first_neighbors()[2]->get_first_neighbors()[f]->get_y_index() != get_y_index())
                {
                    num_first_neighbors_up++;
                }
            }
//                v_migration_all = k_migration;
        }
//        cout << "current node [" << x_index << "][" << y_index << "] = " << its_node_state << endl;
//        cout << "num_first_neighbors_right = " << num_first_neighbors_right << endl;
//        cout << "num_first_neighbors_left = " << num_first_neighbors_left << endl;
//        cout << "num_first_neighbors_up = " << num_first_neighbors_up << endl;
//        for (size_t i = 0; i < 3; i++)
//        {
//            cout << "[" << get_first_neighbors()[i]->get_x_index() << "][" << get_first_neighbors()[i]->get_y_index()
//                 << "] = " << get_first_neighbors()[i]->get_node_state() << " :" <<endl;
//            for (size_t j = 0; j < 3; j++)
//            {
//                cout << "\t[" << get_first_neighbors()[i]->get_first_neighbors()[j]->get_x_index() << "]["
//                     << get_first_neighbors()[i]->get_first_neighbors()[j]->get_y_index() << "] = "
//                     << get_first_neighbors()[i]->get_first_neighbors()[j]->get_node_state() << endl;
//            }
//        }
        //Дальше идет вообще полный пиздец
        if (num_first_neighbors == 0)
        {
            assert(num_first_neighbors_right <= 2);
            v_migration_right = this->V_0_s[num_first_neighbors_right];

            assert(num_first_neighbors_left <= 2);
            v_migration_left = this->V_0_s[num_first_neighbors_left];

            assert(num_first_neighbors_up <= 2);
            v_migration_up = this->V_0_s[num_first_neighbors_up];

        }
        else if (num_first_neighbors == 1)
        {
            if (get_first_neighbors()[0]->get_node_state() != Surface::free_place)
            {
                v_migration_right = 0;
            }
            else
            {
                assert(num_first_neighbors_right <= 2);
                v_migration_right = this->V_1_s[num_first_neighbors_right];
            }


            if (get_first_neighbors()[1]->get_node_state() != Surface::free_place)
            {
                v_migration_left = 0;
            }
            else
            {
                assert(num_first_neighbors_left <= 2);
                v_migration_left = this->V_1_s[num_first_neighbors_left];
            }

            if (get_first_neighbors()[2]->get_node_state() != Surface::free_place)
            {
                v_migration_up = 0;
            }
            else
            {
                assert(num_first_neighbors_up <= 2);
                v_migration_up = this->V_1_s[num_first_neighbors_up];
            }
        }
        else if (num_first_neighbors == 2)
        {
            if (get_first_neighbors()[0]->get_node_state() != Surface::free_place)
            {
                v_migration_right = 0;
            }
            else
            {
                assert(num_first_neighbors_right <= 2);
                v_migration_right = this->V_2_s[num_first_neighbors_right];
            }


            if (get_first_neighbors()[1]->get_node_state() != Surface::free_place)
            {
                v_migration_left = 0;
            }
            else
            {
                assert(num_first_neighbors_left <= 2);
                v_migration_left = this->V_2_s[num_first_neighbors_left];
            }

            if (get_first_neighbors()[2]->get_node_state() != Surface::free_place)
            {
                v_migration_up = 0;
            }
            else
            {
                assert(num_first_neighbors_up <= 2);
                v_migration_up = this->V_2_s[num_first_neighbors_up];
            }
        }
        v_migration_all = v_migration_up + v_migration_left + v_migration_right;
//        cout << "V_up = " << v_migration_up << " V_left = " << v_migration_left << " V_right = " << v_migration_right << endl;
    }
    v_summ = v_adsorpion + v_desorpion + v_migration_all;
//    cout << endl << "\tv_summ = " << v_summ << endl;
//    cout << "-------------------------------------velocity calc end-----------------------------------------------" << endl;
}

void Node::migration()
{
//    cout << "Migration" << endl;
//    velocity_calculation();
//    cout << "V_mig_up = " << v_migration_up << " V_left = " << v_migration_left << " V_right = " << v_migration_right << endl;
    unsigned int num_first_neighbors = 0;
//    cout << "current node [" << x_index << "][" << y_index << "] state = " << get_node_state() << " have :" << endl;
    for (size_t f = 0; f < 3; f++)
    {
        /*cout << "\t[" << get_first_neighbors()[f]->get_x_index() << "][" << get_first_neighbors()[f]->get_y_index() << "] = " <<
                get_first_neighbors()[f]->get_node_state() << endl;*/
        if (get_first_neighbors()[f]->get_node_state() == Surface::free_place)
        {
            num_first_neighbors++;
        }
    }
    if (get_node_state() == Surface::free_place || get_node_state() == Surface::do_not_use || num_first_neighbors == 0)
    {
//        cout << "the migration process is not =((" << endl;
//        cout << "num_neigh = " << num_first_neighbors << " state = " << get_node_state() << endl;
        return;
    }
//    cout << "num_first = " << num_first_neighbors << endl;
    double v_current = generator_null_one() * v_migration_all;
    if (v_current <= v_migration_right) //и не равно 0
    {
//        cout << "mig right : " << endl;
//        cout << "[" << x_index << "][" << y_index << "] = " << its_node_state << " --> ["
//             << get_first_neighbors()[0]->get_x_index() << "][" << get_first_neighbors()[0]->get_y_index()
//             << "] = " << get_first_neighbors()[0]->get_node_state() << endl;
        get_first_neighbors()[0]->set_node_state(get_node_state());
        set_node_state(Surface::free_place);
//        cout << "[" << x_index << "][" << y_index << "] = " << its_node_state << " --> ["
//             << get_first_neighbors()[0]->get_x_index() << "][" << get_first_neighbors()[0]->get_y_index()
//             << "] = " << get_first_neighbors()[0]->get_node_state() << endl;
    }
    else if (v_current <= v_migration_right + v_migration_left)
    {
//        cout << "mig left : " << endl;
//        cout << "[" << x_index << "][" << y_index << "] = " << its_node_state << " --> ["
//             << get_first_neighbors()[1]->get_x_index() << "][" << get_first_neighbors()[1]->get_y_index()
//             << "] = " << get_first_neighbors()[1]->get_node_state() << endl;
        get_first_neighbors()[1]->set_node_state(get_node_state());
        set_node_state(Surface::free_place);
//        cout << "[" << x_index << "][" << y_index << "] = " << its_node_state << " --> ["
//             << get_first_neighbors()[1]->get_x_index() << "][" << get_first_neighbors()[1]->get_y_index()
//             << "] = " << get_first_neighbors()[1]->get_node_state() << endl;
    }
    else if (v_current <= v_migration_right + v_migration_left + v_migration_up)
    {
//        cout << "mig up : " << endl;
//        cout << "[" << x_index << "][" << y_index << "] = " << its_node_state << " --> ["
//             << get_first_neighbors()[2]->get_x_index() << "][" << get_first_neighbors()[2]->get_y_index()
//             << "] = " << get_first_neighbors()[2]->get_node_state() << endl;

        get_first_neighbors()[2]->set_node_state(get_node_state());
        set_node_state(Surface::free_place);
//        cout << "[" << x_index << "][" << y_index << "] = " << its_node_state << " --> ["
//             << get_first_neighbors()[2]->get_x_index() << "][" << get_first_neighbors()[2]->get_y_index()
//             << "] = " << get_first_neighbors()[2]->get_node_state() << endl;
    }
    else {
        cout << "----------------------------------------no migaration" << endl;
    }
}

void Node::adsorption(Surface::host_state new_state)
{
    if (its_node_state == Surface::free_place)
    {
//        cout << "Adsorption for [" << x_index << "][" << y_index << "] = " << its_node_state << " --> ";
        set_node_state(new_state);
//        cout  << "[" << x_index << "][" << y_index << "] = " << its_node_state << endl;
    }
    else
    {
        cout << "----------------------------------------no adsorption" << endl;
    }
}



void Node::set_delta_t_j()//расчитывает время пребывания в ячейке
{
    //    velocity_calculation();
    if (v_summ == 0)
    {
//            cout << "[" << x_index << "][" << y_index << "] state = " << its_node_state << " delta_t_j = " << delta_t_j
//                 <<" v = " << v_summ << endl;
        //        delta_t_j = 0; //тут косяк. Делим на ноль. V-summ не правильно рассчитывается
//        cout << "div null for index = " << common_index << " state = " << its_node_state << " delta_t_j = " << delta_t_j << endl;
        return;
    }
    else
    {
        delta_t_j = -1 * log(generator_null_one())/v_summ;
    }
//    cout << "[" << x_index << "][" << y_index << "] state = " << its_node_state << " delta_t_j = " << delta_t_j << endl;
}
