//
// Created by alexc on 3/9/2019.
//

#include "Monom.h"
#include <cstdlib>

/*__тупо для битового вывода__*/
#include <bitset>
#include <climits>
/*____*/

/*_________BITSET IMPLMENTATION_________*/
/*public functions*/

bitset::Monom::Monom()
{
    _arr = nullptr;
}

bitset::Monom::Monom(int mi, int ma):_min(mi), _max(ma)
{
    if(_min > _max)
        return;
    _size = getSize(_min, _max);
    std::cout << "SIZE: " << _size << std::endl;
    _arr = initArr(_arr, _size);
}

bitset::Monom::Monom::~Monom()
{
    delete [] _arr;
}

void bitset::Monom::INSERT(elem_t x)
{
    if(x > _max || x < _min)
        return;
    int abs_x = std::abs(x);
    int val = abs_x / INT_SIZE;
    int pos = (x < 0) ? ((x % INT_SIZE == 0) ? val - 1 : val) : val;
    int real_pos = pos;
    if(_min >= 0)
    {
        std::cout << "insert -> " << x << std::endl;
        std::cout << "real_pos -> " << real_pos  << std::endl;
        val = ((((pos + 1) * INT_SIZE) - 1) - x) - ((pos - 1) * INT_SIZE); //Смещение влево
        _arr = add_in_pos(_arr, val, real_pos - 1);
        return;
    }
    if(_min < 0 && _max >= 0)
    {
        int min_abs = getMinAbs(_max, _min);
        pos++;
        int zero_pos = min_abs + 1;
        if(x >= 0)
        {
            real_pos = zero_pos + pos;
            val = (((pos + 1) * INT_SIZE) - 1) - x - ((pos - 1) * INT_SIZE);
            _arr = add_in_pos(_arr, val, real_pos - 1);
            return;
        }
        if(x < 0)
        {
            real_pos = zero_pos - (pos);
            val = (((pos) * INT_SIZE)) - abs_x;// - ((pos - 1) * INT_SIZE);
            val = INT_SIZE - val - 1;
            _arr = add_in_pos(_arr, val, real_pos);
            return;
        }
    }
    if(_max < 0)
    {
        int max_abs = getMaxAbs(_max, _min);
        real_pos = max_abs - pos;
        val = (((pos + 1) * INT_SIZE)) - abs_x;
        val = INT_SIZE - val - 1;
        _arr = add_in_pos(_arr, val, real_pos);
    }
}

void bitset::Monom::PRINT() const
{
    if(_max < 0)
    {
        print_minus(_size);
        std::cout << std::endl;
        return;
    }
    if (_min >= 0)
    {
        print_plus();
        return;
    }
    int min_abs = getMinAbs(_max, _min);
    int zero_pos = min_abs + 1;
    std::cout << "zero_pos: " << zero_pos << std::endl;
    print_minus(zero_pos);
    print_plus(zero_pos);
}

bitset::Monom & bitset::Monom::UNION(const Monom & m1, const Monom & m2)
{
//    if(m1._max < m2._min)
//    {
//        //новое мн-во и все копируем
//        return *this;
//    }
    _max = (m1._max > m2._max) ? m1._max : m2._max;
    _min = (m1._min < m2._min) ? m1._min : m2._min;
    _size = getSize(_min, _max);
    _arr = initArr(_arr, _size);
    int copyTo = getLowerCopyPos(m1._min, m1._max, m2._min, m2._max);
    int rCopyCount = getCopyCountElems(m1._min, m1._max, m2._min, m2._max);
    int * temp_arr = (m1._min < m2._min) ? m1._arr : m2._arr;
    int * temp2_arr = (m1._max > m2._max) ? m1._arr : m2._arr;
    //отдельная функция
    int i = 0;
    for(; i < copyTo; i++)
    {
        _arr[i] = temp_arr[i];
    }
    int copySize = _size - rCopyCount;
    int k = copySize;
    int k2 = ((m1._max > m2._max) ? m1._size : m2._size) - rCopyCount;
    for(; k < _size; k++,k2++)
    {
        _arr[k] = temp2_arr[k2];
    }
    int i_root = i;
    int i_temp = copyTo;
    int i_temp2 = 0;
    if(temp_arr != temp2_arr)
    {
        for(; i_root < copySize; i_root++,i_temp++,i_temp2++)
        {
            _arr[i_root] = (temp_arr[i_temp] | temp2_arr[i_temp2]);
        }
    } else
    {
        temp2_arr = (m1._max <= m2._max) ? m1._arr : m2._arr;
        for(; i_root < copySize; i_root++,i_temp++,i_temp2++)
        {
            _arr[i_root] = (temp_arr[i_temp] | temp2_arr[i_temp2]);
        }
    }
    //_____
    return *this;
}

bitset::Monom & bitset::Monom::INTERSECTION(const Monom & m1, const Monom & m2)
{
    _max = (m1._max < m2._max) ? m1._max : m2._max;
    _min = (m1._min > m2._min) ? m1._min : m2._min;
    _size = getSize(_min, _max);
    _arr = initArr(_arr, _size);
    int copyFrom = getLowerCopyPos(m1._min, m1._max, m2._min, m2._max);
    int i_root = 0;
    int i_temp = copyFrom;
    int i_temp2 = 0;
    int * temp_arr = (m1._min < m2._min) ? m1._arr : m2._arr;
    int * temp2_arr = (m1._max > m2._max) ? m1._arr : m2._arr;
    std::cout << "temp_arr: " << temp_arr << " temp_arr2: " << temp2_arr << std::endl;
    if(temp_arr != temp2_arr)
    {
        for(; i_root < _size; i_root++,i_temp++,i_temp2++)
        {
            _arr[i_root] = (temp_arr[i_temp] & temp2_arr[i_temp2]);
        }
    } else
    {
        temp2_arr = (m1._max <= m2._max) ? m1._arr : m2._arr;
        std::cout << "temp_arr: " << temp_arr << " temp_arr2: " << temp2_arr << std::endl;
        for(; i_root < _size; i_root++,i_temp++,i_temp2++)
        {
            _arr[i_root] = (temp_arr[i_temp] & temp2_arr[i_temp2]);
        }
    }
    return *this;
}

