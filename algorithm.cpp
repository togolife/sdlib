#include "stdafx.h"

static void swap(int *a, int *b) {
	int t = *a;
	*a = *b;
	*b = t;
}

// 冒泡排序
void bubble_sort(int *arr, int n) {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n - i - 1; ++j) {
			if (arr[j] > arr[j + 1]) {
				swap(arr + j, arr + j + 1);
			}
		}
	}
}
// 选择排序
void select_sort(int *arr, int n) {
	for (int i = 0; i < n; ++i) {
		int min = i;
		for (int j = i + 1; j < n; ++j) {
			if (arr[j] < arr[min]) {
				min = j;
			}
		}
		if (min != i) {
			swap(arr + i, arr + min);
		}
	}
}
// 插入排序
void insert_sort(int *arr, int n) {
	for (int i = 1; i < n; ++i) {
		int tmp = arr[i];
		int j = i - 1;
		while (j >= 0) {
			if (arr[j] > tmp) {
				arr[j + 1] = arr[j];
				--j;
			}
			else {
				break;
			}
		}
		arr[j + 1] = tmp;
	}
}
// 希尔排序
void shell_sort(int *arr, int n) {
	int i, j, k, p, tmp;
	for (k = n / 2; k > 0; k /= 2) {
		for (i = 0; i < k; ++i) {
			j = i + k;
			while (j < n) {
				tmp = arr[j];
				for (p = j - k; p >= 0; p -= k) {
					if (arr[p] > tmp) {
						arr[p + k] = arr[p];
					} else {
						break;
					}
				}
				arr[p + k] = tmp;
				j += k;
			}
		}
	}
}
// 堆排序
static void build_heap(int *arr, int n) {
	int i = 0;
	for (i = (n - 1) / 2; i >= 0; --i) {
		int pos = i;
		int left = pos * 2 + 1;
		int right = pos * 2 + 2;
		while (left < n) {
			int flag = 0;
			if (arr[pos] < arr[left]) {
				flag = 1;
			}
			if (right < n && arr[pos] < arr[right] && arr[left] < arr[right]) {
				flag = 2;
			}
			if (flag == 0) {
				break;
			} else if (flag == 1) {
				swap(arr + pos, arr + left);
				pos = left;
			} else {
				swap(arr + pos, arr + right);
				pos = right;
			}
			left = pos * 2 + 1;
			right = pos * 2 + 2;
		}
	}
}
static int delete_max(int *arr, int n) {
	int max = arr[0];
	int pos = 0;
	int left = pos * 2 + 1;
	int right = pos * 2 + 2;
	while (left < n - 1) {
		int child = left;
		if (right < n - 1 && arr[left] < arr[right]) {
			child = right;
		}
		if (arr[child] < arr[n - 1])
			break;
		else
			arr[pos] = arr[child];
		pos = child;
		left = pos * 2 + 1;
		right = pos * 2 + 2;
	}
	arr[pos] = arr[n - 1];
	return max;
}
void heap_sort(int *arr, int n) {
	build_heap(arr, n);
	int i = 0;
	for (; i < n; ++i) {
		int max = delete_max(arr, n - i);
		arr[n - i - 1] = max;
	}
}
// 归并排序
// 合并数组
static void merge(int *arr, int first, int middle, int end) {
	int *tmp = (int *)malloc((end - first) * sizeof(int));
	int i = first, j = middle, k = 0;
	while (i < middle && j < end) {
		if (arr[i] < arr[j]) {
			tmp[k++] = arr[i++];
		}
		else {
			tmp[k++] = arr[j++];
		}
	}
	while (i < middle) {
		tmp[k++] = arr[i++];
	}
	while (j < end) {
		tmp[k++] = arr[j++];
	}
	for (i = 0; i < end - first; ++i) {
		arr[first + i] = tmp[i];
	}
	free(tmp);
}
// 小数组排序 3个元素
static void insert_sort(int *arr, int begin, int end) {
	int i = 0;
	for (i = begin + 1; i < end; ++i) {
		int tmp = arr[i];
		int j = i - 1;
		for (; j >= begin; --j) {
			if (arr[j] > tmp) {
				arr[j + 1] = arr[j];
			}
			else {
				break;
			}
		}
		arr[j + 1] = tmp;
	}
}

void merge_sort(int *arr, int begin, int end) {
	if (begin + 3 > end) {
		insert_sort(arr, begin, end);
	}
	else {
		int middle = (begin + end) / 2;
		merge_sort(arr, begin, middle);
		merge_sort(arr, middle, end);
		merge(arr, begin, middle, end);
	}
}
// 快速排序
// media
static int media(int *arr, int begin, int end) {
	int middle = (begin + end) / 2;
	int pos = 0;
	if (arr[begin] > arr[middle]) {
		if (arr[middle] > arr[end - 1]) {
			pos = middle;
		}
		else if (arr[begin] > arr[end - 1]) {
			pos = end - 1;
		}
		else {
			pos = begin;
		}
	}
	else {
		if (arr[begin] > arr[end - 1]) {
			pos = begin;
		}
		else if (arr[middle] > arr[end - 1]) {
			pos = end - 1;
		}
		else {
			pos = middle;
		}
	}
	swap(arr + pos, arr + end - 1);
	return arr[end - 1];
}

void quick_sort(int *arr, int begin, int end) {
	if (begin + 3 > end) {
		insert_sort(arr, begin, end);
	}
	else {
		int num = media(arr, begin, end);
		int i = begin, j = end - 2;
		for (;;) {
			while (arr[i] < num) {
				++i;
			}
			while (arr[j] > num) {
				--j;
			}
			if (i > j) {
				break;
			}
			swap(arr + i, arr + j);
			++i;
			--j;
		}
		swap(arr + i, arr + end - 1);
		quick_sort(arr, begin, i + 1);
		quick_sort(arr, i + 1, end);
	}
}
// 快速选择
int quick_select(int *arr, int begin, int end, int pos) {
	if (begin + 3 > end) {
		insert_sort(arr, begin, end);
		return arr[pos + begin - 1];
	}
	else {
		int num = media(arr, begin, end);
		int i = begin, j = end - 2;
		for (;;) {
			while (arr[i] < num) {
				++i;
			}
			while (arr[j] > num) {
				--j;
			}
			if (i > j) {
				break;
			}
			swap(arr + i, arr + j);
			++i;
			--j;
		}
		swap(arr + i, arr + end - 1);
		if (i + 1 - begin > pos) {
			return quick_select(arr, begin, i + 1, pos);
		}
		else if (i + 1 - begin == pos) {
			return arr[i];
		}
		else {
			pos = pos - (i + 1 - begin);
			return quick_select(arr, i + 1, end, pos);
		}
	}
}
