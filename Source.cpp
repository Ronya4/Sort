#include <iostream>		
#include <array>		
#include <random>		
#include <algorithm>	

static constexpr std::size_t n = 200;					
static constexpr std::size_t method_count = 8;			

#define ELEM_TYPE			short						
#define ARRAY_TYPE			std::array<ELEM_TYPE, n>	
#define ARRAY_REF			ARRAY_TYPE&					
#define ARRAY_CONST_REF		const ARRAY_REF				

#define ITER_TYPE			ARRAY_TYPE::iterator		

// forward	
// back		
// random	


void fill_arrays(ARRAY_REF forward, ARRAY_REF back, ARRAY_REF random) noexcept
{
	for (std::size_t i = 0; i < n; ++i)
	{
		forward[i] = i;
		back[i] = n - i - 1;
		do {
			random[i] = rand() % n;
		} while (std::find(random.data(), random.data() + i, random[i]) != random.data() + i);
	}
}


void print_arrays(ARRAY_CONST_REF forward,
	ARRAY_CONST_REF back, ARRAY_CONST_REF random) noexcept
{
	std::cout << "Forward array:\t";
	for (std::size_t i = 0; i < n; ++i)
		std::cout << forward[i] << "  ";
	std::cout << std::endl << std::endl;
	std::cout << "Back array:\t";
	for (std::size_t i = 0; i < n; ++i)
		std::cout << back[i] << "  ";
	std::cout << std::endl << std::endl;
	std::cout << "Random array:\t";
	for (std::size_t i = 0; i < n; ++i)
		std::cout << random[i] << "  ";
	std::cout << std::endl << std::endl;
}


bool default_pred(ELEM_TYPE a, ELEM_TYPE b) noexcept
{
	return a > b;
}

#define SORT_TYPE_OPTIONS ITER_TYPE first, ITER_TYPE last, bool(*pred)(ELEM_TYPE a, ELEM_TYPE b)
#define SORT_OPTIONS (SORT_TYPE_OPTIONS = default_pred)

static std::size_t comp_count = 0;	
static std::size_t perm_count = 0;	

void simple_exchange_sort SORT_OPTIONS noexcept
{
	for (std::size_t i = 0; i < n; ++i)	
	{
		for (ITER_TYPE j = first; j != last - 1; ++j) 
		{
			++comp_count;
			if (pred(*j, *(j + 1))) 
			{
				++perm_count;
				std::swap(*j, *(j + 1)); 
			}
		}
	}
}


void simple_choice_sort SORT_OPTIONS noexcept
{
	for (std::size_t i = 0; i < n; ++i) // Ïîâòîðåíèå n ðàç
	{
		ITER_TYPE iter_max = first;
		for (ITER_TYPE j = first + 1; j != last - i; ++j) 
		{
			++comp_count;
			if (*iter_max < *j) 
				iter_max = j;
		}
		++comp_count;
		if (pred(*iter_max, *(last - i - 1))) 
		{
			++perm_count;
			std::swap(*iter_max, *(last - i - 1)); 
		}
	}
}


void simple_insert_sort SORT_OPTIONS noexcept
{
	for (ITER_TYPE i = first + 1; i != last; ++i) 
	{
		for (ITER_TYPE j = i - 1; j != first; --j) 
		{
			++comp_count;
			if (pred(*j, *(j + 1)))
			{
				++perm_count;
				std::swap(*j, *(j + 1));
			}
			else
				break;
		}
		++comp_count;
		if (pred(*first, *(first + 1))) 
		{
			++perm_count;
			std::swap(*first, *(first + 1)); 
		}
	}
}


void shayker_sort SORT_OPTIONS noexcept
{
	while (first != last) 
	{
		for (ITER_TYPE i = first; i != last - 1; ++i) 
		{
			++comp_count;
			if (pred(*i, *(i + 1))) 
			{
				++perm_count;
				std::swap(*i, *(i + 1));
			}
		}
		last--;
		if (first == last) 
			break;
		for (ITER_TYPE i = last - 1; i != first; --i) 
		{
			++comp_count;
			if (pred(*(i - 1), *i)) 
			{
				++perm_count;
				std::swap(*(i - 1), *i); 
			}
		}
		first++;
	}
}