bitset::Monom & bitset::Monom::DIFFERENCE(const Monom & m1, const Monom & m2)
{
    _max = m1._max;
    _min = m1._min;
    _size = m1._size;
    _arr = initArr(_arr, _size);
    int copyFrom = getLowerCopyPos(m1._min, m1._max, m2._min, m2._max);
    int i_root = 0;
    int i_temp = copyFrom;
    int i_temp2 = 0;
    int * temp_arr = (m1._min < m2._min) ? m1._arr : m2._arr;
    int * temp2_arr = (m1._max > m2._max) ? m1._arr : m2._arr;
    std::cout << "temp_arr: " << temp_arr << " temp_arr2: " << temp2_arr << std::endl;
    if(temp_arr != temp2_arr)
    {
        for(; i_root < _size; i_root++,i_temp++,i_temp2++)
        {
            _arr[i_root] = (temp_arr[i_temp] & temp2_arr[i_temp2]);
        }
    } else
    {
        temp2_arr = (m1._max <= m2._max) ? m1._arr : m2._arr;
        std::cout << "temp_arr: " << temp_arr << " temp_arr2: " << temp2_arr << std::endl;
        for(; i_root < _size; i_root++,i_temp++,i_temp2++)
        {
            _arr[i_root] = (temp_arr[i_temp] & temp2_arr[i_temp2]);
        }
    }
    return *this;
}

int bitset::Monom::getCopyCountElems(int min1, int max1, int min2, int max2) const
{
    int val_max1 = std::abs(max1) / INT_SIZE;
    val_max1 = (max1 < 0) ? ((max1 % INT_SIZE == 0) ? val_max1 - 1 : val_max1) : val_max1;
    int val_max2 = std::abs(max2) / INT_SIZE;
    val_max2 = (max2 < 0) ? ((max2 % INT_SIZE == 0) ? val_max2 - 1 : val_max2) : val_max2;
    int copy2 = (std::abs(max2) > std::abs(max1)) ? (val_max2 - val_max1) : (val_max1 - val_max2);
    copy2 = ((max1 >= 0 && min1 < 0 && max2 < 0) || (max2 >= 0 && min2 < 0 && max1 < 0)) ? (++copy2) : copy2;
    return copy2;
}

int bitset::Monom::getLowerCopyPos(int min1, int max1, int min2, int max2) const
{
    int val_min1 = std::abs(min1) / INT_SIZE;
    val_min1 = (min1 < 0) ? ((min1 % INT_SIZE == 0) ? val_min1 - 1 : val_min1) : val_min1;
    int val_min2 = std::abs(min2) / INT_SIZE;
    val_min2 = (min2 < 0) ? ((min2 % INT_SIZE == 0) ? val_min2 - 1 : val_min2) : val_min2;
    return ((min1 < 0 && max1 >= 0 && min2 >= 0) || (min2 < 0 && max2 >= 0 && min1 >= 0)) ? ((val_min2 + val_min1) + 1) : ((val_min1 < val_min2) ? (val_min2 - val_min1) : (val_min1 - val_min2));
    //std::cout << "Copy set " << ((min1 < min2) ? 1 : 2) << " to pos: " << copyTo << std::endl;
}

int * bitset::Monom::initArr(int * arr, int size)
{
    arr = new int[size];
    for(int i = 0; i < size; i++)
    {
        arr[i] = 0;
        std::cout << arr[i];
    }
    std::cout << std::endl;
    return arr;
}

int bitset::Monom::getSize(int min, int max)
{
    int size;
    int val = std::abs(max) / INT_SIZE;
    int max_abs = (max < 0) ? ((max % INT_SIZE == 0) ? val - 1 : val) : val; //Определяем позицию максимального эл-та или позицию последнего полож-го после нуля
    val = std::abs(min) / INT_SIZE;
    int min_abs = (min < 0) ? ((min % INT_SIZE == 0) ? val - 1 : val) : val; //Определяем позицию минимального эл-та или позицию последнего отрицательного
    if(max >= 0 && min < 0)
    {
        size = std::abs(max_abs + min_abs) + 1 + 1;
    } else
    {
        size = std::abs(max_abs - min_abs) + 1;
    }
    return size;
}

int bitset::Monom::getMaxAbs(int max, int min) const
{
    int max_abs;
    int val;
    if(max >= 0)
    {
        val = std::abs(max) / INT_SIZE;
        max_abs = (max < 0) ? ((max % INT_SIZE == 0) ? val - 1 : val) : val;
        return (min < 0) ? (max_abs + 1) : max_abs;
    }
    val = std::abs(min) / INT_SIZE;
    max_abs = (min % INT_SIZE == 0) ? val - 1 : val;
    return max_abs;
}

int bitset::Monom::getMinAbs(int max, int min) const
{
    int min_abs;
    int val;
    if(max >= 0)
    {
        val = std::abs(min) / INT_SIZE;
        min_abs = (min < 0) ? ((min % INT_SIZE == 0) ? val - 1 : val) : val;
        return min_abs;
    }
    val = std::abs(max) / INT_SIZE;
    min_abs = (max % INT_SIZE == 0) ? val - 1 : val;
    return min_abs;
}

int * bitset::Monom::add_in_pos(int * arr, int val, int pos)
{
    int temp = 1;
    temp = temp << val;
    arr[pos] |= temp;
    return arr;
}

void bitset::Monom::print_plus(int pos) const
{
    int i = pos;
    int j;
    int i_zp = 0;
    for(; i < _size; i++)
    {
        for(j = 0; j < INT_SIZE; j++)
        {
            if(check_bit(_arr[i], j))
            {
                std::cout << "{" << (INT_SIZE * i_zp) + j << "}";
            }
        }
        i_zp++;
    }
    std::cout << std::endl;
}

void bitset::Monom::print_minus(int size) const
{
    int i = 0;
    int j;
    int lower_border;
    for(; i < size; i++)
    {
        lower_border = (INT_SIZE * (size - i));
        for(j = 0; j < INT_SIZE; j++)
        {
            if(check_bit(_arr[i], j))
            {
                std::cout << "{" << ((lower_border - j))*(-1) << "}";
            }
        }
    }
}

void bitset::Monom::print_plus() const
{
    int i = 0;
    int j;
    int i_zp = std::abs(_min) / INT_SIZE;//getMinAbs(_min, _max);
    for(; i < _size; i++)
    {
        for(j = 0; j < INT_SIZE; j++)
        {
            if(check_bit(_arr[i], j))
            {
                std::cout << "{" << (INT_SIZE * i_zp) + j << "}";
            }
        }
        i_zp++;
    }
    std::cout << std::endl;
}

