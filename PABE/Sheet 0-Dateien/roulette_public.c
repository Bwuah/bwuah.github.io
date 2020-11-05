#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ROUNDS 	  16
#define MAX_NUMBER 	  36
#define WIN_SCORE 	1024
// the flag has been redacted... the real flag is only available from the remote service
#define FLAG "FLAG{REDACTED}"

struct gamestate {
	unsigned int score;
	char name[9];
	unsigned char numbers[MAX_ROUNDS];
};

void get_name(struct gamestate* state)
{
	printf("Please enter a name (max. 8 chars): ");
	scanf("%s", &(state->name));
	printf("\nWelcome %s!\n", state->name);
}

void init_game(struct gamestate* state)
{
	state->score = 8;
	get_name(state);
	srand(time(NULL));
	for (unsigned char i = 0; i < MAX_ROUNDS; i++)
		state->numbers[i] = rand() % MAX_NUMBER;
}

char* get_color(unsigned char number)
{
	if ((number >= 1 && number <= 10) || (number >= 19 && number <= 28))
		if (number % 2)
			return "black";
		return "red";
	if ((number >= 11 && number <= 18) || (number >= 29 && number <= 36))
		if (number % 2)
			return "red";
		return "black";
	return "green";
}

char* get_odd_or_even(unsigned char number)
{
	if(number == 0)
		return "green";
	if(number % 2)
		return "odd";
	return "even";
}

void print_options_main_menu(unsigned int score, unsigned int i)
{
	printf("You have %d jetons. Round %d is about to start.\n", score, i+1);
	printf("1 - bet\n");
	printf("2 - options\n");
	printf("3 - quit\n");
}

void print_numbers(unsigned char number)
{
	unsigned char a, b, c, d;
	do{
		a = rand() % MAX_NUMBER;
		b = rand() % MAX_NUMBER;
		c = rand() % MAX_NUMBER;
		d = rand() % MAX_NUMBER;
	}	while(a == number || b == number || c == number || d == number || a == b || a == c || a == d || b == c || b == d || c == d);

	// Draw Animation
	sleep(1);
	printf("...| %u | %u | %u |...\n          O\n\n", (rand()%MAX_NUMBER), (rand()%MAX_NUMBER), (rand()%MAX_NUMBER));
	sleep(1);
	printf("...| %u | %u | %u |...\n          O\n\n", (rand()%MAX_NUMBER), (rand()%MAX_NUMBER), (rand()%MAX_NUMBER));
	sleep(1);
	printf("...| %u | %u | %u |...\n          O\n\n", a, b, c);
	sleep(1);
	printf("...| %u | %u | %u |...\n          O\n\n", b, c, number);
	sleep(2);
	printf("...| %u | %u | %u |...\n          O\n\n\n", c, number, d);
	sleep(1);
}

int calculate_win(char** placement, unsigned int bet, unsigned char number, char* color, char* oddOrEven)
{
	char string_placement[10], string_number[10], string_color[10], string_bet[10], string_oddOrEven[10];
	sprintf(string_placement, "%s", placement);
	sprintf(string_number, "%u", number);
	sprintf(string_color, "%s", color);
	sprintf(string_bet, "%u", bet);
	sprintf(string_oddOrEven, "%s", oddOrEven);

	printf("%s %s %s %s %s\n", string_placement, string_number, string_color, string_bet, string_oddOrEven);

	if(!strcmp(string_placement, string_color) || !strcmp(string_placement, string_oddOrEven))
	{
		printf("You won %d Jetons!\n\n", bet*2);
		return bet*2;
	}

	if(!strcmp(string_placement, string_number))
	{
		printf("You won %d Jetons!\n\n", bet*36);
		return bet*36;
	}

	printf("The house won.\n");
	return -bet;
}

void gameloop()
{
	unsigned int i = 0;
	unsigned int option;
	struct gamestate state;

	init_game(&state);

	while (i < MAX_ROUNDS)
	{
		print_options_main_menu(state.score, i);
		scanf("%u", &option);

		switch (option)
		{
			case 1:
			{
				char* placement[10];
				unsigned int bet;
				printf("Faites vos jeux!\nOn what do you want to bet? [0-%u, red|black, even|odd]\n", MAX_NUMBER);
				scanf("%9s", &placement);
				printf("How much would you like to bet? [1-%u]\n", state.score);
				scanf("%u", &bet);
				if (bet > state.score)
				{
					printf("invalid bet!\n");
					continue;
				}
				printf("Rien ne va plus!\n\n");
				print_numbers(state.numbers[i]);
				printf("... its a %u (%s / %s)!\n", state.numbers[i], get_color(state.numbers[i]), get_odd_or_even(state.numbers[i]));
				state.score = state.score + calculate_win(placement, bet, state.numbers[i], get_color(state.numbers[i]), get_odd_or_even(state.numbers[i]));

				if (!state.score) {
					printf("We got to have to ask you to leave.\n");
					exit(0);
				}
				if (state.score >= WIN_SCORE) {
					printf("You won!\n");
					printf(FLAG);
					return;
				}
				i++;
				break;
			}
			case 2:
				printf("Welcome to the options menu. What would you like to do?\n");
				printf("1 - rules\n");
				printf("2 - set name\n");
				printf("3 - ask for credit\n");

				scanf("%u", &option);
				switch (option)
				{
					case 1:
						printf("Visit: http://lmgtfy.com/?q=Roulette+Rules\n");
						break;
					case 2:
						get_name(&state);
						break;
					case 3:
						if(state.score < 5){
								state.score = 5;
								printf("We grant you some more Jetons.\n");
								printf("The house always pays its debts. Will you?\n\n");
						} else{
							printf("You have enough Jetons to play.\n\n");
						}
						break;
					default:
						break;
				}
				break;
			default:
				continue;
		}
		if(option == 3)
		{
			printf("Goodbye, %s!\n\n", state.name);
			break;
		}
	}
}

int main(int argc, char* argv[])
{
	setvbuf(stdout, NULL, _IONBF, 0);
	gameloop();
	return EXIT_SUCCESS;
}
