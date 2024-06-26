#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>
#include <stdexcept>

std::random_device rd;
std::mt19937 gen(rd());

int k_stat(std::vector<int>& vec, int l, int r, int key)
{
    while(l < r)
    {
        if (vec[l] < key) {++l;}
        else if (vec[r] >= key) {--r;}
        else
        {
            std::swap(vec[l], vec[r]);
            ++l;
        }
    }
    return r;
}

void b_sort(std::vector<int>& vec, int l, int r)
{
    for (int i = l; i <= r; ++i)
    {
        for (int j = i + 1; j <= r; ++j)
        {
            if (vec[i] > vec[j])
            {
                std::swap(vec[i], vec[j]);
            }
        }
    }
}

// quick_sort является одним из самых быстрых по ассимтотике алгоритмов сортировки ( O(n*log(n)) ).
// Единственной проблемой, по сравнению с merge_sort является ситуация, когда в качестве разделяющего элемента (pivot)
// часто попадаются крайние элементы в рассматриваемом массиве (подмассиве).
// Для решения этой проблемы pivot в данной реализации выбирается случайно - очень сложно спецально подобрать "плохой" массив для сортировки.
// Также в данной реализации при рассмотрении массива длиной 16 и меньше применяется сортировка пузырьком.
// Это сделано потому, что данная сортировка хорошо себя показывает на отсортированных или почти отсортированных массивах.
// Также отмечу, что данная реализация оптимизирована под наборы данных с большим колличеством повторяющихся чисел.
//
// В общем и целом, наилучшие результаты по времени выполнения демонстрируют алгоритмы сортировки, основанные на предпосчёте, поэтому
// для достижения более интересных результатов по данной задаче требуется больше информации о природе сортируемых чисел.
void q_sort(std::vector<int>& vec, int l, int r)
{
    if (r - l <= 16)
    {
        b_sort(vec, l, r);    
        return;
    }
    int key = vec[l + (gen() % (r-l))];
    int ind = k_stat(vec, l, r, key);
    q_sort(vec, l, ind - 1);
    while (ind < r && vec[ind] == key) { ++ind; }
    q_sort(vec, ind, r);
}
