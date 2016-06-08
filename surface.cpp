//#include "surface.h"
#include "node.h"
using namespace std;
Surface::Surface(): number_of_nodes_in_x(22), number_of_nodes_in_y(31)
{
    set_all_free_nodes();
    set_all_nodes();
    init_surface();
    set_all_first_neighbors();
    set_all_second_neighbors();
    set_all_delta_t_j();
}

Surface::Surface(unsigned int x, unsigned int y): number_of_nodes_in_x(x/3*3), number_of_nodes_in_y(y/3*3)
{
    set_all_free_nodes();
    set_all_nodes();
    init_surface();
    set_all_first_neighbors();
    set_all_second_neighbors();
    set_all_delta_t_j();
}

Surface::~Surface()
{
    for (size_t i = 0 ; i < number_of_nodes_in_y; i++)
    {
        for (size_t j = 0; j < number_of_nodes_in_x; j++)
        {
            delete its_surface[i][j];
        }
    }
}

void Surface::set_number_of_nodes_in_x(int x)
{
    number_of_nodes_in_x = x;
    set_all_free_nodes();
}

void Surface::set_number_of_nodes_in_y(int y)
{
    number_of_nodes_in_y = y;
    set_all_free_nodes();
}

void Surface::set_all_free_nodes()
{
    all_free_nodes = (number_of_nodes_in_x * number_of_nodes_in_y);
}

void Surface::set_all_nodes()
{
    all_nodes = number_of_nodes_in_x * number_of_nodes_in_y;
}

void Surface::init_surface()
{
    its_surface.resize(number_of_nodes_in_y);
    for (size_t i = 0 ; i < number_of_nodes_in_y; i++)
    {
        its_surface[i].resize(number_of_nodes_in_x);
        for (size_t j = 0; j < number_of_nodes_in_x; j++)
        {
//            Node *n = new Node();
//            std::vector<Node*>& vn = its_surface[i];
//            vn.reserve(number_of_nodes_in_x);
//            std::cout << "cap i = " << its_surface.capacity()
//                      << "cap j = " << vn.capacity() << std::endl;
//            vn[j] = n;
            its_surface[i][j] = new Node();
            its_surface[i][j]->set_x_index(j);//возможно нужно поменять местами i и j
            its_surface[i][j]->set_y_index(i);
            its_surface[i][j]->set_common_index(its_surface[i][j]->get_by_xy(i, j, number_of_nodes_in_x));
            its_surface[i][j]->set_node_state(free_place);//Все блять зануляем
        }
    }
    unsigned int cnt = 0;
    for (size_t i = 0; i < number_of_nodes_in_y; i++)
    {
        if (cnt < 3)
        {
            for (size_t j = cnt; j < number_of_nodes_in_x; j+=3)
            {
                its_surface[i][j]->set_node_state(do_not_use); //Обозначаем центры шестиугольников, которые мы исключаем из рассчета.
                all_free_nodes--;//Считаем количество свободных мест
            }
            ++cnt;
        }
        if (cnt == 3)
        {
            cnt = 0;
        }
    }
}

void Surface::print_surface()
{
    for (size_t i = 0; i < number_of_nodes_in_y; i++)
    {
        for (size_t j = 0; j < number_of_nodes_in_x; j++)
            std::cout << its_surface[i][j]->get_node_state() << " ";
        std::cout << std::endl;
    }
}
unsigned int Surface::get_boundary_conditions (int i, int size)
{
    if (i < 0)
        return size + i - ((size) % 3);
    else if (i >= size)
        return i - size + ((size) % 3);
    else
        return i;
    return 0;
}

