#pragma once
#include "TileSourceOptionsWrapper.h"
#include <osgEarth/Common>
#include <osgEarth/TileSource>
#include <osgEarth/URI>
class TMSOptionsWrapper :
	public TileSourceOptionsWrapper
{
public:
	optional<URI>& url() { return _url; }
	const optional<URI>& url() const { return _url; }

	optional<std::string>& tmsType() { return _tmsType; }
	const optional<std::string>& tmsType() const { return _tmsType; }

	optional<std::string>& format() { return _format; }
	const optional<std::string>& format() const { return _format; }

public:
	TMSOptionsWrapper(const TileSourceOptionsWrapper& opt = TileSourceOptionsWrapper());
	TMSOptionsWrapper(const std::string& inUrl);

	/** dtor */
	virtual ~TMSOptionsWrapper() { }

public:
	Config getConfig() const;
protected:
	void mergeConfig(const Config& conf);

private:
	void fromConfig(const Config& conf);
	optional<URI>         _url;
	optional<std::string> _tmsType;
	optional<std::string> _format;
};