void log_descent(ITER_TYPE current, ITER_TYPE first, ITER_TYPE last,
	bool(*pred)(ELEM_TYPE a, ELEM_TYPE b)) noexcept
{
	auto i = current - first; 
	auto dist = last - first;
	if (i >= dist / 2)
		return;
	ITER_TYPE iter_max = first + 2 * i + 1; 
	++comp_count;
	if (iter_max + 1 != last && pred(*(iter_max + 1), *iter_max)) 
		++iter_max; 
	++comp_count;
	if (pred(*iter_max, *current)) 
	{
		++perm_count;
		std::swap(*iter_max, *current);
		log_descent(iter_max, first, last, pred); 
	}
}


void heap_sort SORT_OPTIONS noexcept
{
	for (ITER_TYPE::difference_type i = n / 2 - 1; i >= 0; --i)
		log_descent(first + i, first, last, pred); 
	for (std::size_t i = 0; i < n - 1; ++i) 
	{
		++perm_count;
		std::swap(*first, *(last - 1)); 
		--last;
		log_descent(first, first, last, pred); 
	}
}


void shell_sort SORT_OPTIONS noexcept
{
	for (std::size_t i = log2(n); i > 0; i = log2(i)) 
	{
		for (ITER_TYPE j = first + i;; j += i) 
		{
			for (ITER_TYPE k = j - i;; k -= i)
			{
				++comp_count;
				if (pred(*k, *(k + i))) 
				{
					++perm_count;
					std::swap(*k, *(k + i)); 
				}
				else
					break;
				if (k == first)
					break;
			}
			if (last - j <= i)
				break;
		}
	}
}


void binary_insert_sort SORT_OPTIONS noexcept
{
	for (ITER_TYPE i = first + 1; i != last; ++i) 
	{
		ITER_TYPE l = first; 
		ITER_TYPE r = i; 
		while (r != l) 
		{
			long long dist = r - l;
			ITER_TYPE half = l + dist / 2;
			++comp_count;
			if (pred(*half, *i))
				r = half;
			else
				l = half + 1;
		}
		for (ITER_TYPE j = i; j != l; --j)
		{
			++perm_count;
			std::swap(*(j - 1), *j); 
		}
	}
}


void quick_sort SORT_OPTIONS noexcept
{
	if (last - first <= 1) 
		return;
	++comp_count;
	if (*(last - 1) < *first)
	{
		++perm_count;
		std::swap(*(last - 1), *first); 
	}
	short pivot = *(last - 1); 
	ITER_TYPE wall = first; 
	bool state = false; 
	for (ITER_TYPE i = first + 1; i != last - 1; ++i)
	{
		++comp_count;
		if (state && pred(pivot, *i)) 
		{
			++perm_count;
			std::swap(*i, *(wall + 1)); 
			++wall;
		}
		else if (!state && pred(pivot, *i)) 
			++wall;
		else
			state = true;
	}
	quick_sort(first, wall + 1, pred); 
	quick_sort(wall + 1, last, pred);
}


enum sort_type
{
	SIMPLE_EXCHANGE = 0,
	SIMPLE_CHOICE = 1,
	SIMPLE_INSERT = 2,
	SHAYKER = 3,
	HEAP = 4,
	SHELL = 5,
	BINARY_INSERT = 6,
	QUICK = 7
};

typedef void(*sort_ptr)(SORT_TYPE_OPTIONS); 


template <sort_type type> void sort SORT_OPTIONS noexcept
{
	static sort_ptr sort_arr[method_count] = {
		&simple_exchange_sort, &simple_choice_sort, &simple_insert_sort,
		&shayker_sort, &heap_sort, &shell_sort, &binary_insert_sort, &quick_sort
	};
	sort_arr[static_cast<std::size_t>(type)](first, last, pred);
}

