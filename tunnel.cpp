/**
 * @file tunnel.cpp
 * @author Dragoslav Rakić (dragoslav_rakic@outlook.com)
 * @date 15, February, 2017
 * @brief Program with algorithm for calculating the most quicker way for a number of people to pass through a tunnel.
 *
 * @bug No known bugs.
 *
 * This program solves the issue, which is the fastest way passing through a narrow dark tunnel for a number of people
 * with having only one flashlight. The tunnel can pass only two people at the same time and somebody has to go back
 * in order to return flashlight. Everyone has a certain speed, two people travel at the speed of those person that has
 * a lowest speed. Person that is returning back travel at his own speed.
 * Although it seems that the solution for this problem can be easily solved with a recursive function, this program
 * represents a new way approach for this problem. Although the program uses a recursive function, it is used only to
 * summarize all the results of the solutions in vector of paths.
 * Program uses class solutionsGenerator for creating a matrix (two-dimensional array), and all possible solutions for
 * each way of the path, forward (two persons going), backward(one person returning). Program shows how we can transform
 * this problem in binary operations. For example if we have 3 persons that is on one side, lets say at beginning on tunnel
 * we can represent this with 111, if first two people cross on other side we can represent this as 110.
 * Program can do in theory bit shifting and bit operations to solve this problem, but for know we will simulate bits like
 * characters.
 * We will do same operation on bits with another program version.
 *
 * Let's see some examples:
 * 111 FO (beginning of the tunnel) forward->backward
 * 110 B0 (end of the tunnel) backward->forward
 * 101 F1 (beginning of the tunnel) forward->backward
 * 111 B1 (end of the tunnel)
 *
 * Scenario of this example - In F0 we see all people is on beginning of the tunnel, on B0 first two people cross the tunnel,
 *                            on F1 first person is returned, on B1 first and last person move to the end of tunnel.
 *
 * On B0 we have tree solutions: 101, 110, 011. The same solutions we have on F1. So we can make a 4 matrices of solutions:
 *
 *             B0[0] 101    F1[0] 101
 * F0[0] 111   B0[1] 110    F1[1] 110    B1[0] 111
 *             B0[2] 011    F1[2] 011
 *
 *             Path possibilities: F0[0],B[0],F1[1],B1[0]
 *                                 F0[0],B[0],F1[2],B1[0]
 *                                 F0[0],B[1],F1[0],B1[0]
 *                                 F0[0],B[1],F1[2],B1[0]
 *                                 F0[0],B[2],F1[0],B1[0]
 *                                 F0[0],B[2],F1[1],B1[0]
 *
 * Rules:
 * - forward->backward is a path operation that check out binary match. If we had on beginning of the tunnel 4 persons with
 * representation 0111, and on the end 0111 after forward->backward operation we see that there is something wrong.
 * Why? If we have on beginning 0111, then we now that on the end is 1000. After forward->backward operation from
 * 0111 on the end only can be: 1011, 1101 and 1110 because two people are returning and the first is already on the end.
 * - same is for backward->forward operation, here one person is returning so if we have on end 1010 we cannot have on the
 * beginning 1110. Why? If we have on end 1010, we know that on the beginning is 0101, so possible solutions are: 1101,0111.
 *
 */

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>




/**@brief Structure which contains information about the person*/
struct PersonInformation
{
    std::string name;
    unsigned speed;
};

/**@brief Structure which contains pointers to information about the persons
 *
 * Purpose of this structure is to connect vector of paths to every user, we have two users here for backward->forward operation
 * when two people is returning, the lower speed is then assigned. For forward->backward operation on second name is nullptr*/
struct PersonPointers
{
    std::string *first_name = nullptr;
    std::string *second_name = nullptr;
    unsigned *speed = nullptr;
};


/**
 * @brief Class Person creates individual person and set persons name and speed.
 *
 * This class forms a array of Persons by the function enterUser(), array is provided from a structure PersonalInformation for
 * each user, pointer of array  mp_of_persons points to a individual structure which contains all information about the user.
 *
 */