void Surface::set_first_neighbors (Node * node, int array_i, int array_j) //Устанавливаем первых соседей для node[array_i][array_j]
{
    /*cout << "Первые соседи для узла [" << array_i << "][" << array_j << "] = " \
         << its_surface[array_i][array_j]->get_node_state() << " :" << endl;*/
    //Можно сделать проверку на то, что если state == do_no_use, то все соседи устанавливаем в ноль.

    if (its_surface[get_boundary_conditions(array_i - 1, number_of_nodes_in_y)][array_j]->get_node_state() != do_not_use)
    {

        node->set_first_neighbors(its_surface[array_i][get_boundary_conditions(array_j + 1, number_of_nodes_in_x)]);//right
        node->set_first_neighbors(its_surface[get_boundary_conditions(array_i - 1, number_of_nodes_in_y)][array_j]);//left

        node->set_first_neighbors(its_surface[get_boundary_conditions(array_i + 1, number_of_nodes_in_y)] \
                [get_boundary_conditions(array_j - 1, number_of_nodes_in_x)]);//up

        /*cout << "[" << array_i << "][" << get_boundary_conditions(array_j + 1, number_of_nodes_in_x) << "] = " \
             << its_surface[array_i][get_boundary_conditions(array_j + 1, number_of_nodes_in_x)]->get_node_state()  << endl;
        cout << "[" << get_boundary_conditions(array_i - 1, number_of_nodes_in_y) << "][" << array_j << "] = " \
             << its_surface[get_boundary_conditions(array_i - 1, number_of_nodes_in_y)][array_j]->get_node_state()  << endl;
        cout << "[" << get_boundary_conditions(array_i + 1, number_of_nodes_in_y) << "]["
             << get_boundary_conditions(array_j - 1, number_of_nodes_in_x) << "] = " \
             << its_surface[get_boundary_conditions(array_i + 1, number_of_nodes_in_y)] \
                [get_boundary_conditions(array_j - 1, number_of_nodes_in_x)]->get_node_state()  << endl;*/


    }
    else
    {

        node->set_first_neighbors(its_surface[get_boundary_conditions(array_i + 1, number_of_nodes_in_y)][array_j]);//right
        node->set_first_neighbors(its_surface[array_i][get_boundary_conditions(array_j - 1, number_of_nodes_in_x)]);//left
        node->set_first_neighbors(its_surface[get_boundary_conditions(array_i - 1, number_of_nodes_in_y)] \
                [get_boundary_conditions(array_j + 1, number_of_nodes_in_x)]);//up

        /*cout << "[" << get_boundary_conditions(array_i + 1, number_of_nodes_in_y) << "][" << array_j << "] = " \
             << its_surface[get_boundary_conditions(array_i + 1, number_of_nodes_in_y)][array_j]->get_node_state() << endl;
        cout << "[" << array_i << "][" << get_boundary_conditions(array_j - 1, number_of_nodes_in_x) << "] = " \
             << its_surface[array_i][get_boundary_conditions(array_j - 1, number_of_nodes_in_x)]->get_node_state() << endl;
        cout << "[" << get_boundary_conditions(array_i - 1, number_of_nodes_in_y) << "]["
             << get_boundary_conditions(array_j + 1, number_of_nodes_in_x) << "] = " \
             << its_surface[get_boundary_conditions(array_i - 1, number_of_nodes_in_y)] \
                [get_boundary_conditions(array_j - 1, number_of_nodes_in_x)]->get_node_state() << endl;*/

    }
}
void Surface::set_second_neighbors (Node * node, int array_i, int array_j)
{
    /* cout << "Вторые соседи для узла [" << array_i << "][" << array_j << "] = " \
         << its_surface[array_i][array_j]->get_node_state() << " :" << endl;
*/
    node->set_second_neighbors(its_surface[get_boundary_conditions(array_i - 1, number_of_nodes_in_y)] \
            [get_boundary_conditions(array_j - 1, number_of_nodes_in_y)]);

    node->set_second_neighbors(its_surface[get_boundary_conditions(array_i - 2, number_of_nodes_in_y)] \
            [get_boundary_conditions(array_j + 1, number_of_nodes_in_y)]);

    node->set_second_neighbors(its_surface[get_boundary_conditions(array_i - 1, number_of_nodes_in_y)] \
            [get_boundary_conditions(array_j + 2, number_of_nodes_in_y)]);

    node->set_second_neighbors(its_surface[get_boundary_conditions(array_i + 1, number_of_nodes_in_y)] \
            [get_boundary_conditions(array_j - 2, number_of_nodes_in_y)]);

    node->set_second_neighbors(its_surface[get_boundary_conditions(array_i + 1, number_of_nodes_in_y)] \
            [get_boundary_conditions(array_j + 1, number_of_nodes_in_y)]);

    node->set_second_neighbors(its_surface[get_boundary_conditions(array_i + 2, number_of_nodes_in_y)] \
            [get_boundary_conditions(array_j - 1, number_of_nodes_in_y)]);

    /*cout << "[" << get_boundary_conditions(array_i - 1, number_of_nodes_in_y) << "][" \
         << get_boundary_conditions(array_j - 1, number_of_nodes_in_y) << "] = " \
         << its_surface[get_boundary_conditions(array_i - 1, number_of_nodes_in_y)]
            [get_boundary_conditions(array_j - 1, number_of_nodes_in_y)]->get_node_state() << endl;

    cout << "[" << get_boundary_conditions(array_i - 2, number_of_nodes_in_y) << "][" \
         << get_boundary_conditions(array_j + 1, number_of_nodes_in_y) << "] = " \
         << its_surface[get_boundary_conditions(array_i - 2, number_of_nodes_in_y)] \
            [get_boundary_conditions(array_j + 1, number_of_nodes_in_y)]->get_node_state() << endl;

    cout << "[" << get_boundary_conditions(array_i - 1, number_of_nodes_in_y) << "][" \
         << get_boundary_conditions(array_j + 2, number_of_nodes_in_y) << "] = " \
         << its_surface[get_boundary_conditions(array_i - 1, number_of_nodes_in_y)]
            [get_boundary_conditions(array_j + 2, number_of_nodes_in_y)]->get_node_state() << endl;

    cout << "[" << get_boundary_conditions(array_i + 1, number_of_nodes_in_y) << "][" \
         << get_boundary_conditions(array_j - 2, number_of_nodes_in_y) << "] = " \
         << its_surface[get_boundary_conditions(array_i + 1, number_of_nodes_in_y)] \
            [get_boundary_conditions(array_j - 2, number_of_nodes_in_y)]->get_node_state() << endl;

    cout << "[" << get_boundary_conditions(array_i + 1, number_of_nodes_in_y) << "][" \
         << get_boundary_conditions(array_j + 1, number_of_nodes_in_y) << "] = " \
         << its_surface[get_boundary_conditions(array_i + 1, number_of_nodes_in_y)] \
            [get_boundary_conditions(array_j + 1, number_of_nodes_in_y)]->get_node_state() << endl;

    cout << "[" << get_boundary_conditions(array_i + 2, number_of_nodes_in_y) << "][" \
         << get_boundary_conditions(array_j - 1, number_of_nodes_in_y) << "] = " \
         << its_surface[get_boundary_conditions(array_i + 2, number_of_nodes_in_y)] \
            [get_boundary_conditions(array_j - 1, number_of_nodes_in_y)]->get_node_state() << endl;
            */
}

