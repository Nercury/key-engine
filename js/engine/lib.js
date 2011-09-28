function Js() 
{
	this.config = JSON.parse(js_main_config_contents);
	this.app = {
		nativePath : js_main_app_path,
		pathSeparator : js_main_app_path_separator
	};
}
Js.prototype.log = function(o) {
	if (typeof o == 'undefined')
		js_main.textLog('[undefined]');
	else if (o === null)
		js_main.textLog('[null]');
	else if (typeof o === 'string' || typeof o === 'integer')
		js_main.textLog(o);
	else
		js_main.textLog(JSON.stringify(o));
}
Js.prototype.script = function(filename) {
	var execute_result = js_main.executeFile(filename);
	if (execute_result === true) {
		return true;
	} else {
		js_main.textLog(execute_result);
		js_main.textLog(js_main_log_separator);
		return false;
	}
}
Js.prototype.man = function(obj) {
	var fields = obj.__fields;
	if (!fields)
		return "Object has no documentation";
	var str = '';
	for (var i = 0; i < fields.length; i++) {
		var info = obj[fields[i]];
		str += "\n" + info.type + ' ' + fields[i] + ' (' + (info.read_only == '1' ? 'get' : 'get, set') + ')';
		str += "\n" + info.doc + "\n";
	}
	return str;
}
Js.prototype.help = function(obj) {
	return Js.prototype.log(Js.prototype.man(obj));
}
js = new Js();