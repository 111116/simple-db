
std::map<std::string, Database> dblist;

void drop(std::string dbName);
void create(std::string dbName);
void show(std::string dbName);

static Database* selected = NULL;

static void interact();