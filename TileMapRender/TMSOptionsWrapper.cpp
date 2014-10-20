#include "StdAfx.h"
#include "TMSOptionsWrapper.h"


TMSOptionsWrapper::TMSOptionsWrapper(const TileSourceOptionsWrapper& opt)
: TileSourceOptionsWrapper(opt)
{
	setDriver("tms");
	fromConfig(_conf);
}

TMSOptionsWrapper::TMSOptionsWrapper(const std::string& inUrl) :
TileSourceOptionsWrapper()
{
	setDriver("tms");
	fromConfig(_conf);
	_url = inUrl;
}
Config TMSOptionsWrapper::getConfig() const {
	Config conf = TileSourceOptions::getConfig();
#ifdef _LOG_CALLS_
	LOG("TMSOptionsWrapper::getConfig"); LOG("url"); LOG(conf.key().c_str()); LOG("\n");
#endif
	conf.updateIfSet("url", _url);
	conf.updateIfSet("tms_type", _tmsType);
	conf.updateIfSet("format", _format);
	return conf;
}


void TMSOptionsWrapper::mergeConfig(const Config& conf) {
#ifdef _LOG_CALLS_
	LOG("TMSOptionsWrapper::mergeConfig"); LOG("url"); LOG(conf.key().c_str()); LOG("\n");
#endif
	TileSourceOptions::mergeConfig(conf);
	fromConfig(conf);
}
void TMSOptionsWrapper::fromConfig(const Config& conf) {
#ifdef _LOG_CALLS_
	LOG("TMSOptionsWrapper::fromConfig"); LOG("url"); LOG(conf.key().c_str()); LOG("\n");
#endif
	conf.getIfSet("url", _url);
	conf.getIfSet("format", _format);
	conf.getIfSet("tms_type", _tmsType);
}
