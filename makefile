all: autocomplete1 autocomplete2

autocomplete1: autocomplete.c autocomplete_main1.c
	gcc -Wall -o autocomplete1 autocomplete.c autocomplete_main1.c

autocomplete2: autocomplete.c autocomplete_main2.c
	gcc -Wall -o autocomplete2 autocomplete.c autocomplete_main2.c