int bitset::Monom::check_bit(int val, int pos) const
{
    //std::cout << "<check_bit: " << std::endl;
    //std::cout << "pos -> " << pos << std::endl;
    unsigned int temp = (1<<(INT_SIZE - 1));
    //std::cout << "(1<<(pos)) -> " << std::bitset<sizeof((temp>>(pos))) * CHAR_BIT>((temp>>(pos))) << "\n";
    //std::cout << "val -> " << std::bitset<sizeof(val) * CHAR_BIT>(val) << "\n";
    return (val & (temp>>(pos)));
}



/*_________CIRCLE LIST IMPLMENTATION_________*/
/*public functions*/

circlelist::Monom empty_monom;

circlelist::Monom::Monom()
{
    _tail = nullptr;
}

circlelist::Monom::~Monom()
{
    MAKENULL();
}

void circlelist::Monom::MAKENULL()
{
    if(_tail != nullptr)
    {
        _tail = deleteList(_tail);
    }
}

circlelist::Monom & circlelist::Monom::DIFFERENCE(const Monom & m1, const Monom & m2)
{
    if(&empty_monom == this)//Если пытаемся вызвать на фейковом элементе, то возвращаем фейковый элемент
        return empty_monom;
    if(_tail != nullptr)//Если множество на котором вызываем не пустое, то чистим его
    {
        _tail = deleteList(_tail);
    }
    if(&m1 == &m2)//Если множества равны, то возвращаем пустое множество
        return *this;
    if(m1._tail == nullptr) //Если первое множество пустое, то возвращаем пустое множество
        return *this;
    if(m2._tail == nullptr) //Если второе множество пустое, то возвращаем первое множество
    {
       // _tail = new node(m1._tail -> data, nullptr);
        _tail = copyMonom(_tail, m1._tail);
        return *this;
    }
    if(equal(m1._tail, m2._tail)) //Если мн-ва эквивалентны
    {
        return *this;
    }
    if(m1._tail -> data < m2._tail -> next -> data) //Если голова одного мн-ва больше хвоста другого , то просто возвр первое
    {
        _tail = copyMonom(_tail, m1._tail);
        return *this;
    }
    node * temp = m1._tail -> next;
    node * temp2 = m2._tail -> next;
    while (temp != m1._tail && temp2 != m2._tail && (temp -> data < m2._tail -> data) && (temp2 -> data < m1._tail -> data))
    {
        if(temp -> data < temp2 -> data) // Если значение первого мн-ва меньше второго, добавляем и смещаем первый указатель
        {
            _tail = addWithCheck(temp -> data, _tail);
            temp = temp -> next;
            continue;
        }
        if(temp -> data > temp2 -> data)// Если значение второго мн-ва меньше первого, смещаем второй указатель
        {
            temp2 = temp2 -> next;
            continue;
        }
        temp = temp -> next;//Если значения равны, то смещаем оба указателя
        temp2 = temp2 -> next;
    }
    if(temp == m1._tail && temp2 == m2._tail) //Если оба указателя хвосты
    {
        if(temp -> data != temp2 -> data) //Если хвосты не равны, то добавляем значение
        {
            _tail = addWithCheck(temp -> data, _tail);
        }
        return *this;
    }
    if(temp == m1._tail && (temp2 -> data < temp -> data)) //Если дошли до хвоста первого мн-ва и указатель второго, меньше хвоста первого
    {
        while (temp2 != m2._tail && temp -> data > temp2 -> data) //идем по второму мн-ву, пока хвост превого больше указателя на второе
        {
            temp2 = temp2 -> next;
        }
        if(temp -> data != temp2 -> data)
        {
            _tail = addWithCheck(temp -> data, _tail);
        }
        return *this;
    }
    if(temp -> data > m2._tail -> data) //Если значение первого указателя больше хвоста 2-го мн-ва, то просто добавляем первое
    {
        while (temp != m1._tail)
        {
            _tail = addWithCheck(temp -> data, _tail);
            temp = temp -> next;
        }
        _tail = addWithCheck(temp -> data, _tail);
        return *this;
    }
    if(temp2 == m2._tail && temp -> data < temp2 -> data) //Если значение первого указателя меньше хвоста 2-го мн-ва и дошли до конца второго мн-ва
    {
        while (temp != m1._tail)
        {
            if(temp -> data != temp2 -> data)
            {
                _tail = addWithCheck(temp -> data, _tail);
            }
            temp = temp -> next;
        }
        if(temp -> data != temp2 -> data)
        {
            _tail = addWithCheck(temp -> data, _tail);
        }
    }
    return *this;
}

circlelist::Monom & circlelist::Monom::INTERSECTION(const Monom & m1, const Monom & m2)
{
    if(&empty_monom == this) //Если пытаемся вызвать на фейковом элементе, то возвращаем фейковый элемент
        return empty_monom;
    if(_tail != nullptr) //Если множество на котором вызываем не пустое, то чистим его
    {
        _tail = deleteList(_tail);
    }
    if(m1._tail == nullptr || m2._tail == nullptr) //Если одно из множеств пустое, то возвращаем пустое множество
        return *this;
    if(&m1 == &m2) //Если мн-ва равны, то возвращаем одно из можеств
    {
        _tail = copyMonom(_tail, m1._tail);
        return *this;
    }
    if(equal(m1._tail, m2._tail))
    {
        _tail = copyMonom(_tail, m1._tail);
        return *this;
    }
    node * temp = m1._tail -> next;
    node * temp2 = m2._tail -> next;
    if(m1._tail -> next -> data > m2._tail -> data || m2._tail -> next -> data > m1._tail -> data)
        return *this;//Если значение головы первого больше хвоста второго или наоборот, то мн-ва не пересекаются и смысла идти по ним нет
    while(temp != m1._tail && (temp -> data <= m2._tail -> data) && (temp2 -> data <= m1._tail -> data) && temp2 != m2._tail)
    {//Идем по спискам, пока не дошли до конца одного из них, и значения указателей не больше значений хвостов списков
        if(temp -> data > temp2 -> data) //Если значение первого указателя больше значения второго, то перверяем следующий эл-т второго мн-ва
        {
            temp2 = temp2 -> next;
            continue;
        }
        if(temp -> data < temp2 -> data) //Если значение первого указателя меньше значения второго, то перверяем следующий эл-т первого мн-ва
        {
            temp = temp -> next;
            continue;
        }
        //Если значения совпали:
        _tail = addWithCheck(temp -> data, _tail);
        temp2 = temp2 -> next;
        temp = temp -> next;
    }
    //Проверка хвостов мн-в
    if(temp == m1._tail && temp2 == m2._tail) //Если указатели обоих мн-в указывают на последние эл-ты ссписка
    {
        if(temp -> data == temp2 -> data) //Если хвосты равны, то добавляем значение во мн-во
        {
            _tail = addWithCheck(temp -> data, _tail);
        }
        return *this; //Возвращаем мн-во
    }
    if(temp == m1._tail && (temp2 -> data <= temp -> data)) //Если дошли до конца первого мн-ва и указатель на эл-т второго мн-ва не больше хвоста первого
    {
        _tail = intersect_tail(_tail, m1._tail, m2._tail, temp2);
        return *this;
    }
    if(temp2 == m2._tail && (temp -> data <= temp2 -> data)) //Если дошли до конца второго мн-ва и указатель на эл-т первогшо мн-ва не больше хвоста второго
    {
        _tail = intersect_tail(_tail, m2._tail, m1._tail, temp);
        return *this;
    }
    return *this;
}

