#include "person.h"
#include "text-interface.h" // includes stdio.h, vector, string

#include <toml.hpp>

//------------------------------------------------------------------------------

typedef struct {
	int error; // 0 if everything has gone swimmingly

	// config.toml options
	int start_screen_load_time;
} init_return_t;

// init_return_t.error states
enum {
	INIT_SUCCESS = 0,
	INIT_CONFIG_OPEN_FAILURE, // probably never used because toml.hpp has its own errors
	INIT_CONFIG_READ_FAILURE
};

// TODO: as the scope of init() grows, add real error codes in place of true/false
init_return_t init() {
	srand(time(NULL));

	std::string config_filepath = "./config/config.toml"; // only using std::string because toml.hpp requires... me no likey...

	const toml::value root = toml::parse(config_filepath);

	int start_screen_load_time = toml::find<int>(root, "engine", "start_screen_load_time");

	init_return_t output = (init_return_t) {
		INIT_SUCCESS,

		start_screen_load_time
	};

	return output;
}

//------------------------------------------------------------------------------

// user commands
enum {
	NOOP = 0,
	QUIT,
	PROCEED,
	INFO
};

int main(void) {
	init_return_t init_output = init();

	if(init_output.error != INIT_SUCCESS) {
		printf("ERROR: init() error! Consult the manual with the following error code: [ " BOLD_ON "%d" BOLD_OFF " ] for more info!", init_output.error);
		return init_output.error;
	}

	start_screen(init_output.start_screen_load_time);

	bool game_should_quit = false;
	bool game_should_proceed = false;

	int day_counter = 0;

	Person player = interactive_create_person();

	while(!game_should_quit) {
		day_counter++;

		game_should_proceed = false;

		clear();

		print_daily_heading(day_counter, player);

			while(!game_should_proceed) {
			//------------------------------------------------------------------------------

			prompt();

			input_t command = input();

			//------------------------------------------------------------------------------

			int command_id = NOOP; // default
			if(strcmp(command.input, "shutdown") == 0) command_id = QUIT;
			else if(strcmp(command.input, "proceed") == 0) command_id = PROCEED;
			else if(strcmp(command.input, "info") == 0 || strcmp(command.input, "playerinfo") == 0) command_id = INFO;

			// printf("COMMAND RECIEVED: %s, LENGTH: %d, OPCODE: %d\n", command.input, command.length), command_id);

			//------------------------------------------------------------------------------

			switch(command_id) {
				case QUIT:
					clear();
					printf("Shutting down...\n");
					// TODO: save procedures
					printf("Goodbye!\n");
					exit(0);
				case PROCEED:
					game_should_proceed = true;
					break;
				case INFO:
					print_person(player);
					break;
				default: continue;
			}
		}
	}

	return 0;
}
