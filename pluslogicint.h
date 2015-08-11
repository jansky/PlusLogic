typedef struct PLIFunction
{
	std::string name;
	std::vector<PLCalcToken> tokens;
	std::vector<char> variable_names;
} PLIFunction;

void replaceAll(std::string& str, const std::string& from, const std::string& to);
long double evaluateInput(std::string input, long double ans, bool exitonexception);
void evaluateCmdInput(int argc, char **argv);
void exceptionresult(PLException err, bool exitonexception);



PLIFunction pli_create_function(std::string name, std::vector<PLCalcToken> tokens, std::vector<char> variable_names);
int pli_get_function_id_by_name(std::string name);
long double pli_evaluate_function(PLIFunction function, std::string args);

bool pli_save_vars_funcs_to_file(std::string fname);
long double pli_load_vars_funcs_from_file(std::string fname, long double ans,bool exitonexception=false);
void pli_report_logic_mode();
bool pli_file_exists(std::string fname);

void pli_truth_table_gen(std::vector<std::string> var_names, std::vector<std::string> formulas, std::vector<PLVariable> variables, PLLogicMode mode);
