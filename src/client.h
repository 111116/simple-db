#pragma once

#include "database.h"

std::map<std::string, Database*> dbList;

void drop(std::string dbName);
void create(std::string dbName);
void show(std::string dbName);

static Database* selected = nullptr;
