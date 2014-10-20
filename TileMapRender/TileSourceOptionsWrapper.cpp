#include "StdAfx.h"
#include "TileSourceOptionsWrapper.h"


TileSourceOptionsWrapper::TileSourceOptionsWrapper(const ConfigOptions& options)
:TileSourceOptions(options)
{
}


TileSourceOptionsWrapper::~TileSourceOptionsWrapper()
{
}

optional<int>& TileSourceOptionsWrapper::tileSize()
{
#ifdef _LOG_CALLS_
	LOG("TileSourceOptionsWrapper::tileSize"); LOG("\n");
#endif
	return TileSourceOptions::tileSize();
}
const optional<int>& TileSourceOptionsWrapper::tileSize() const
{
#ifdef _LOG_CALLS_
	LOG("TileSourceOptionsWrapper::tileSize"); LOG("\n");
#endif
	return TileSourceOptions::tileSize();
}

optional<float>& TileSourceOptionsWrapper::noDataValue()
{
#ifdef _LOG_CALLS_
	LOG("TileSourceOptionsWrapper::noDataValue"); LOG("\n");
#endif
	return TileSourceOptions::noDataValue();
}
const optional<float>& TileSourceOptionsWrapper::noDataValue() const
{
#ifdef _LOG_CALLS_
	LOG("TileSourceOptionsWrapper::noDataValue"); LOG("\n");
#endif
	return TileSourceOptions::noDataValue();

}
optional<float>& TileSourceOptionsWrapper::noDataMinValue()
{
#ifdef _LOG_CALLS_
	LOG("TileSourceOptionsWrapper::noDataMinValue"); LOG("\n");
#endif
	return TileSourceOptions::noDataMinValue();
}
const optional<float>& TileSourceOptionsWrapper::noDataMinValue() const
{
#ifdef _LOG_CALLS_
	LOG("TileSourceOptionsWrapper::noDataMinValue"); LOG("\n");
#endif
	return TileSourceOptions::noDataMinValue();
}

optional<float>& TileSourceOptionsWrapper::noDataMaxValue()
{
#ifdef _LOG_CALLS_
	LOG("TileSourceOptionsWrapper::noDataMaxValue"); LOG("\n");
#endif
	return TileSourceOptions::noDataMaxValue();
}
const optional<float>& TileSourceOptionsWrapper::noDataMaxValue() const
{
#ifdef _LOG_CALLS_
	LOG("TileSourceOptionsWrapper::noDataMaxValue"); LOG("\n");
#endif
	return TileSourceOptions::noDataMaxValue();
}

optional<std::string>& TileSourceOptionsWrapper::blacklistFilename()
{
#ifdef _LOG_CALLS_
	LOG("TileSourceOptionsWrapper::blacklistFilename"); LOG("\n");
#endif
	return TileSourceOptions::blacklistFilename();
}
const optional<std::string>& TileSourceOptionsWrapper::blacklistFilename() const
{
#ifdef _LOG_CALLS_
	LOG("TileSourceOptionsWrapper::blacklistFilename"); LOG("\n");
#endif
	return TileSourceOptions::blacklistFilename();
}

optional<ProfileOptions>& TileSourceOptionsWrapper::profile()
{
#ifdef _LOG_CALLS_
	LOG("TileSourceOptionsWrapper::profile"); LOG("\n");
#endif
	return TileSourceOptions::profile();
}
const optional<ProfileOptions>& TileSourceOptionsWrapper::profile() const
{
#ifdef _LOG_CALLS_
	LOG("TileSourceOptionsWrapper::profile"); LOG("\n");
#endif
	return TileSourceOptions::profile();
}

optional<int>& TileSourceOptionsWrapper::L2CacheSize()
{
#ifdef _LOG_CALLS_
	LOG("TileSourceOptionsWrapper::L2CacheSize"); LOG("\n");
#endif
	return TileSourceOptions::L2CacheSize();

}
const optional<int>& TileSourceOptionsWrapper::L2CacheSize() const
{
#ifdef _LOG_CALLS_
	LOG("TileSourceOptionsWrapper::L2CacheSize"); LOG("\n");
#endif
	return TileSourceOptions::L2CacheSize();
}


Config TileSourceOptionsWrapper::getConfig() const
{
#ifdef _LOG_CALLS_
	LOG("TileSourceOptionsWrapper::getConfig"); LOG("\n");
#endif
	return TileSourceOptions::getConfig();
}


void TileSourceOptionsWrapper::mergeConfig(const Config& conf)
{
#ifdef _LOG_CALLS_
	LOG("TileSourceOptionsWrapper::mergeConfig"); LOG("\n");
#endif
	TileSourceOptions::mergeConfig(conf);
}
