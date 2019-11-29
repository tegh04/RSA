#include<stdio.h> /* printf, scanf*/
#include<math.h> /* pow, floor, log10*/
#include<string.h> /* strlen */


struct character
{
    /* Resultant from public key encryption */
    unsigned char public_encrypt[256];

    /* Resultant from private key encryption */
    unsigned char private_encrypt[256];
};

/*******************************************************************************
* List of functions
*
*******************************************************************************/
/* List prime and non-prime numbers */
void square_root(void);

 /* Finds the GCD (Greatest Common Divisor) */
int euclid(int num1, int num2);

/* Carries out modular Exponentiation */
int euler_theorem(int base, int modulus, int power);

/* Finds the number of possibles values for k such (GCD(n, k)=1, k<n */
int euler_totient(int input);

/* Finds the multiplicative modulo inverse */
int extended(int num1, int num2);

 /* For determining the security of choosen_value */
int hamming_weight(int num);


int coprimality_copairwise(int size);

/* Store number into an array written in binary */
void binarise_num(int num, int* array);

void reverse(int* array);

int digit_len(int num, int base);


int main(void)
{

    /**************************************************************************
    *   User based variables
    ***************************************************************************/
	int prime1, prime2;

	unsigned long int prime_product, totient_product;
    /* prime1*prime2, (prime1-1)*(prime2-1) */

	int chosen_value; /* Choose value for public key */

	int inverse; /* Multiplicative modulo inverse */


	unsigned char string[30];
	 /* For taking input from the user for encryption/decryption */

	unsigned char ciphertext[30]; /* The string that is encrypted*/


	square_root(); /*Finds prime numbers */

	printf("Please enter two numbers from the list of numbers"
           " that are prime.>>>\n");

	scanf("%d", &prime1);
	scanf("%d", &prime2);


	/*totient_product = (prime1-1) * (prime2-1); */
	/* possible addable line ^ if prime stored in memory*/


	/* checks whether prime1 and prime2 are actually prime.*/
	if(euler_totient(prime1)==(prime1-1) && euler_totient(prime2)==(prime2-1))
	{
		totient_product = (prime1-1) * (prime2-1);
	}

	else
	{
		printf("\nThe numbers or one of the numbers you "
         "have enter is not prime, please try again.\n");
		return 1;
	}

	prime_product = prime1*prime2;


	do
	{
	    int num;

	    do
        {


            printf("\nEnter a number to find a hamming weight. "
                   "Enter a non-positive number to not run the program\n");
            scanf("%d", &num);

            if(num > 0)
            {
                printf("\nThe hamming weight is: %d\n", hamming_weight(num));
            }

        }
        while(num > 0);


	    printf
	    ("\nThe totient product of the two prime numbers is %lu.\n"
        "Please enter a value such that the product and value enter\n"
        "are comprime.\n ", totient_product);

		scanf("%d", &chosen_value);

	}
	while(euclid(chosen_value, totient_product) != 1 &&
       chosen_value<prime_product);



	printf("\n The public key is (%d, %d).\n", prime_product, chosen_value);

	inverse = extended(chosen_value, totient_product);

	printf("\nThe private key is (%d, %d).\n", prime_product, inverse);


	/* digit_len(chosen_value, 2) find number bit required to represent number
	   number in binary*/
    int binary_chosen_value[digit_len(chosen_value, 2)];


    int binary_inverse[digit_len(inverse, 2)];


    binarise_num(chosen_value, binary_chosen_value);

    binarise_num(inverse, binary_inverse);




    struct character ASCII;

    /* rows are ASCII character, column are power used in modulo
    exponentiation, Exponents are 1, 2, 4, 8 */
    int public_exponentiation[256][digit_len(chosen_value, 2)];

    int private_exponentiation[256][digit_len(inverse, 2)];

    int i, j;

    for(i=0; i<=255; i++)
    {
        public_exponentiation[i][0] = (  (int)pow(i, 1) )%prime_product;

        for(j=1; j<=digit_len(chosen_value, 2)-1; j++)
        {
            /* Apply modulo exponentiation by squaring*/
            public_exponentiation[i][j] =

            ( (public_exponentiation[i][j-1] % prime_product)*
            (public_exponentiation[i][j-1] % prime_product) )
            % prime_product;


        }
    }

    reverse(binary_chosen_value); /* ****** Arrays are reversed */
    reverse(binary_inverse);/* ***** Arrays are reversed */


    /*The exponent product array */
    int product_array[hamming_weight(chosen_value)];


    long int exponent_product;

    exponent_product = 1;

    int index_array[hamming_weight(chosen_value)];

    int k;

    i = 0; /* index of index_array below */

    for(j=0; j<=digit_len(chosen_value, 2)-1; j++)
    {
        if(binary_chosen_value[j]==1)
        {
            index_array[i] = j;
            i = i + 1;
        }
    }

    for(j=0; j<=digit_len(chosen_value, 2)-1; j++)
    {

        for(i=0; i<=255; i++)
        {

            for(k=0; k<=hamming_weight(chosen_value)-1; k++)
            {

                product_array[k] = public_exponentiation[i][index_array[k]];

                exponent_product = exponent_product * product_array[k];

                if( exponent_product >= prime_product )
                {
                    exponent_product = exponent_product % prime_product;
                }

            }

            ASCII.public_encrypt[i] = (unsigned char)exponent_product;


        }


    }



	printf("\nEnter the plain text you would like to encrypt using the"
        " public key.\n ");

	fgets(string, 30, stdin);

    unsigned int num;

    for(i=0; i<=(strlen(string)-1); i++)
    {
        num = (unsigned int)string[i];

        printf("%c", ASCII.public_encrypt[num]);
    }

//Might use code below later
/*
	for(string_counter=0; string_counter<=(strlen(string)-1); ++string_counter)
	{
		ciphertext[string_counter] = euler_theorem(string[string_counter],
                                             prime_product, chosen_value);
	}

	for(string_counter=0; string_counter<=(strlen(string)-1); ++string_counter)
	{
		printf("%d", ciphertext[string_counter]);
	}
*/
	return 0;
}


