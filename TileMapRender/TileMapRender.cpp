// TileMapRender.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osg/Notify>
#include <osgGA/GUIEventHandler>
#include <osgGA/StateSetManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgEarth/MapNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/Controls>
#include <osgEarthSymbology/Color>
#include <osgEarthDrivers/tms/TMSOptions>
#include <iostream>
#include "TMSOptionsWrapper.h"
#include "ImageLayerWrapper.h"
#define TMS_PATH "E:\\MyProjects\\OSGRelated\\MapTileSystem\\Debug\\land_shallow_topo_east.tif"
using namespace osgEarth;
using namespace osgEarth::Drivers;
using namespace osgEarth::Util;

int main(int argc, char** argv)
{
	
	osg::ArgumentParser arguments(&argc, argv);
	Map* map = new Map();

	TMSOptionsWrapper* imagery = new TMSOptionsWrapper();
	

	imagery->url() = "http://readymap.org/readymap/tiles/1.0.0/7/";

	ImageLayer* imageLayer = new ImageLayerWrapper("Imagery", *(reinterpret_cast<TileSourceOptions*>(imagery)));
	map->addImageLayer(imageLayer);

	TMSOptionsWrapper* elevation = new TMSOptionsWrapper();

	elevation->url() = "http://readymap.org/readymap/tiles/1.0.0/9/";
	//elevation->url() = TMS_PATH;
	
	
	map->addElevationLayer(new ElevationLayer("SRTM", *(reinterpret_cast<TileSourceOptions*>(elevation))));

	MapNode* node = new MapNode(map);


	osgViewer::Viewer viewer(arguments);
	viewer.setCameraManipulator(new EarthManipulator);
	viewer.setSceneData(node);

	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.addEventHandler(new osgViewer::WindowSizeHandler());
	viewer.addEventHandler(new osgViewer::ThreadingHandler());
	viewer.addEventHandler(new osgViewer::LODScaleHandler());
	viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
	viewer.addEventHandler(new osgViewer::HelpHandler(arguments.getApplicationUsage()));

	return viewer.run();
}