circlelist::Monom & circlelist::Monom::UNION(const Monom & m1, const Monom & m2)
{
    if(&empty_monom == this)//Если пытаемся вызвать на фейковом элементе, то возвращаем фейковый элемент
        return empty_monom;
    if(_tail != nullptr)//Если множество на котором вызываем не пустое, то чистим его
    {
        _tail = deleteList(_tail);
    }
    if(m2._tail == nullptr)//Если второе множество пустое, то возвращаем первое множество
    {
        _tail = copyMonom(_tail, m1._tail);
        return *this;
    }
    if(m1._tail == nullptr)//Если первое множество пустое, то возвращаем второе множество
    {
        _tail = copyMonom(_tail, m2._tail);
        return *this;
    }
    if(&m1 == &m2)//Если множества равны, то возвращаем первое множество
    {
        _tail = copyMonom(_tail, m1._tail);
        return *this;
    }
    if(m1._tail -> next -> data > m2._tail -> data) //Если голова одного мн-ва больше хвоста другого, то просто сливаем оба мн-ва
    {
        _tail = copyMonom(_tail, m2._tail);
        _tail = addToMonom(_tail, m1._tail);
        return *this;
    }
    if(m2._tail -> next -> data > m1._tail -> data)//Если голова одного мн-ва больше хвоста другого, то просто сливаем оба мн-ва
    {
        _tail = copyMonom(_tail, m1._tail);
        _tail = addToMonom(_tail, m2._tail);
        return *this;
    }
    _tail = copyMonom(_tail, m1._tail); //Записываем мн-во 1 в результат
    node * temp = m2._tail -> next;
    node * temp_r = _tail -> next;
    node * next;
    while (temp != m2._tail) //Идем по мн-ву 2 до хвоста
    {
        if(temp -> data > _tail -> data) //Если текущее значение больше хвоста, то добавляем в конец и идем дальше
        {
            //Добавляем в конец(temp_r)
            _tail = addToEnd(temp -> data, _tail);
            temp = temp -> next;
            continue;
        }
        if(temp -> data < _tail -> next -> data)//Если текущее значение меньше головы, то добавляем в начало и идем дальше
        {
            //добавляем в начало(temp_r)
            next = _tail -> next;
            _tail -> next = new node(temp -> data, next);
            temp = temp -> next;
            continue;
        }
        while (temp_r != _tail) //Идем по иходному списку с позиции temp_r до конца
        {
            if(temp -> data == temp_r -> data) //Если значения совпали, то выходим из цикла
                break;
            if( (temp -> data > temp_r -> data) && (temp -> data < temp_r -> next -> data) ) //Если текущее значение 2-го мн-ва находится в промежутке (an,an+1), то добавляем между ними и выходим из цикла
            {
                next = temp_r -> next;
                temp_r -> next = new node(temp -> data, next);
                temp_r = temp_r -> next;
                break;
            }
            temp_r = temp_r -> next;
        }
        temp = temp -> next;
    }
    if(temp -> data > _tail -> data) //Если хвост 2-го мн-ва больше хвоста первого, то добавляем значение в хвост
    {
        //Добавляем в конец(temp_r)
        _tail = addToEnd(temp -> data, _tail);
        //temp = temp -> next;
        return *this;
    }
    while (temp_r != _tail) //Если первое мн-во не закончилось, то проходим по 1-му мн-ву до конца и сравниваем значения с хвостом второго
    {
        if(temp -> data == temp_r -> data)
            break;
        if( (temp -> data > temp_r -> data) && (temp -> data < temp_r -> next -> data) )
        {
            next = temp_r -> next;
            temp_r -> next = new node(temp -> data, next);
            break;
        }
        temp_r = temp_r -> next;
    }
    if(temp_r == _tail && temp_r -> data < temp -> data) //Если дошли до конца первого и второго мн-ва и хвост первого множества меньше хвоста второго
        _tail = addToEnd(temp -> data, _tail);//Добавляем хвост второго мн-ва в конец первого
    return *this;
    /*copy monom start*/
    /*copy monom end*/
}

void circlelist::Monom::INSERT(const elem_t x)
{
    if(&empty_monom == this)//Если пытаемся вызвать на фейковом элементе, то возвращаем фейковый элемент
        return;
    if(_tail == nullptr) //Если м-во пустое, то создаем новый хвост
    {
        _tail = new node(x);
        return;
    }
    node * temp = _tail -> next; //Указываем на голову
    if(_tail -> next -> data == x || _tail -> data == x) //Если значение головы или хвоста равно x, то такой эл-т уже есть и выходим из функции
    {
        return;
    }
    if(temp == _tail && _tail -> data < x)  //Если в списке один элемент и он меньше x, то добавляем в конец
    {
        _tail -> next = new node(x, _tail);
        _tail = _tail -> next;
        return;
    }
    if(temp == _tail && _tail -> data > x)//Если в списке один элемент и он больше x, то добавляем в начало
    {
        _tail -> next = new node(x, _tail);
        return;
    }
    if(temp -> data > x) //Если  первый эл-т больше x, то добавляем в начало
    {
        _tail -> next = new node(x, temp);
        return;
    }
    if(_tail -> data < x) //Если  последний эл-т меньше x, то добавляем в конец
    {
        _tail -> next = new node(x, temp);
        _tail = _tail -> next;
    }
    while(temp != _tail && temp -> data < x) //Идем до конца мн-ва
    {
        if(temp -> data < x && temp -> next -> data > x) //если значение входит в промежуток (An, An+1), добавляем
        {
            node * next = temp -> next;
            temp -> next = new node(x, next);
            return;
        }
        temp = temp -> next;
    }
}

