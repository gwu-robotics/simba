#ifndef RAYCASTVEHICLE_H
#define RAYCASTVEHICLE_H

#include <ModelGraph/Shape.h>
#include <ModelGraph/VehicleEnums.h>
#include <iostream>

// It's a simple class, but enough to hold the info we need.

class RaycastVehicle : public Shape
{
public:

  RaycastVehicle(std::string sName, vector<double> dParameters,
                 Eigen::Vector6d dPose){
    m_dParameters = dParameters;
    m_dPose = dPose;
    m_sName = sName;
    m_sBodyMesh = "NONE";
    m_sWheelMesh = "NONE";
  }

  void SetWheelPose(int i, Eigen::Vector6d wheel_pose){
    if(i==0){
      m_FLWheelPose = wheel_pose;
    }
    else if(i==1){
      m_FRWheelPose = wheel_pose;
    }
    else if(i==2){
      m_BLWheelPose = wheel_pose;
    }
    else if(i==3){
      m_BRWheelPose = wheel_pose;
    }
    else{
      std::cout<<"[RaycastVehicle] There's nothing to do..."<<std::endl;
    }
  }

  void SetMeshes(std::string sBodyMesh, std::string sWheelMesh,
                 std::vector<double> vBodyDim, std::vector<double> vWheelDim){
    m_sBodyMesh = sBodyMesh;
    m_sWheelMesh = sWheelMesh;
    Eigen::Vector3d body_dim;
    Eigen::Vector3d wheel_dim;
    body_dim<<vBodyDim[0], vBodyDim[1], vBodyDim[2];
    wheel_dim<<vWheelDim[0], vWheelDim[1], vWheelDim[2];
    m_BodyMeshDim = body_dim;
    m_WheelMeshDim = wheel_dim;
  }

  vector<double> GetParameters(){
    return m_dParameters;
  }

  Eigen::Vector6d GetWheelPose(int i){
    if(i==0){
      return m_FLWheelPose;
    }
    else if(i==1){
      return m_FRWheelPose;
    }
    else if(i==2){
      return m_BLWheelPose;
    }
    else if(i==3){
      return m_BRWheelPose;
    }
    else{
      std::cout<<"[RaycastVehicle] There's nothing to do..."<<std::endl;
    }
  }

  std::string GetBodyMesh(){
    return m_sBodyMesh;
  }

  std::string GetWheelMesh(){
    return m_sWheelMesh;
  }

  Eigen::Vector3d GetBodyMeshDim(){
    return m_BodyMeshDim;
  }

  Eigen::Vector3d GetWheelMeshDim(){
    return m_WheelMeshDim;
  }

  vector<double> m_dParameters;
  Eigen::Vector6d m_FLWheelPose;
  Eigen::Vector6d m_FRWheelPose;
  Eigen::Vector6d m_BLWheelPose;
  Eigen::Vector6d m_BRWheelPose;
  std::string m_sBodyMesh;
  std::string m_sWheelMesh;
  Eigen::Vector3d m_BodyMeshDim;
  Eigen::Vector3d m_WheelMeshDim;

};



#endif // RAYCASTVEHICLE_H