void main()
{
	ARRAY_TYPE forward, back, random;		
	std::size_t table[method_count][3][2];	

	
	fill_arrays(forward, back, random);
	std::cout << "BEFOR:" << std::endl;
	print_arrays(forward, back, random);
	comp_count = 0; perm_count = 0;
	sort<SIMPLE_EXCHANGE>(forward.begin(), forward.end());
	table[0][0][0] = comp_count;
	table[0][0][1] = perm_count;
	comp_count = 0; perm_count = 0;
	sort<SIMPLE_EXCHANGE>(back.begin(), back.end());
	table[0][1][0] = comp_count;
	table[0][1][1] = perm_count;
	comp_count = 0; perm_count = 0;
	sort<SIMPLE_EXCHANGE>(random.begin(), random.end());
	table[0][2][0] = comp_count;
	table[0][2][1] = perm_count;
	std::cout << "AFTER (SIMPLE EXCHANGE):" << std::endl;
	print_arrays(forward, back, random);

	
	fill_arrays(forward, back, random);
	std::cout << "BEFOR:" << std::endl;
	print_arrays(forward, back, random);
	comp_count = 0; perm_count = 0;
	sort<SIMPLE_CHOICE>(forward.begin(), forward.end());
	table[1][0][0] = comp_count;
	table[1][0][1] = perm_count;
	comp_count = 0; perm_count = 0;
	sort<SIMPLE_CHOICE>(back.begin(), back.end());
	table[1][1][0] = comp_count;
	table[1][1][1] = perm_count;
	comp_count = 0; perm_count = 0;
	sort<SIMPLE_CHOICE>(random.begin(), random.end());
	table[1][2][0] = comp_count;
	table[1][2][1] = perm_count;
	std::cout << "AFTER (SIMPLE CHOICE):" << std::endl;
	print_arrays(forward, back, random);

	fill_arrays(forward, back, random);
	std::cout << "BEFOR:" << std::endl;
	print_arrays(forward, back, random);
	comp_count = 0; perm_count = 0;
	sort<SIMPLE_INSERT>(forward.begin(), forward.end());
	table[2][0][0] = comp_count;
	table[2][0][1] = perm_count;
	comp_count = 0; perm_count = 0;
	sort<SIMPLE_INSERT>(back.begin(), back.end());
	table[2][1][0] = comp_count;
	table[2][1][1] = perm_count;
	comp_count = 0; perm_count = 0;
	sort<SIMPLE_INSERT>(random.begin(), random.end());
	table[2][2][0] = comp_count;
	table[2][2][1] = perm_count;
	std::cout << "AFTER (SIMPLE INSERT):" << std::endl;
	print_arrays(forward, back, random);

	fill_arrays(forward, back, random);
	std::cout << "BEFOR:" << std::endl;
	print_arrays(forward, back, random);
	comp_count = 0; perm_count = 0;
	sort<SHAYKER>(forward.begin(), forward.end());
	table[3][0][0] = comp_count;
	table[3][0][1] = perm_count;
	comp_count = 0; perm_count = 0;
	sort<SHAYKER>(back.begin(), back.end());
	table[3][1][0] = comp_count;
	table[3][1][1] = perm_count;
	comp_count = 0; perm_count = 0;
	sort<SHAYKER>(random.begin(), random.end());
	table[3][2][0] = comp_count;
	table[3][2][1] = perm_count;
	std::cout << "AFTER (SHAYKER):" << std::endl;
	print_arrays(forward, back, random);

	fill_arrays(forward, back, random);
	std::cout << "BEFOR:" << std::endl;
	print_arrays(forward, back, random);
	comp_count = 0; perm_count = 0;
	sort<HEAP>(forward.begin(), forward.end());
	table[4][0][0] = comp_count;
	table[4][0][1] = perm_count;
	comp_count = 0; perm_count = 0;
	sort<HEAP>(back.begin(), back.end());
	table[4][1][0] = comp_count;
	table[4][1][1] = perm_count;
	comp_count = 0; perm_count = 0;
	sort<HEAP>(random.begin(), random.end());
	table[4][2][0] = comp_count;
	table[4][2][1] = perm_count;
	std::cout << "AFTER (HEAP):" << std::endl;
	print_arrays(forward, back, random);

	fill_arrays(forward, back, random);
	std::cout << "BEFOR:" << std::endl;
	print_arrays(forward, back, random);
	comp_count = 0; perm_count = 0;
	sort<SHELL>(forward.begin(), forward.end());
	table[5][0][0] = comp_count;
	table[5][0][1] = perm_count;
	comp_count = 0; perm_count = 0;
	sort<SHELL>(back.begin(), back.end());
	table[5][1][0] = comp_count;
	table[5][1][1] = perm_count;
	comp_count = 0; perm_count = 0;
	sort<SHELL>(random.begin(), random.end());
	table[5][2][0] = comp_count;
	table[5][2][1] = perm_count;
	std::cout << "AFTER (SHELL):" << std::endl;
	print_arrays(forward, back, random);

	fill_arrays(forward, back, random);
	std::cout << "BEFOR:" << std::endl;
	print_arrays(forward, back, random);
	comp_count = 0; perm_count = 0;
	sort<BINARY_INSERT>(forward.begin(), forward.end());
	table[6][0][0] = comp_count;
	table[6][0][1] = perm_count;
	comp_count = 0; perm_count = 0;
	sort<BINARY_INSERT>(back.begin(), back.end());
	table[6][1][0] = comp_count;
	table[6][1][1] = perm_count;
	comp_count = 0; perm_count = 0;
	sort<BINARY_INSERT>(random.begin(), random.end());
	table[6][2][0] = comp_count;
	table[6][2][1] = perm_count;
	std::cout << "AFTER (BINARY INSERT):" << std::endl;
	print_arrays(forward, back, random);

	fill_arrays(forward, back, random);
	std::cout << "BEFOR:" << std::endl;
	print_arrays(forward, back, random);
	comp_count = 0; perm_count = 0;
	sort<QUICK>(forward.begin(), forward.end());
	table[7][0][0] = comp_count;
	table[7][0][1] = perm_count;
	comp_count = 0; perm_count = 0;
	sort<QUICK>(back.begin(), back.end());
	table[7][1][0] = comp_count;
	table[7][1][1] = perm_count;
	comp_count = 0; perm_count = 0;
	sort<QUICK>(random.begin(), random.end());
	table[7][2][0] = comp_count;
	table[7][2][1] = perm_count;
	std::cout << "AFTER (QUICK):" << std::endl;
	print_arrays(forward, back, random);

	const char* methods[method_count] = {
		"SIMPLE EXCHANGE\t", "SIMPLE CHOICE\t", "SIMPLE INSERT\t",
		"SHAYKER\t\t", "HEAP\t\t", "SHELL\t\t", "BINARY INSERT\t", "QUICK\t\t"
	};

	const char* array_types[3] = {
		"FORWARD\t", "BACK\t\t", "RANDOM\t\t"
	};

	const char* result_types[2] = {
		"COMPARISION", "PERMUTATION"
	};

	std::cout << std::endl
		<< "=========================================================================================" << std::endl;
	for (std::size_t i = 0; i < method_count; ++i)
	{
		for (std::size_t j = 0; j < 3; ++j)
		{
			std::cout << "METHOD: " << methods[i] << "  "
				<< "ARRAY: " << array_types[j] << "  ";
			for (std::size_t k = 0; k < 2; ++k)
				std::cout << result_types[k] << ":  " << table[i][j][k] << "\t";
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	std::cout
		<< "=========================================================================================" << std::endl;
}
