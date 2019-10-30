constexpr long int collatz(long int i)
{
    if (i == 1)
        return 0;
    if (i % 2 == 0)
        return collatz(i / 2) + 1;

    return collatz(3 * i + 1) + 1;
}

int main()
{
    constexpr long int i = collatz(63728127);
}