circlelist::elem_t circlelist::Monom::MIN() const
{
    return _tail -> next -> data;
}

circlelist::elem_t circlelist::Monom::MAX() const
{
    return _tail -> data;
}

bool circlelist::Monom::EMPTY() const
{
    return _tail == nullptr;
}

void circlelist::Monom::PRINT() const
{
    if(&empty_monom == this)
    {
        std::cout << "Fake monom" << std::endl;
        return;
    }
    if(_tail == nullptr)
        return;
    node * temp = _tail -> next;
    std::cout << std::setw(25) << "<data>" << std::endl;
    while (temp != _tail)
    {
        std::cout << std::setw(25) << temp -> data << std::endl;
        temp = temp -> next;
    }
    std::cout << std::setw(25) << temp -> data << std::endl;
}

bool circlelist::Monom::EQUAL(const Monom & m2) const
{
    if(this == &m2) //Если адреса равны, то мн-ва эквивалентны
        return true;
    return equal(_tail, m2._tail);
}

bool circlelist::Monom::equal(node * tail, node * m2_tail) const
{
    if(tail == nullptr && m2_tail == nullptr)
        return true;
    if(tail == nullptr || m2_tail == nullptr) //Если одно из множеств пустое, мн-ва не эквивалентны
    {
        return false;
    }
    if(tail -> data != m2_tail -> data) //Если значения хвостов не равны, то мн-ва не эквивалентны
    {
        return false;
    }
    node * temp1 = tail -> next;
    node * temp2 = m2_tail -> next;
    while(temp1 != tail && temp2 != m2_tail)
    {
        if(temp1 -> data != temp2 -> data) //Если значения не совпали, то мн-ва не эквивалентны
        {
            return false;
        }
        temp1 = temp1 -> next;
        temp2 = temp2 -> next;
    }
    return !(temp1 != tail || temp2 != m2_tail); //Если один из указателей не равен хвосту, то мн-ва не эквивалентны
}

circlelist::Monom & circlelist::Monom::FIND(elem_t x, Monom & m2)
{
    if(&empty_monom == this) //Если вызываем на пустом мн-ве, вернуть пустое мн-во
        return empty_monom;
    node * check;
    if(_tail -> next -> data == x || _tail -> data == x) //Если  значение хвоста или головы первого мн-ва равно x, вернуть первое мн-во
    {
        return *this;
    }
    if(m2._tail -> data == x || m2._tail -> next -> data == x) //Если  значение хвоста или головы второго мн-ва равно x, вернуть второе мн-во
    {
        return m2;
    }
    if(_tail -> next == _tail) //Если в 1-м мн-ве один элемент
    {
        if(m2._tail -> next == m2._tail) //Если второе  мн-во имеет 1 эл-т, то x нет ни в одном мн-ве
        {
            return empty_monom;//false
        }
        check = findPrevEl(x, m2._tail); //Иначе проверяем на наличие во 2-м мн-ве
        if(check != nullptr)
            return m2;
        return empty_monom;
    }
    if(m2._tail -> next == m2._tail) //Если второе мн-во хранит 1 эл-т
    {
        check = findPrevEl(x, _tail); //Проверяем на наличие в 1-ом мн-ве
        if(check != nullptr)
        {
            return *this;
        }
        return empty_monom;//False
    }
    check = findPrevEl(x, _tail);//Проверяем на наличие в 1-ом мн-ве
    if(check != nullptr)
        return *this;
    check = findPrevEl(x, m2._tail);//Иначе проверяем на наличие во 2-м мн-ве
    if(check != nullptr)
        return m2;
    return empty_monom;
}

void circlelist::Monom::DELETE(elem_t x)
{
    if(&empty_monom == this) //Если вызываем на пустом мн-ве
        return;
    //_tail = deleteEL(_tail, x);
    node * prev;
    node * cur;
    if(_tail -> data == x)
    {
        //delete tail
        if(_tail -> next == _tail)
        {
            delete _tail;
            _tail = nullptr;
        } else
        {
            cur = _tail;
            prev = findPrevEl(x, _tail);
            prev -> next = _tail -> next;
            _tail = prev;
            delete cur;
        }
        return;
    }
    if(_tail -> next == _tail)
        return;
    if(_tail -> next -> data == x)
    {
        //delete head
        cur = _tail -> next;
        _tail -> next = cur -> next;
        delete cur;
        return;
    }
    prev = findPrevEl(x, _tail);
    if(prev == nullptr)
        return;
    cur = prev -> next;
    prev -> next = cur -> next;
    delete cur;
}

circlelist::Monom & circlelist::Monom::ASSIGN(const Monom & m)
{
    if(&empty_monom == this)//Если вызываем на пустом мн-ве
        return empty_monom;
    if(&m == this) //Проверка на самоприсваивание
    {
        return *this;
    }
    if(_tail != nullptr) //Если мн-во не пустое, чистим
    {
        _tail = deleteList(_tail);
    }
    if(m._tail == nullptr) //Если присваиваемое мн-во пустое, вернуть текущее пустое мн-во
    {
        return *this;
    }
    _tail = copyMonom(_tail, m._tail);
    return *this;
}

bool circlelist::Monom::REPEAT(const Monom & m) const
{
    if(_tail == nullptr || m._tail == nullptr)
        return true;
    node * temp = _tail -> next;
    node * check;
    if(_tail -> data == m._tail -> data)
        return true;
    while (temp != _tail)
    {
        if(temp -> data == m._tail -> data)
            return true;
        check = findPrevEl(temp -> data, m._tail);
        if(check != nullptr)
            return true;
        temp = temp -> next;
    }
    check = findPrevEl(_tail -> data, m._tail);
    return check != nullptr;
}