void Surface::set_all_first_neighbors()
{
    for (size_t i = 0 ; i < number_of_nodes_in_y; i++)
    {
        for (size_t j = 0; j < number_of_nodes_in_x; j++)
        {
            set_first_neighbors(its_surface[i][j], i, j);
        }
    }
}

void Surface::set_all_second_neighbors()
{
    for (size_t i = 0 ; i < number_of_nodes_in_y; i++)
    {
        for (size_t j = 0; j < number_of_nodes_in_x; j++)
        {
            set_second_neighbors(its_surface[i][j], i, j);
        }
    }
}

Node *Surface::get_element_in_surface(int i, int j)
{
    return its_surface[i][j];
}

Node *Surface::get_element_by_common_index(unsigned int index)
{
    return (index < number_of_nodes_in_y * number_of_nodes_in_x) ?
                its_surface[index / number_of_nodes_in_x][index % number_of_nodes_in_x] :
                nullptr;
}

void Surface::adsorption(Surface::host_state new_state)
{
//    cout << "Adsorption" << endl;
//    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
//    mt19937 r (seed);
    unsigned int index = generator_null_one() * get_all_nodes();
    while ( /*((get_element_by_common_index(index)->get_node_state() == new_state) || \
                   (get_element_by_common_index(index)->get_node_state() == do_not_use)) && \ */
            (get_element_by_common_index(index)->get_node_state() != free_place))
    {
//        cout << "generate index in adsorption" << endl;
//        index = r() % get_all_nodes();
        index = generator_null_one() * get_all_nodes();
    }
    get_element_by_common_index(index)->set_node_state(new_state);

}
void Surface::desorption() //Процесс десорбции
{
    cout << "Desorption" << endl;
    unsigned int index = generator_null_one() * get_all_nodes();
    while ((get_element_by_common_index(index)->get_node_state() == do_not_use) || \
           (get_element_by_common_index(index)->get_node_state() == free_place))
    {
        index = generator_null_one() * get_all_nodes(); //Выбираем случайный индекс ноды
    }
    get_element_by_common_index(index)->set_node_state(free_place); //нода стала свободна!!! Ужасная частица покинула её

}

//Процесс начального заполнения поверхности, та же адсорбция, только тут несколько процессов адсорбции и сразу
void Surface::initial_distribution(unsigned int amount_of_particles, Surface::host_state new_state)
{
    //    unsigned int index = 0;
    while(amount_of_particles > 0)
    {
        adsorption(new_state);
        amount_of_particles--;
    }

}