class Person
{
    unsigned m_number_of_persons;
    PersonInformation *mp_of_persons; /**< pointer to array of structure PersonInformation*/

public:

    /** @brief Function returns a number of Persons.
     *  @param no parameters
     *  @return returns the number of persons*/
    unsigned numberOfPeople() {return m_number_of_persons;}

    /** @brief Constructor of a class Person*/
    Person() : m_number_of_persons(0), mp_of_persons(nullptr) {}

    /** @brief Destructor of a class Person*/
    ~Person() { delete[] mp_of_persons;}

    /** @brief Overloading operator[] that returns from arrays index structure of PersonInformation*/
    const PersonInformation& operator[] (const unsigned index)
    {
        return mp_of_persons[index];
    }

    /**
     * @brief Function enterUser() takes the the number of persons and fills persons data, name and speed and then returns
     * array of Persons information to the private member mp_of_persons, pointer to array of structure PersonInformation.
     *
     * @param no parameter
     * @return no return
     *
     */
    void enterUser()
    {

        unsigned number_of_persons;

        std::cout << "\nEnter number of Persons:";
        std::cin >> number_of_persons;


        mp_of_persons = new PersonInformation[number_of_persons];
        m_number_of_persons = number_of_persons;

        for (unsigned i = 0; i < number_of_persons; i++)
        {
            std::cout << "\nEnter name of Person:";
            std::cin >> mp_of_persons[i].name;
            std::cout << "\nEnter speed of Person:";
            std::cin >> mp_of_persons[i].speed;
        }
    }
    /**
     * @brief Function personsInformation() returns a pointer to array of structure PersonInformation
     *
     * @param no parameters
     * @return pointer to array of structure PersonInformation
     */
    PersonInformation* personsInformation()
    {
        return mp_of_persons;
    }


};




using bit = char;               /**< simulating character like a bit, char '1', char '0'*/
using map_path_row = unsigned;  /**< key value of map pointer m_map_path_speed*/


/**
 * @brief Class solutionsGenerator generates all possible solutions from ones and zeros ('1' and '0').
 *
 * Class defines method for making a matrix of all possible solutions with logical ones and zeros.
 * Constructor of the class receives the number of logical ones and zeros, addition of this two
 * parameters give the class number of columns, and number of rows depends on the number of
 * all possible solutions. The goal is to from a given parameters make a matrix, two dimensional
 * array with all solutions.
 */
class solutionsGenerator
{
    unsigned m_matrix_rows;                  /**< number of rows for two-dimensional arrays*/
    unsigned m_matrix_columns;               /**< number of columns for two-dimensional arrays*/
    unsigned m_number_of_ones;               /**< number of logical ones '1' defined in class constructor*/
    bit **mp_matrix;                         /**< double pointer points to two-dimensional array(matrix) of generated solutions*/
    PersonInformation *pointer_of_persons;   /**< pointer to array of structure PersonInformation*/

public:

    std::map<map_path_row, PersonPointers*> *m_map_path_speed;  /**< map for entering rows that connect path and info about travelers*/
    solutionsGenerator *mp_next;                                /**< pointer to the next solution generator connected via << and >> operators*/


    /** @brief Constructor of a class.*/
    solutionsGenerator()
    : m_matrix_rows(0), m_matrix_columns(0), m_number_of_ones(0), mp_matrix(nullptr),
      pointer_of_persons(nullptr), m_map_path_speed(nullptr), mp_next(nullptr) {}

    /** @brief Destructor of a class.*/
    ~solutionsGenerator() { for(unsigned i = 0; i < m_matrix_rows; i++) delete[] mp_matrix[i]; }