circlelist::Monom & circlelist::Monom::MERGE(const Monom & m1, const Monom & m2)
{
    if(&empty_monom == this)//Если пытаемся вызвать на фейковом элементе, то возвращаем фейковый элемент
        return empty_monom;
    if(_tail != nullptr)
    {
        _tail = deleteList(_tail);
    }
    node * temp = m1._tail -> next;
    node * temp2 = m2._tail -> next;
    while(temp != m1._tail && temp2 != m2._tail) //Идем по двум мн-вам и пишем в исходное
    {
        if(temp -> data < temp2 -> data)
        {
            _tail = addWithCheck(temp -> data, _tail);
            temp = temp -> next;
        }
        if(temp -> data > temp2 -> data)
        {
            _tail = addWithCheck(temp2 -> data, _tail);
            temp2 = temp2 -> next;
        }
    }

    if(temp == m1._tail && temp2 == m2._tail)//если в 2-х мн-вах дошли до хвоста
    {
        if(temp -> data < temp2 -> data)
        {
            _tail = addWithCheck(temp -> data, _tail);
            _tail = addToEnd(temp2 -> data, _tail);
        } else
        {
            _tail = addWithCheck(temp2 -> data, _tail);
            _tail = addToEnd(temp -> data, _tail);
        }
    }
    if(temp == m1._tail && temp2 != m2._tail)
    {
        _tail = merge_tail(_tail, m1._tail, m2._tail, temp2);
        return *this;
    }
    if(temp2 == m2._tail && temp != m1._tail)
    {
        _tail = merge_tail(_tail, m2._tail, m1._tail, temp);
    }
    return *this;
}

/*private functions*/

circlelist::node * circlelist::Monom::addWithCheck(elem_t data, node * tail)
{
    if(tail == nullptr)
    {
        tail = new node(data);
    } else
    {
        tail = addToEnd(data, tail);
    }
    return tail;
}

circlelist::node * circlelist::Monom::addToMonom(node * l_el, node * tail)
{
    node * temp = tail -> next;
    node * l_temp = l_el;
    while (temp != tail)
    {
        l_temp -> next = new node(temp -> data, l_temp -> next);
        l_temp = l_temp -> next;
        temp = temp -> next;
    }
    l_temp -> next = new node(tail -> data, l_temp -> next);
    l_temp = l_temp -> next;
    return l_temp;
}
//
//circlelist::node * circlelist::Monom::deleteEL(node * tail, elem_t x)
//{
//    node * prev;
//    node * cur;
//    if(tail -> data == x)
//    {
//        //delete tail
//        if(tail -> next == _tail)
//        {
//            delete tail;
//            tail = nullptr;
//        } else
//        {
//            cur = tail;
//            prev = findPrevEl(x, tail);
//            prev -> next = tail -> next;
//            tail = prev;
//            delete cur;
//        }
//        return tail;
//    }
//    if(tail -> next == tail)
//        return tail;
//    if(tail -> next -> data == x)
//    {
//        //delete head
//        cur = tail -> next;
//        tail -> next = cur -> next;
//        delete cur;
//        return tail;
//    }
//    prev = findPrevEl(x, _tail);
//    if(prev == nullptr)
//        return tail;
//    cur = prev -> next;
//    prev -> next = cur -> next;
//    delete cur;
//    return tail;
//}

circlelist::node * circlelist::Monom::addToEnd(elem_t data, node * tail)
{
    node * next = new node(data, tail -> next);
    tail -> next = next;
    return next;
}

circlelist::node * circlelist::Monom::findPrevEl(elem_t x, node * tail) const
{
    node * temp = tail -> next;
    while (temp != tail)
    {
        if(temp -> next -> data > x)
            return nullptr;
        if(temp -> next -> data == x)
            return temp;
        temp = temp -> next;
    }
    return nullptr;
}

circlelist::node * circlelist::Monom::copyMonom(node * t1, node * t2)
{
    node * temp2 = t2 -> next;
    t1 = new node;
    t1 -> data = t2 -> data;
    node * temp1 = t1;
    while(temp2 != t2)
    {
        temp1 -> next = new node(temp2 -> data, t1);
        temp1 = temp1 -> next;
        temp2 = temp2 -> next;
    }
    return t1;
}

circlelist::node * circlelist::Monom::deleteList(node * tail)
{
    node * temp1;
    node * temp2 = tail -> next;
    while (temp2 != tail)
    {
        temp1 = temp2;
        temp2 = temp2 -> next;
        delete temp1;
    }
    delete temp2;
    tail = nullptr;
    return tail;
}


circlelist::node * circlelist::Monom::intersect_tail(node * tail, node * m1_tail, node * m2_tail, node * temp2)
{
    while (temp2 != m2_tail && m1_tail -> data >= temp2 -> data) //Идем по второму мн-ву, проверяя эл-ты на соответствие с хвостом первого
    {
        if(m1_tail -> data == temp2 -> data)
        {
            tail = addWithCheck(m1_tail -> data, tail);
            return tail;
        }
        temp2 = temp2 -> next;
    }
    if(m1_tail -> data == temp2 -> data) //Если хвосты равны, то добавляем эл-т
    {
        tail = addWithCheck(m1_tail -> data, tail);
    }
    return tail;
}


circlelist::node * circlelist::Monom::merge_tail(node * tail, node * m1_tail, node * m2_tail, node * temp2)
{
    if(m1_tail -> data < temp2 -> data)
        tail = addToEnd(m1_tail -> data, tail);
    while (temp2 != m2_tail)
    {
        tail = addWithCheck(temp2 -> data, tail);
        if(m1_tail -> data > temp2 -> data && m1_tail -> data < temp2 -> next -> data)
            tail = addToEnd(m1_tail -> data, tail);
        temp2 = temp2 -> next;
    }
    tail = addToEnd(temp2 -> data, tail);
    return tail;
}

/*___________________SINGLE LINKED LIST______________________*/

slinkedlist::Monom fake_monom;

slinkedlist::Monom::Monom()
{
    _head = nullptr;
}

slinkedlist::Monom::~Monom()
{
    MAKENULL();
}

void slinkedlist::Monom::MAKENULL()
{
    if(_head != nullptr)
    {
        _head = deleteList(_head);
    }
}