void Surface::migration()
{
//    cout << "Migration" << endl;
    //проверка на то, что на поверхности есть хотя бы один элемент, который может мигрировать
    //количество элементов, которые могут мигрировать. Для осуществления процесса миграции это число должно быть больше нуля.
    unsigned int num_element_for_migration = 0;

    for (size_t i = 0 ; i < number_of_nodes_in_y; i++)
    {
        for (size_t j = 0; j < number_of_nodes_in_x; j++)
        {
            if (its_surface[i][j]->get_node_state() != do_not_use &&
                its_surface[i][j]->get_node_state() != free_place)
            {
                num_element_for_migration++;
            }
        }
    }
    if (num_element_for_migration == 0)
    {
//        cout << "The migration process does not go" << endl;
        return;
    }
    unsigned int index = generator_null_one() * get_all_nodes(); //генерирует случайное число

    while ((get_element_by_common_index(index)->get_node_state() == do_not_use) || \
           (get_element_by_common_index(index)->get_node_state() == free_place) ||
           get_element_by_common_index(index)->num_free_first_neighbors() == 0)
    {
        //Здесь продолжаем генерировать случайное число, если полученное ранее случайное число является индексом элемента,
        //который не должен мигрировать, то есть это свободное место или ЗАПРЕТНОЕ МЕСТО =О
        index = generator_null_one() * get_all_nodes();
    }
    unsigned int neighbors = generator_null_one() * 3;
    while (get_element_by_common_index(index)->get_first_neighbors() \
           [neighbors]->get_node_state() != free_place)
    {
        neighbors = generator_null_one() * 3;
    }
    get_element_by_common_index(index)->get_first_neighbors() \
            [neighbors]->set_node_state(get_element_by_common_index(index)->get_node_state());
    get_element_by_common_index(index)->set_node_state(free_place);
}

void Surface::velocity_calculation() //Эта функция для расчета скоростей по всей поверхности, то есть по 3-му алгоритму
{
    unsigned int num_free_place = 0;
    for (size_t i = 0; i < number_of_nodes_in_y; i++)
    {
        for (size_t j = 0; j < number_of_nodes_in_x; j++)
        {
            if (its_surface[i][j]->get_node_state() == free_place)
            {
                num_free_place++;
            }
        }
    }
    unsigned int num_particles = 0; //количество частиц
    for (size_t i = 0; i < number_of_nodes_in_y; i++)
    {
        for (size_t j = 0; j < number_of_nodes_in_x; j++)
        {
            if ((its_surface[i][j]->get_node_state() != free_place) && (its_surface[i][j]->get_node_state() != do_not_use))
            {
                num_particles++;
            }
        }
    }
    unsigned int num_first_neighbors = 0;
    for (size_t i = 0; i < number_of_nodes_in_y; i++)
    {
        for (size_t j = 0; j < number_of_nodes_in_x; j++)
        {
            if (its_surface[i][j]->get_node_state() != Surface::do_not_use && \
                its_surface[i][j]->get_node_state() != Surface::free_place)
            {
                for (size_t f = 0; f < 3; f++)
                {
                    if (its_surface[i][j]->get_first_neighbors()[f]->get_node_state() == Surface::free_place)
                    {
                        num_first_neighbors++;
                    }
                }
            }
        }
    }
//    cout << "num_first_neighbors = " << num_first_neighbors << endl;
    v_adsorpion_surface = k_adsorpion_surface * num_free_place;
//    cout << "V_ads = " << v_adsorpion << endl;
    v_desorpion_surface = k_desorpion_surface * num_particles;
//    cout << "V_des = " << v_desorpion << endl;
    v_migration_surface = k_migration_surface * num_first_neighbors;
//    double E_0 = 0.5; // Энергия активации, если у узла 3 свободных места
//    double delta_eps_1 = 0.2;
//    v_migration = k_migration * exp(-1 * (E_0 + num_first_neighbors * delta_eps_1)/RT);
//    cout << "V_mig = " << v_migration << endl;
    v_summ_surface = v_adsorpion_surface + v_desorpion_surface + v_migration_surface;
//    cout << "V_summ = " << v_summ << endl;
}

