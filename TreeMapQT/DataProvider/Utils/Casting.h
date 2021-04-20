#pragma once

#include <string>

bool is_int(const std::string& str);
bool is_float(const std::string& str);
bool is_double(const std::string& str);

bool is_int(const std::string& str, int& value);
bool is_float(const std::string& str, float& value);
bool is_double(const std::string& str, double& value);

double to_double(const std::string& str);
int to_int(const std::string& str);

std::string lower(const std::string& name);