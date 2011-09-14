#include "SsJsonConfig.h"

using namespace std;
using namespace v8;

#include <iostream>
#include <fstream>

#include <key-common/app.h>

#include <json/reader.h>
#include <json/value.h>

key::SsJsonConfig::SsJsonConfig(std::string main_config_filename) : configuration_is_valid(false) {
	if (stringFromFile(main_configuration_contents, fullPathTo(main_config_filename))) {
		Json::Value root;
		Json::Reader reader;

		if (!reader.parse(main_configuration_contents, root))
		{
			cout << "could not PARSE configuration file!" << endl; 
			cout << reader.getFormatedErrorMessages() << endl;
		} 
		else 
		{
			// read config
			js_root_dir = root.get("js_root_dir", "js").asString();
			js_main_file = root.get("js_main_file", "main.js").asString();

			configuration_is_valid = true;
		}
	} else {
		std::cout << "Failed to read configuration from \"" << fullPathTo(main_config_filename) << "\"." << std::endl;
	}
}

key::SsJsonConfig::~SsJsonConfig() {

}

void key::SsJsonConfig::initCore(v8::Handle<v8::Object> & dest) {
	dest->Set(String::New("js_main_config_contents"), String::New(main_configuration_contents.c_str()));
	dest->Set(String::New("js_main_app_path"), String::New(getAppPath().c_str()));
	string s;
	s.append(1, PATH_SEPARATOR);
	dest->Set(String::New("js_main_app_path_separator"), String::New(s.c_str()));
}