    /** @brief Class operator() from parameters input create possible solution matrix.
     *
     * This overloading operator from its parameters create an matrix, two-dimensional array of all combinations zeros
     * and ones. This is achieved with for loops that creates first row and then by modifying previous rows creates
     * a matrix, ie. two-dimensional array.
     *
     *  @param number_of_ones receives number of logical ones
     *  @param number_of_zeros receives number of logical zeros
     *  @param p_of_persons creates a link to the array pointer for structure PersonInformation
     * */
    solutionsGenerator& operator() (const unsigned number_of_ones, const unsigned number_of_zeros, PersonInformation *p_of_persons)
    {
        m_matrix_rows    = number_of_ones * number_of_zeros + 1;
        m_matrix_columns = number_of_ones + number_of_zeros,
                m_number_of_ones = number_of_ones;
        mp_matrix        = new bit*[m_matrix_rows];        /**< creation of two dimensional array pointers*/
        pointer_of_persons = p_of_persons;

        m_map_path_speed = new std::map<map_path_row, PersonPointers*>[m_matrix_rows];

        for (unsigned i = 0; i < m_matrix_rows; i++)
            mp_matrix[i] = new bit[m_matrix_columns];            /**< insert in each row new columns*/

        for (unsigned i = 0; i < m_matrix_columns; i++)
            mp_matrix[0][i] = i < m_number_of_ones ? '1' : '0';  /**< generating first row*/


        for (unsigned i = 1; i < m_matrix_rows; i++)            /**< generating rows after first one*/
        {
            for (unsigned j = 0; j < m_matrix_columns; j++)
                mp_matrix[i][j] = mp_matrix[ i - 1 ][j];       /**< copy row before*/

            for (int k = (int) m_matrix_columns - 1; k >= 0 ; k--)
                if (mp_matrix[i][k - 1] == '1' && mp_matrix[i][k] == '0') /**< bit generation through bit change order */
                {
                    mp_matrix[i][k - 1] = '0';
                    mp_matrix[i][k]     = '1';
                    break;
                }
        }

        return *this;
    }

    /**
     * @brief Class operator>> receives as parameter another class of solutionsGenerator for forward backward scan
     *
     * This class operator scan two solutionsGenerators, this pointer generator and rhs generator. This is forward backward
     * operation that loops through rows from both generators looking for a pattern match. After pattern match is found
     * in both rows possible path is then recorded in map m_map_path_speed. In map we are putting in which rhs index row is
     * pattern found, and from given patterns we found users that are traveling and record in a map by value pointer to
     * this users.
     *
     * @param rhs parameter, right hand side generator reference
     */
    void operator>> (solutionsGenerator &rhs)
    {
        mp_next = &rhs;                                            /**< pointer that connect this pointer generator to rhs generator*/

        for(unsigned i = 0; i < m_matrix_rows; i++)                /**< checks all rows from this pointer matrix*/
        {
            for(unsigned k = 0; k < rhs.m_matrix_rows; k++)        /**< checks all rows from rhs reference matrix*/
            {
                bool found_zero = false;                           /** tree bool variable for checking if in each row from *this matrix */
                bool found_first_one = false;                      /** and rhs matrix is there a pattern that loops are looking through columns,*/
                bool found_second_one = false;                     /** pattern must be two ones in same column, if its zeros abort scanning, no match*/
                unsigned column_of_first_person;
                unsigned column_of_second_person;

                for(unsigned j = 0; j < m_matrix_columns; j++)         /**< checks all columns from this pointer matrix*/
                {
                    if (mp_matrix[i][j] == '0' && rhs.mp_matrix[k][j] == '0')  /**< checks zero '0' match in same column index*/
                    {
                        found_zero = true;
                        break;
                    }

                    if (mp_matrix[i][j] == '1' && rhs.mp_matrix[k][j] == '1') /**< checks one '1' match in same column index*/
                    {
                        if (found_first_one)
                        {
                            column_of_second_person = j;
                            found_second_one = true;
                        }
                        else
                        {
                            column_of_first_person = j;
                            found_first_one = true;
                        }
                    }
                }

                if (!found_zero && found_second_one)    /**< if no '0' and two '1' are found then record rhs row in which is found and persons data*/
                {
                    PersonPointers *temp = new PersonPointers;
                    temp->first_name  = &pointer_of_persons[column_of_first_person].name;
                    temp->second_name = &pointer_of_persons[column_of_second_person].name;

                    unsigned speed_column_check                                                                           /*< put lower speed**/
                    = pointer_of_persons[column_of_first_person].speed > pointer_of_persons[column_of_second_person].speed
                    ? column_of_first_person : column_of_second_person;
                    temp->speed = &pointer_of_persons[speed_column_check].speed;

                    m_map_path_speed[i][k] = temp;                               /**< put in map rhs rows (k) by key and persons data by value*/
                                                                                 /**< value i is index of this pointer matrix row thats compared*/
                }
            }
        }

    }