slinkedlist::Monom & slinkedlist::Monom::INTERSECTION(const Monom & m1, const Monom & m2)
{
    if(&fake_monom == this) //Если пытаемся вызвать на фейковом элементе, то возвращаем фейковый элемент
        return fake_monom;
    if(_head != nullptr) //Если множество на котором вызываем не пустое, то чистим его
    {
        _head = deleteList(_head);
    }
    if(m1._head == nullptr || m2._head == nullptr) //Если одно из множеств пустое, то возвращаем пустое множество
        return *this;
    if(&m1 == &m2) //Если мн-ва равны, то возвращаем одно из можеств
    {
        _head = new node(m1._head -> data, nullptr);
        addMonom(_head, m1._head -> next);
        return *this;
    }
//    if(equal(m1._head, m2._head))//Если мн-ва равны, то возвращаем одно из можеств
//    {
//        _head = new node(m1._head -> data, nullptr);
//        addMonom(_head, m1._head -> next);
//        return *this;
//    }
    node * temp = m1._head;
    node * l_el = _head; //Указатель на конец множества
    while (temp != nullptr)
    {
        if(temp -> data == m2._head -> data) //Если значение текущего элемента равно значению головы второго множества
        {
            if(l_el == nullptr) //Если множества в которое мы пишем пустое
            {
                _head = new node(temp -> data, nullptr); //Создаем новую голову
                 l_el = _head; //Указатель на последний элемент теперь равен голове
            }
            else
            {
                l_el -> next = new node(temp -> data, nullptr); //Добавляем элемент в хвост
                l_el = l_el -> next; //Обновляем конец списка
            }
        } else
        {
            if(findPrevEl(temp -> data, m2._head -> next) != nullptr) //Ищем повторяющееся значение во втором множестве, если нашли, добавляем новый элемент
            {
                if(l_el == nullptr) //Если множества в которое мы пишем пустое
                {
                    _head = new node(temp -> data, nullptr); //Создаем новую голову
                    l_el = _head; //Указатель на последний элемент теперь равен голове
                } else
                {
                    l_el -> next = new node(temp -> data, nullptr);
                    l_el = l_el -> next;
                }
            }
        }
        temp = temp -> next;
    }
    return *this;
}

slinkedlist::Monom & slinkedlist::Monom::DIFFERENCE(const Monom & m1, const Monom & m2)
{
    if(&fake_monom == this)//Если пытаемся вызвать на фейковом элементе, то возвращаем фейковый элемент
        return fake_monom;
    if(_head != nullptr)//Если множество на котором вызываем не пустое, то чистим его
    {
        _head = deleteList(_head);
    }
    if(&m1 == &m2)//Если множества равны, то возвращаем пустое множество
        return *this;
    if(m1._head == nullptr) //Если первое множество пустое, то возвращаем пустое множество
        return *this;
    if(m2._head == nullptr) //Если второе множество пустое, то возвращаем первое множество
    {
        _head = new node(m1._head -> data, nullptr);
        addMonom(_head, m1._head -> next);
        return *this;
    }
//    if(equal(m1._head, m2._head))
//        return *this;
    node * temp = m2._head;
    _head = new node(m1._head -> data, nullptr);//Копируем множество m1 в текущее
    addMonom(_head, m1._head -> next);//Копируем множество m1 в текущее
    while (temp != nullptr)
    {
        if(_head -> data == temp -> data) //Если значения головы m1 совпадают с текущим значением
        {
            _head = deleteHead(_head); //Удаляем голову
        } else
        {
            node * prev = findPrevEl(temp -> data, _head); //Ищем значение в искомом множестве
            if(prev != nullptr) //Если нашли повторяющееся значение
            {
                _head = deleteEl(_head, prev); //Удаляем элемент
            }
        }
        temp = temp -> next;
    }
    return *this;
}

slinkedlist::Monom & slinkedlist::Monom::UNION(const Monom & m1, const Monom & m2)
{
    if(&fake_monom == this)//Если пытаемся вызвать на фейковом элементе, то возвращаем фейковый элемент
        return fake_monom;
    if(_head != nullptr)//Если множество на котором вызываем не пустое, то чистим его
    {
        _head = deleteList(_head);
    }
    if(m1._head == nullptr && m2._head == nullptr)
        return *this;
    if(m2._head == nullptr || &m1 == &m2)//Если второе множество пустое или они равны, то возвращаем первое множество
    {
        _head = new node(m1._head -> data, nullptr);
        addMonom(_head, m1._head -> next);
        return *this;
    }
    if(m1._head == nullptr)//Если первое множество пустое, то возвращаем второе множество
    {
        _head = new node(m2._head -> data, nullptr);
        addMonom(_head, m2._head -> next);
        return *this;
    }
//    if(equal(m1._head, m2._head)) //Если мн-ва равны, то возвращаем первое множество
//    {
//        _head = new node(m1._head -> data, nullptr);
//        addMonom(_head, m1._head -> next);
//        return *this;
//    }
    node * temp = m2._head;
    node * check; //Заводим указатель для проверки на существование элемента в мн-ве
    _head = new node(m1._head -> data, nullptr); //Копируем множество m1 в текущее множество
    node * l_el = addMonom(_head, m1._head -> next);
    while (temp != nullptr)
    {
        if(temp -> data != m1._head -> data) //Если значение текущее зачение второго множества не равно значению головы первого множества
        {
            check = findPrevEl(temp -> data, m1._head); //Ищем текущее зачение второго в первом множестве
            if(check == nullptr) // Если ничего не нашло(этого значения нету в первом множестве)
            {
                l_el -> next = new node(temp -> data, nullptr); //Добавляем значение в текущее множество
                l_el = l_el -> next;
            }
        }
        temp = temp -> next;
    }
    return *this;
}

slinkedlist::Monom & slinkedlist::Monom::MERGE(const Monom & m1, const Monom & m2)
{
    if(&fake_monom == this)//Если пытаемся вызвать на фейковом элементе, то возвращаем фейковый элемент
        return fake_monom;
    if(_head != nullptr)//Если множество на котором вызываем не пустое, то чистим его
    {
        _head = deleteList(_head);
    }
    _head = new node(m1._head -> data, nullptr);
    node * temp = addMonom(_head, m1._head -> next);
    addMonom(temp, m2._head);
    return * this;
}

void slinkedlist::Monom::INSERT(elem_t x)
{
    if(&fake_monom == this)//Если пытаемся вызвать на фейковом элементе, то возвращаем фейковый элемент
        return;
    if(_head == nullptr) //Если список пуст
    {
        _head = new node(x, nullptr); //Добавляем новую голову
        return;
    }
    if(_head -> data == x) //Если элемент уже существует, выходим из функции
        return;
    if(findPrevEl(x, _head) != nullptr)//Ищем элемент, если элемент уже существует, выходим из функции
        return;
    node * last = get_last_el(_head); //Добавляем элемент в конец множества
    last -> next = new node(x, nullptr);
}

