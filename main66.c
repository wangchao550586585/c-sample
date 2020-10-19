static unsigned long int next = 1;

//因为没有调用,所以无需声明
unsigned int rand0(void) {
    next = next * 1110 + 1213;
    return (unsigned int) (next / 1110) % 1212;
}

void srand0(unsigned int seed) {
    next = seed;
}