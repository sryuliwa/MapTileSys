#pragma once
#include <osgEarth/MapNode>
#include <osgEarthUtil/Common>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarth/Common>
#include <osgEarth/Config>
#include <osgEarth/TileSource>
#include <osgEarth/TerrainLayer>
using namespace osgEarth;
using namespace osgEarth::Drivers;
using namespace osgEarth::Util;
class ImageLayerWrapper :
	public ImageLayer
{
public:
	ImageLayerWrapper(const ImageLayerOptions& options);
	ImageLayerWrapper(const std::string& name, const TileSourceOptions& driverOptions);
	ImageLayerWrapper(const ImageLayerOptions& options, TileSource* tileSource);

	~ImageLayerWrapper();
public: // runtime properties
	void setOpacity(float opacity);

	float getOpacity() const;

	void disableLODBlending();
	bool isLODBlendingEnabled() const;

public: // methods


	virtual GeoImage createImage(const TileKey& key, ProgressCallback* progress = 0, bool forceFallback = false);


	GeoImage createImageInNativeProfile(const TileKey& key, ProgressCallback* progress, bool forceFallback);

public: // TerrainLayer override

	CacheBin* getCacheBin(const Profile* profile);
protected:

	GeoImage createImageInKeyProfile(const TileKey& key, ProgressCallback* progress, bool forceFallback);

	GeoImage createImageFromTileSource(const TileKey& key, ProgressCallback* progress, bool forceFallback);

	
	GeoImage assembleImageFromTileSource(const TileKey& key, ProgressCallback* progress, bool forceFallback);


	virtual void initTileSource();
};

