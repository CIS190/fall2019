void crash(int depth)
{
	int * p = nullptr;

	if (depth == 10)
		depth = *p;
	else
		crash(depth + 1);
}

int main()
{
	int a = 0;

	for (auto i {0}; i < 10; ++i)
		a++;

	crash(0);
}
