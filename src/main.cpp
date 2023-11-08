#include "person.h"
#include "text-interface.h" // includes stdio.h, vector, string

//------------------------------------------------------------------------------

// TODO: as the scope of init() grows, add real error codes in place of true/false
bool init() {
	srand(time(NULL));

	return true;
}

//------------------------------------------------------------------------------

int main(void) {
	bool init_success = init();

	if(!init_success) {
		printf("ERROR: init() failed!\n");
		return 1;
	}

	start_screen();

	Person p = create_random_person();

	print_person(p);

	printf("\n");

	print_person(p, RIGHT);

	printf("\n");

	print_person(p, CENTER);

	return 0;
}