/******************************************************************************
*For modulo exponentiation
*Inputs: inputs character of string(base),
*        power(depends on private or public key)
*        modulus: the value of the product of the two prime number
*Outputs: A remainder which will be converted to an ASCII character
*Author: Tegh
******************************************************************************/
int euler_theorem(int base, int modulus, int power)
{

	int coprime_output, quotient, power_remainder, remainder;

	if(euclid(base, modulus)==1)
	{
		coprime_output = euler_totient(power);

		quotient = power/coprime_output;

		power_remainder = power - quotient*coprime_output;

		remainder = pow(base, power_remainder);   /* % modulus */

		remainder = remainder % modulus;

		printf("\nThe remainder is %d\n", remainder);

		return remainder;
	}

}


/******************************************************************************
*For Euler theorem
*Inputs: num
*
*Outputs: number values of k such GCD(n, k) = 1
*Author: Tegh
******************************************************************************/
int euler_totient(int input)
{
	int counter, checker;
	counter = 0;

	for(checker=1 ; checker<=input; ++checker)
	{
		if(euclid(input, checker) ==1)
		{
			counter= counter +1;
		}
	}

	printf("%d", counter);

	return counter;
}

/******************************************************************************
*Finds GCD(Greatest Common Divisor)
*Inputs two numbers
*Outputs GCD
*Author: Tegh
******************************************************************************/

int euclid(int num1, int num2)
{

	while((num1 != 0) && (num2 != 0 ) )
	{
		printf("\nExecuting euclid\n");

		if(num1>= num2 && num2!=0 )
		{
			num1 = num1% num2;
		}
		if(num2>= num1 && num1!=0)
		{
			num2 = num2%num1;
		}

		printf("\n num1:%d \t num2:%d \n", num1, num2);

	}

	if (num2==0 )

	{
		return num1;
	}

	if(num1==0)
	{
		return num2;
	}
}

