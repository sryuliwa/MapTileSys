#pragma once
#include <osgEarth/TileSource>
#include <osgEarth/Registry>
#include <osgEarthUtil/TMS>

#include <osg/Notify>

using namespace osgEarth;
using namespace osgEarth::Util;
class TileSourceOptionsWrapper :
	public TileSourceOptions
{
public:
	TileSourceOptionsWrapper(const ConfigOptions& options = ConfigOptions());
	~TileSourceOptionsWrapper();
public:

	optional<int>& tileSize();
	const optional<int>& tileSize() const;

	optional<float>& noDataValue();
	const optional<float>& noDataValue() const;
	optional<float>& noDataMinValue();
	const optional<float>& noDataMinValue() const;

	optional<float>& noDataMaxValue();
	const optional<float>& noDataMaxValue() const;

	optional<std::string>& blacklistFilename();
	const optional<std::string>& blacklistFilename() const;

	optional<ProfileOptions>& profile();
	const optional<ProfileOptions>& profile() const;

	optional<int>& L2CacheSize();
	const optional<int>& L2CacheSize() const;
public:
	virtual Config getConfig() const;
protected:
	virtual void mergeConfig(const Config& conf);

};

