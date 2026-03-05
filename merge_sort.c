void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    
    int left[100], right[100];
    
    int i = 0;
    while (i < n1) {
        left[i] = arr[l + i];
        i++;
    }
    
    int j = 0;
    while (j < n2) {
        right[j] = arr[m + 1 + j];
        j++;
    }
    
    int k = l;
    i = 0;
    j = 0;
    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}
