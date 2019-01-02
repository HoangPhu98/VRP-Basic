
void swap(int* number_1, int* number_2)
{
	int temp = *number_1;
	*number_1 = *number_2;
	*number_2 = temp;
}

void shuffleArray(int* arr, int n)
{

	int minPosition;
	int maxPosition = n;
	int swapPosition;

	int i = 1;
	while (i < n - 1)
	{
		minPosition = i + 1;
		swapPosition = rand() % (maxPosition - minPosition) + minPosition;
		swap(&arr[i], &arr[swapPosition]);
		i++;
	}
}