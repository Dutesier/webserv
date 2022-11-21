#include "Config.hpp"

Config::Config()
    : port(80), address("localhost"), root("/var/www/html"), server(false),
      location(false) {}