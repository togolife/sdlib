#pragma once

#ifndef SDLIB_ALGORITHM_H
#define SDLIB_ALGORITHM_H

void bubble_sort(int *arr, int n);
void select_sort(int *arr, int n);
void insert_sort(int *arr, int n);
void shell_sort(int *arr, int n);
void heap_sort(int *arr, int n);
void merge_sort(int *arr, int begin, int end);
void quick_sort(int *arr, int begin, int end);
int quick_select(int *arr, int begin, int end, int pos);

#endif