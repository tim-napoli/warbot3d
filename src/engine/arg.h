/* arg.h
 *  Functions to manage command line arguments.
 */
 
void arg_init (int argc, char** argv);

void arg_set_value (const char* name, const char* value);

const char* arg_get_value (const char* name);

int arg_get_value_i (const char* name);