    /**
     * @brief Class operator<< receives as parameter another class of solutionsGenerator for backward forward scan
     *
     * This class operator scan two solutionsGenerators, this pointer generator and rhs generator. This is backward forward
     * operation that loops through rows from both generators looking for a pattern match. After pattern match is found
     * in both rows possible path is then recorded in map m_map_path_speed. In map we are putting in which rhs index row is
     * pattern found, and from given patterns we found users that are traveling and record in a map by value pointer to
     * this user.
     *
     * @param rhs parameter, right hand side generator reference
     */
    void operator<< (solutionsGenerator &rhs)
    {

        mp_next = &rhs;                                            /**< pointer that connect this pointer generator to rhs generator*/

        for(unsigned i = 0; i < m_matrix_rows; i++)                /**< checks all rows from this pointer matrix*/
        {
            for(unsigned k = 0; k < rhs.m_matrix_rows; k++)        /**< checks all rows from rhs reference matrix*/
            {
                bool found_zero = false;                           /** tree bool variable for checking if in each row from *this matrix */
                bool found_one = false;                      /** and rhs matrix is there a pattern that loops are looking through columns,*/
                unsigned column_of_first_person;

                for(unsigned j = 0; j < m_matrix_columns; j++)         /**< checks all columns from this pointer matrix*/
                {
                    if (mp_matrix[i][j] == '0' && rhs.mp_matrix[k][j] == '0')  /**< checks zero '0' match in same column index*/
                    {
                        found_zero = true;
                        break;
                    }

                    if (mp_matrix[i][j] == '1' && rhs.mp_matrix[k][j] == '1') /**< checks one '1' match in same column index*/
                    {
                        column_of_first_person = j;
                        found_one = true;
                    }
                }

                if (!found_zero && found_one) /**< if no '0' and one '1' are found then record rhs row in which is found and persons data*/
                {
                    PersonPointers *temp = new PersonPointers;
                    temp->first_name  = &pointer_of_persons[column_of_first_person].name;
                    temp->speed       = &pointer_of_persons[column_of_first_person].speed;
                    m_map_path_speed[i][k] = temp;                            /**< put in map rhs rows (k) by key and persons data by value*/
                                                                              /**< value i is index of this pointer matrix row thats compared*/
                }
            }
        }

    }


    /** @brief Function matrix() returns a double pointer mp_matrix.*/
    bit** matrix() { return mp_matrix; }

    /** @brief Function rows() returning number of rows in matrix.*/
    unsigned rows() {return m_matrix_rows;};

    /** @brief Function columns() returning number of columns in matrix.*/
    unsigned columns() {return m_matrix_columns;};

};


using vector_of_persons = std::vector<PersonPointers*>;
/**
 * @brief pathSortingFunction() recursive function sort all possibilities from generated matrices and from theirs map entries
 *
 * This function sort in pth_vector all possible solutions from recursion method that operates on solutionsGenerator pointer
 * start. This pointer is first of many pointers that goes from each one generated matrices ie. generators, the information
 * from each generator is connected and put in vector for easier looping through iterator.
 *
 * @param start is a pointer that points to solutionsGenerator generator
 * @param pth_vector is a vector of vectors reference in which will be recorded all possible solutions
 * @param dir_number is reference of half number directions through tunnel
 *
 * @return no return
 * */

