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

int main(void) {
	init_return_t init_output = init();

	if(init_output.error != INIT_SUCCESS) {
		printf("ERROR: init() error! Consult the manual with the following error code: [ " BOLD_ON "%d" BOLD_OFF " ] for more info!", init_output.error);
		return init_output.error;
	}

	start_screen(init_output.start_screen_load_time);

	return 0;
}
