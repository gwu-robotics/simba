#ifndef SIMDEVICEMANAGER_H
#define SIMDEVICEMANAGER_H

#include <SimDevices/SimDeviceInfo.h>
#include <SimDevices/SimDevices.h>
#include <ModelGraph/PhysicsEngine.h>
#include <URDFParser/URDF_Parser.h>

using namespace std;

class SimDeviceManager
{

public:
  PhysicsEngine                      m_PhysWrapper;
  URDF_Parser                        m_Parser;
  vector<SimDeviceInfo>              m_vSimDevices;
  SceneGraph::GLSceneGraph&          m_rSceneGraph;

  /// <DeviceName, SimGPS*>
  map<string, SimGPS*>               m_SimGPSList;
  map<string, SimVicon*>             m_SimViconList;
  map<string, SimCam*>               m_SimCamList;
  map<string, SimLaser2D*>           m_SimLaser2DList;
  map<string, SimLaser3D*>           m_SimLaser3DList;
  map<string, SimpleController*>     m_SimpleControllerList;
  map<string, CarController*>        m_CarControllerList;
  PoseController                     m_SimpPoseController;

  /// Constructor
  SimDeviceManager(SceneGraph::GLSceneGraph& rSceneGraph);

  /// Initializers
  bool InitFromXML(PhysicsEngine& rPhysWrapper, XMLDocument& doc, string sProxyName, string sPoseFile);
  void InitAllDevices();
  void InitDeviceByName(string sDeviceName);
  void InitCamDevice(SimDeviceInfo& Device, string sCameraModel);
  void InitViconDevice(SimDeviceInfo& Device);
  void InitController(SimDeviceInfo& Device);

  /// Update devices
  void UpdateAllDevices();

  SimDeviceInfo GetDeviceInfo(string sDeviceName);

  /// Get pointers to all devices
  SimpleController* GetSimpleController(string name);
  SimCam* GetSimCam(string name);
  SimGPS* GetSimGPS(string name);
  SimVicon* GetSimVecon(string name);
};

#endif // SIMDEVICEMANAGER_H