void pathSortingFunction(const unsigned i, solutionsGenerator *start, std::vector<vector_of_persons> &pth_vector, const unsigned &dir_number)
{
    static bool end;                                 /**< designate end of recursion*/
    static vector_of_persons temp_path_vec;          /**< temporary path for recursion method*/
    static unsigned temp_vec_row = 0;                /**< iteration through recursion*/

    if(start != nullptr)
        for(auto &it : start->m_map_path_speed[i])   /**< i is row from m_map_path_speed*/
        {
            temp_path_vec.resize(temp_vec_row);      /**< resize vector of persons when its iteration from recursion decreases*/
            temp_path_vec.push_back(it.second);      /**< push back person(s) name and speed*/
            end = true;
            ++temp_vec_row;
            pathSortingFunction(it.first, start->mp_next, pth_vector, dir_number);    /**recursion*/
            --temp_vec_row;
            if(end && ((dir_number*2 - 2) == temp_vec_row)) /**< if end of solutions and end of direction path */
            {
                pth_vector.push_back(temp_path_vec);        /**< on end of recursion push back persons path data */
            }
            end = false;
        }
}

/**
 * @brief printMinimum() function prints on screen all paths and then minimum needed path through iteration of persons vector
 *
 * @param v_of_paths contains all path solutions
 * @return no return
 */
void printMinimum(std::vector<vector_of_persons> &v_of_paths)
{
    unsigned minimal_speed = 0;
    std::vector<PersonPointers*> min_vector_of_paths;


    for (auto &it : v_of_paths)              /**< vector rows iteration*/
    {
        unsigned total_speed = 0;

        for (auto &itb : it)
        {
            if(itb->second_name != nullptr)  /**< path iteration*/
                std::cout << "->";
            else
                std::cout << "<-";

            std::cout << *itb->first_name;
            if(itb->second_name != nullptr)
                std::cout << "-" << *itb->second_name;
            std::cout << " " << *itb->speed << " ";
            total_speed += *itb->speed;
        }
        if(minimal_speed == 0)
        {
            minimal_speed = total_speed;
            min_vector_of_paths = it;
        }
        else if(minimal_speed > total_speed)
        {
            minimal_speed = total_speed;
            min_vector_of_paths = it;
        }
        std::cout << "** total speed: " << total_speed << " **\n";
    }

    unsigned min_speed = 0;

    std::cout << "\n***minimal speed ***" << "\n";
    for (auto &it : min_vector_of_paths)
    {
        if(it->second_name != nullptr)
            std::cout << "->";
        else
            std::cout << "<-";

        std::cout << *it->first_name;
        if(it->second_name != nullptr)
            std::cout << "-" << *it->second_name;
        std::cout << " " << *it->speed << " ";
        min_speed += *it->speed;
    }

    std::cout << "** minimal speed: " << min_speed << " **\n";
}

int main()
{

    Person Persons;
    Persons.enterUser();

    unsigned directions_number = (Persons.numberOfPeople() - 1);  /**< half number of directions through tunnel*/

    /**< two way direction*/
    solutionsGenerator forward[directions_number];
    solutionsGenerator backward[directions_number];

    unsigned f_pattern_one  = Persons.numberOfPeople();    /**< number of patterns '1' thats decreasing*/
    unsigned f_pattern_zero = 0;                           /**< number of patterns '0' thats increasing*/
    unsigned b_pattern_one = 2;                            /**< number of patterns '1' thats increasing*/
    unsigned b_pattern_zero = Persons.numberOfPeople() - 2;/**< number of patterns '0' thats decreasing*/


    /**< loop for creating all possible solution matrices for each direction forward and backwards*/
    for (unsigned i = 0; i < directions_number; i++)
    {
        forward[i](f_pattern_one--, f_pattern_zero++, Persons.personsInformation());
        backward[i](b_pattern_one++, b_pattern_zero--, Persons.personsInformation());
    }

    /**< connect matrices(generators), and scan them with operators >> and <<  */
    for (unsigned i = 0; i < directions_number; i++)
    {
        forward[i] >> backward[i];

        if ( (i + 1) < directions_number)
            backward[i] << forward[i + 1];
    }


    solutionsGenerator *start = &forward[0];              /**< start from first generated matrix*/

    std::vector<vector_of_persons> vector_of_paths;       /**< pointer that connect this pointer generator with rhs generator*/


    for(unsigned i = 0; i < start->rows(); i++)
        pathSortingFunction(i, start, vector_of_paths, directions_number);


    printMinimum(vector_of_paths);



    return 0;
}

