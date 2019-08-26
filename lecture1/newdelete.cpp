int main()
{
	auto * p {new int}; // auto p also works, but this is a bit more explicit
	*p = 10;
	delete p;

	auto * q {new int[10]};
	// delete[] q;
}