void Surface::selection_process_for_node()//Здесь происходит выбор процесса для конкретной ноды.
{
//    cout << "selection_process" << endl;
//    double v_ad = 0.3, v_des = 0.3, v_mig = 0.4;
    Node * n = select_node(); //Выбор ноды с минимальным временем пребывания в ячейке
//    cout << "select node with state = " << n->get_node_state() << " x= "
//         << n->get_x_index() << " y = " << n->get_y_index() << endl;
    double gener = generator_null_one() * n->get_v_summ();
    /*cout << "V_ad = " << n->get_v_adsorpion() << " V_des = " << n->get_v_desorpion()
         << " V_mig = " << n->get_v_migration()  << " gener = " << gener << endl << endl;*/
    if (gener <= n->get_v_adsorpion())
    {

        n->adsorption(Surface::substance1);
//        print_surface();
    }
//    else if (gener <= n->get_v_adsorpion() + n->get_v_desorpion())
//    {
////        cout << "case 2" << endl;
//        desorption();
////        print_surface();
//    }
//    else if (gener <= n->get_v_adsorpion() + n->get_v_desorpion() + n->get_v_migration())
    else if (gener <= n->get_v_adsorpion() + n->get_v_migration())
//    if (gener <= n->get_v_migration())
    {
//        cout << "case 3" << endl;
        n->migration();
//        print_surface();
    }
    else {
        cout << "----------------------------------------no process" << endl;
    }
    if (n->get_node_state() != free_place)
        all_free_nodes--;
//    cout << get_all_free_nodes() << endl;
}

void Surface::selection_process_for_surface()
{
//    cout << "selection_process" << endl;
//    double v_ad = 0.3, v_des = 0.3, v_mig = 0.4;
    velocity_calculation();
//    double d = generator_without_limitation();
//    unsigned d = generator_without_limitation();
    double gener = generator_null_one() * v_summ_surface;

    cout << "V_ad = " << v_adsorpion_surface << " V_des = " << v_desorpion_surface
         << " V_mig = " << v_migration_surface  << " gener = " << (double)gener << endl << endl;

    if (gener <= v_adsorpion_surface) //и не равно 0
    {
//        cout << "case 1" << endl;
        adsorption(Surface::substance1);
//        print_surface();
    }
//    else if (gener <= v_adsorpion_surface + v_desorpion_surface)
//    {
//        cout << "case 2" << endl;
//        desorption();
//        print_surface();
//    }
    else if (gener <= v_adsorpion_surface + v_desorpion_surface + v_migration_surface)
//    if (gener <= v_migration_surface)
    {
//        cout << "case 3" << endl;
        migration();
//        print_surface();
    }
    else {
        cout << "no process" << endl;
    }

}

void Surface::set_all_delta_t_j()//расчет времени пребывания для всех узлов
{
    set_all_velocity_calculation();//расчет скоростей для всех узлов
    for (size_t i = 0 ; i < number_of_nodes_in_y; i++)
    {
        for (size_t j = 0; j < number_of_nodes_in_x; j++)
        {
                its_surface[i][j]->set_delta_t_j();
//            if (its_surface[i][j]->get_node_state() != do_not_use)
//                cout << "[" << i << "][" << j << "] state = " << its_surface[i][j]->get_node_state() <<
//                        " delta_t_j = " << its_surface[i][j]->get_delta_t_j() << endl;
//            else
//                break;
        }
    }
}

void Surface::set_all_velocity_calculation()//расчет скоростей для всех узлов
{
    for (size_t i = 0 ; i < number_of_nodes_in_y; i++)
    {
        for (size_t j = 0; j < number_of_nodes_in_x; j++)
        {
            its_surface[i][j]->velocity_calculation();
        }
    }
}

Node *Surface::select_node() // находит ноду с минимальным временем пребывания ячейки в текущем состоянии
{
    set_all_delta_t_j();//расчет скоростей и времен пребывания для всех узлов
    Node * result = its_surface[0][0];
    double min_t = 100000;
    for (size_t i = 0 ; i < number_of_nodes_in_y; i++)
    {
        for (size_t j = 0; j < number_of_nodes_in_x; j++)
        {
            if (its_surface[i][j]->get_node_state() == do_not_use)
            {
//                cout << "select node called" << endl;
                continue;
            }
            if (its_surface[i][j]->get_delta_t_j() <= min_t && its_surface[i][j]->get_delta_t_j() != 0)
            {
                if (its_surface[i][j]->get_v_summ() != 0)
                {
                    min_t = its_surface[i][j]->get_delta_t_j();
//                    cout << "\t It is if : mit_t = " << min_t  << " state = " << its_surface[i][j]->get_node_state() << endl;
                    result = its_surface[i][j];
                }
                else
                {
                    continue;
                }
            }
        }
    }
//    cout << "----------------- select node begin -----------------" << endl;
//    cout << "RESULT: mit_t = " << min_t << " for [" << result->get_x_index() << "]["<< result->get_y_index() << "] = "
//         << result->get_node_state() << " v_mig = " << result->get_v_migration() << " v_ad = " << result->get_v_adsorpion()
//         << endl;
//    cout << "----------------- select node end -----------------" << endl;
    return result;
}
