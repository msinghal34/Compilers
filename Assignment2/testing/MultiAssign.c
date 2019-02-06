
int g, h;
int i;

void main()

{
	int a, b, c;
	a = 3;		// variable 'a' gets register, say, v0
	b = a;		// variable 'b' gets register v0 also
	b = 4;		// variable 'b' should get a new register, 
			// say, v1 having value 4
	c = a;		// variable 'c' will have register v0

	g = 3;
	h = g;
	g = 5;
	i = h;

	c = i;
}