/******************************************************************************
*Finds modulo inverse using extended euclidean algorithm
i.e. finds value of d such that a*d % b = 1
*Inputs: choosen_value and euler_totient(prime1*prime2)
*Outputs: modulo inverse
*Author: Tegh
******************************************************************************/

int extended(int num1, int num2)
{
	int counter;
	counter =1;

	if(euclid(num1, num2)!=1)
	{
		printf("\nThe multiplicative modulo inverse \n"
         "doesn`t exist as GCD doesn`t equal 1.\n");
		return 1;
	}


	while(num1*counter % num2 != 1)
	{
		counter=counter+1;
	}

	printf("\nThe multiplicative modulo inverse is %d for %d\n",
        counter, num1);

	return counter;
}

/******************************************************************************
*Finds primes number via square root
*Takes user input to determine up to which number to calculate primes
*No outputs
*
******************************************************************************/

void square_root(void)
{
	int num, counter, divisor;

	do
	{
		printf("Please enter the value up to which you would like to check are "
        "prime numbers.\nPlease note a value greater than 255 "
        "must be entered.>>>\n");
		scanf("%d", &num);
	}
	while(num<255);

	for(counter=2; counter<=num; ++counter)
	{
		divisor = floor(sqrt(counter));

		while(divisor!=1 && counter%divisor!=0)
		{
			divisor--;
		}

		printf("%d", divisor);

		if(counter%divisor==0 && divisor!=1)
		{
			printf("\n %d not prime. \n", counter);
		}

		else
		{
			printf("\n %d is prime \n", counter);
		}

	}
}
/******************************************************************************
*For improving security based on selection of chosen_value
*Takes user input,
*Outputs weight(The number of one bits in a number)
*
*Author: Tegh
******************************************************************************/
int hamming_weight(int num)
{

    int quotient, weight, remainder;

    weight = 0;

    while(num != 0)
    {

        remainder = num%2;


        num = num/2;

        if(remainder == 1)
        {
            weight = weight+1;
        }


    }


    return weight;
}


/******************************************************************************
* Check copairwise coprimality
* Inputs: size of array
* Outputs: If 0 => copairwise coprime else 1(error)
* Author: Tegh
******************************************************************************/


int coprimality_copairwise(int size)
{
    printf("Enter the amounts of numbers you would like to check\n"
           " are copairwise prime>>>\n");
    scanf("%d ", &size);

    int number[size+1];


    int counter1, counter2;

    for(counter1 =0; counter1<=size-1; counter1++)
    {

        printf("\nEnter a number\n");

        scanf("%d ", &number[counter1]);
    }



    for(counter1=0; counter1<=size-1; counter1++)
    {
        for(counter2 =0; counter2<=size-1; counter2++)
        {

            if(euclid(number[counter1], number[counter2])> 1)
            {
                return 1;
            }

        }
    }

    return 0;

}


/******************************************************************************
* Use with binarise num to binarise a number.
* Inputs: Array
* Outputs: Array with reverse Elements
* Author: Tegh
******************************************************************************/
void reverse(int *array)
{
    int i;

    int num_elements;

    num_elements = sizeof(*array)/sizeof(int);


    int temp_var;

    for(i=0; i<=(int)num_elements/2 - 1; ++i)
    {
        temp_var = *(array+i);

        *(array+i) = *(array+num_elements-1-i);

        *(array+num_elements-1-i) = temp_var;
    }
}



/******************************************************************************
*Finds length of a number based on its base
*Inputs: Number, Base
*Outputs: Length of a number
*Author: Tegh
******************************************************************************/
int digit_len(int num, int base)
{
    int num_digits;

    num_digits = floor(log10(num)/log10(base))+1;

    return num_digits;

}


/******************************************************************************
*
*Inputs: Number, Base
*Outputs: Length of a number
*Author: Tegh
******************************************************************************/
void binarise_num(int num, int *array)
{
    int quotient, remainder;

    int i;

    int num_digits;


    num_digits = digit_len(num, 2);

    for(i=0; i<= num_digits-1 ; ++i)
    {
        remainder = num%2;

        *(array+i) = remainder;

        num = num/2;

    }


    reverse(array);
}