slinkedlist::Monom & slinkedlist::Monom::ASSIGN(const Monom & m)
{
    if(&fake_monom == this)//Если пытаемся вызвать на фейковом элементе, то возвращаем фейковый элемент
        return fake_monom;
    if(this == &m) //Если происходит самоприсваивание
    {
        return *this;
    }
    if(_head != nullptr) //Если множество на котором вызываем не пустое, то чистим его
    {
        _head = deleteList(_head);
    }
    if(m._head == nullptr) //Если множество m пустое, возвращаем пустое множество
    {
        return *this;
    }
    _head = new node(m._head -> data, nullptr); //Присваиваем мн-во
    addMonom(_head, m._head -> next);
    return * this;
}

bool slinkedlist::Monom::REPEAT(const Monom & m) const
{
    if(_head == nullptr || m._head == nullptr) //Если одно из множеств пустое, возвращаем истину(тк пустое множество, является подмножеством любого множества)
        return true;
    if(_head -> data == m._head -> data)//Если значения голов совпадает, возвращаем истина
        return true;
    node * temp = _head;
    while(temp != nullptr)
    {
        if(temp -> data == m._head -> data) //Сравниваем текщее значение с головным значение
            return true;
        if(findPrevEl(temp -> data, m._head)) //Поиск по значению, если нашли текущий элемент в мн-ве, true
            return true;
        temp = temp -> next;
    }
    return false;
}

bool slinkedlist::Monom::EQUAL(const Monom & m2) const
{
    if(this == &m2)
        return true;
    return equal(_head, m2._head);
}

slinkedlist::elem_t slinkedlist::Monom::MIN() const
{
    node * temp = _head -> next;
    elem_t min = _head -> data;
    while(temp != nullptr)
    {
        if(min > temp -> data)
            min = temp -> data;
        temp = temp -> next;
    }
    return min;
}

slinkedlist::elem_t slinkedlist::Monom::MAX() const
{
    node * temp = _head -> next;
    elem_t max = _head -> data;
    while(temp != nullptr)
    {
        if(max < temp -> data)
            max = temp -> data;
        temp = temp -> next;
    }
    return max;
}

void slinkedlist::Monom::DELETE(elem_t x)
{
    if(_head == nullptr)
        return;
    if(_head -> data == x)
    {
        _head = deleteHead(_head);
        return;
    }
    node * prev = findPrevEl(x, _head);
    if(prev != nullptr)
    {
        _head = deleteEl(_head, prev);
    }
}

slinkedlist::Monom & slinkedlist::Monom::FIND(elem_t x, Monom & m2)
{
    if(&fake_monom == this || &fake_monom == &m2)//Если пытаемся вызвать на фейковом элементе или передать фейковый элемент, то возвращаем фейковый элемент
        return fake_monom;
    if(_head -> data == x)
        return *this;
    if(m2._head -> data == x)
        return m2;
    node * temp = _head -> next;
    node * temp2 = m2._head -> next;
    if(findPrevEl(x, temp) != nullptr)
        return *this;
    if(findPrevEl(x, temp2) != nullptr)
        return m2;
    return fake_monom;
}

bool slinkedlist::Monom::EMPTY() const
{
    return _head == nullptr;
}

void slinkedlist::Monom::PRINT() const
{
    if(&fake_monom == this)
    {
        std::cout << "Fake elem" << std::endl;
        return;
    }
    node * temp = _head;
    std::cout << std::setw(25) << "<data>" << std::endl;
    while (temp != nullptr)
    {
        std::cout << std::setw(25) << temp -> data << std::endl;
        temp = temp -> next;
    }
}

bool slinkedlist::Monom::equal(node * head, node * m2_head) const
{
    if(head == nullptr && m2_head == nullptr)
        return true;
    if(head == nullptr || m2_head == nullptr)
        return false;
    node * temp2 = m2_head;
    int weight = sub_set(head, m2_head);
    if(weight == 0)
        return false;
    int m2_weight = 0;
    while (temp2 != nullptr)
    {
        m2_weight++;
        temp2 = temp2 -> next;
    }
    return (m2_weight == weight);
}

slinkedlist::node * slinkedlist::Monom::deleteEl(node * head, node * prev)
{
    node * cur = prev -> next;
    prev -> next = cur -> next;
    delete cur;
    return head;
}

slinkedlist::node * slinkedlist::Monom::deleteHead(node * head)
{
    node * cur = head;
    head = head -> next;
    delete cur;
    return head;
}

slinkedlist::node * slinkedlist::Monom::addMonom(node * l_el, node * m2_head)
{
    node * temp = l_el;
    node * temp2 = m2_head;
    while (temp2 != nullptr)
    {
        temp -> next = new node(temp2 -> data, nullptr);
        temp = temp -> next;
        temp2 = temp2 -> next;
    }
    return temp;
}

slinkedlist::node * slinkedlist::Monom::findPrevEl(elem_t x, node * head) const
{
    node * temp = head;
    while (temp != nullptr)
    {
        if(temp -> next == nullptr)
            break;
        if(temp -> next -> data == x)
            return temp;
        temp = temp -> next;
    }
    return nullptr;
}

slinkedlist::node * slinkedlist::Monom::get_last_el(node * head) const
{
    node * temp = head;
    node * temp2 = temp -> next;
    while (temp2 != nullptr)
    {
        temp = temp -> next;
        temp2 = temp2 -> next;
    }
    return temp;
}

slinkedlist::node * slinkedlist::Monom::deleteList(node * head)
{
    node * temp1;
    node * temp2 = head;
    while (temp2 != nullptr)
    {
        temp1 = temp2;
        temp2 = temp2 -> next;
        delete temp1;
    }
    head = nullptr;
    return head;
}

int slinkedlist::Monom::sub_set(node * m1_head, node * m2_head) const //Проверка на то, является m2 подмножеством m1(результат 0, если нет, мощность мн-ва, если да)
{
    node * temp = m1_head;
    node * temp2;
    int count = 0;
    while (temp != nullptr)
    {
        temp2 = m2_head;
        while(temp2 != nullptr && temp -> data != temp2 -> data)
        {
            temp2 = temp2 -> next;
        }
        if(temp == nullptr)
            return 0;
        temp = temp -> next;
        count++;
    }
    return count;
}