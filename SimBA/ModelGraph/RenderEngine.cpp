#include <ModelGraph/RenderEngine.h>


void RenderEngine::Init(std::string sLocalSimName){
  //Start our SceneGraph interface
  pangolin::CreateGlutWindowAndBind(sLocalSimName,640,480);
  SceneGraph::GLSceneGraph::ApplyPreferredGlSettings();
  glClearColor(0, 0, 0, 1);
  glewInit();
}

///////////////////////

// Add to our list of SceneEntities
// This will be added later in 'AddToScene'
void RenderEngine::AddNode( ModelNode *pNode){

  // Add our RaycastVehicle (a myriad of shapes)
  if (dynamic_cast<RaycastVehicle*>(pNode) != NULL){
    // A RaycastVehicle is made of a cube and four cylinders.
    // Since a vehicle is one shape in the PhysicsEngine, but five shapes
    // in the RenderEngine, we must call PhysicsEngine::GetVehicleTransform
    // anytime we want to update the rendering.

    // Get the positions of every part of the car.

    RaycastVehicle* pVehicle = (RaycastVehicle*) pNode;
    std::vector<double> params = pVehicle->GetParameters();

    // The chassis
    SceneGraph::GLBox* chassis = new SceneGraph::GLBox();
    chassis->SetExtent(params[Width], params[WheelBase], params[Height]);
    chassis->SetPose(pVehicle->GetPose());
    m_mSceneEntities[pNode] = chassis;

    // FL Wheel
    SceneGraph::GLCylinder* FLWheel = new SceneGraph::GLCylinder();
    FLWheel->Init(2*params[WheelRadius], 2*params[WheelRadius],
                  params[WheelWidth], 10, 10);
    FLWheel->SetPose(pVehicle->GetWheelPose(0));
    m_mRaycastWheels[pVehicle->GetName()+"@FLWheel"] = FLWheel;

    // FR Wheel
    SceneGraph::GLCylinder* FRWheel = new SceneGraph::GLCylinder();
    FRWheel->Init(2*params[WheelRadius], 2*params[WheelRadius],
                  params[WheelWidth], 10, 10);
    FRWheel->SetPose(pVehicle->GetWheelPose(1));
    m_mRaycastWheels[pVehicle->GetName()+"@FRWheel"] = FRWheel;

    // BL Wheel
    SceneGraph::GLCylinder* BLWheel = new SceneGraph::GLCylinder();
    BLWheel->Init(2*params[WheelRadius], 2*params[WheelRadius],
                  params[WheelWidth], 10, 10);
    BLWheel->SetPose(pVehicle->GetWheelPose(2));
    m_mRaycastWheels[pVehicle->GetName()+"@BLWheel"] = BLWheel;

    // BR Wheel
    SceneGraph::GLCylinder* BRWheel = new SceneGraph::GLCylinder();
    BRWheel->Init(2*params[WheelRadius], 2*params[WheelRadius],
                  params[WheelWidth], 10, 10);
    BRWheel->SetPose(pVehicle->GetWheelPose(3));
    m_mRaycastWheels[pVehicle->GetName()+"@BRWheel"] = BRWheel;
  }

  // Add our Shapes
  if (dynamic_cast<Shape*>(pNode) != NULL){
    Shape* pShape = (Shape*)(pNode);

    //Box
    if (dynamic_cast<BoxShape*>(pShape) != NULL){
      BoxShape* pbShape = (BoxShape *) pShape;
      SceneGraph::GLBox* new_box = new SceneGraph::GLBox();
      new_box->SetExtent(pbShape->m_dBounds[0], pbShape->m_dBounds[1],
                         pbShape->m_dBounds[2]);
      new_box->SetPose(pbShape->GetPose());
      m_mSceneEntities[pNode] = new_box;
    }

    //Cylinder
    else if (dynamic_cast<CylinderShape*>(pShape) != NULL){
      CylinderShape* pbShape = (CylinderShape *) pShape;
      SceneGraph::GLCylinder* new_cylinder = new SceneGraph::GLCylinder();
      new_cylinder->Init(pbShape->m_dRadius, pbShape->m_dRadius,
                         pbShape->m_dHeight, 32, 1);
      new_cylinder->SetPose(pbShape->GetPose());
      m_mSceneEntities[pNode] = new_cylinder;
    }

    //Plane
    else if (dynamic_cast<PlaneShape*>(pShape) != NULL){
      PlaneShape* pbShape = (PlaneShape *) pShape;
      SceneGraph::GLGrid* new_plane = new SceneGraph::GLGrid();
      new_plane->SetNumLines(20);
      new_plane->SetLineSpacing(1);
      Eigen::Vector3d eig_norm;
      eig_norm<<pbShape->m_dNormal[0],
          pbShape->m_dNormal[1],
          pbShape->m_dNormal[2];
      new_plane->SetPlane(eig_norm);
      m_mSceneEntities[pNode] = new_plane;
    }

    //Light
    else if (dynamic_cast<LightShape*>(pShape) != NULL){
      LightShape* pbShape = (LightShape *) pShape;
      SceneGraph::GLShadowLight* new_light = new SceneGraph::GLShadowLight();
      new_light->SetPose(pbShape->GetPose());
      new_light->EnableLight();
      m_mSceneEntities[pNode] = new_light;
    }

    //Mesh
    else if (dynamic_cast<MeshShape*>(pShape) != NULL){
      MeshShape* pbShape = (MeshShape *) pShape;
      SceneGraph::GLMesh* new_mesh =
          new SceneGraph::GLMesh(pbShape->GetFileDir());
      new_mesh->SetPose(pbShape->GetPose());
      m_mSceneEntities[pNode] = new_mesh;
    }
  }
}

///////////////////////////////////////

void RenderEngine::AddDevice(SimDeviceInfo info){


}

///////////////////////////////////////


void RenderEngine::AddToScene(){

  // Add shapes
  std::map<ModelNode*, SceneGraph::GLObject* >::iterator it;
  for(it = m_mSceneEntities.begin(); it != m_mSceneEntities.end(); it++) {
    SceneGraph::GLObject* p = it->second;
    m_glGraph.AddChild( p );
  }
  // If we have them, add wheels
  std::map<string, SceneGraph::GLObject* >::iterator jj;
  for(jj = m_mRaycastWheels.begin(); jj != m_mRaycastWheels.end(); jj++) {
    SceneGraph::GLObject* w = jj->second;
    m_glGraph.AddChild( w );
  }
}

void RenderEngine::CompleteScene(){
  const SceneGraph::AxisAlignedBoundingBox bbox =
      m_glGraph.ObjectAndChildrenBounds();
  const Eigen::Vector3d center = bbox.Center();
  const double size = bbox.Size().norm();
  const double far = 2*size;
  const double near = far / 1E3;

  // Define Camera Render Object (for view / scene browsing)
  pangolin::OpenGlRenderState stacks(
        pangolin::ProjectionMatrix(640,480,420,420,320,240,near,far),
        pangolin::ModelViewLookAt(center(0), center(1) + 7,
                                  center(2) + 6,
                                  center(0), center(1), center(2),
                                  pangolin::AxisZ) );
  m_stacks3d = stacks;

  // We define a new view which will reside within the container.

  // We set the views location on screen and add a handler which will
  // let user input update the model_view matrix (stacks3d) and feed through
  // to our scenegraph
  m_view3d = new pangolin::View(0.0);
  m_view3d->SetBounds( 0.0, 1.0, 0.0, 0.75/*, -640.0f/480.0f*/ );
  m_view3d->SetHandler( new SceneGraph::HandlerSceneGraph(
                          m_glGraph, m_stacks3d) );
  m_view3d->SetDrawFunction( SceneGraph::ActivateDrawFunctor(
                               m_glGraph, m_stacks3d) );

  // window for display image capture from SimCamera
  m_LSimCamImage = new SceneGraph::ImageView(true, true);
  m_LSimCamImage->SetBounds( 0.0, 0.5, 0.75, 1.0/*, 512.0f/384.0f*/ );

  // window for display image capture from SimCamera
  m_RSimCamImage = new SceneGraph::ImageView(true, true);
  m_RSimCamImage->SetBounds( 0.5, 1.0, 0.75, 1.0/*, 512.0f/384.0f */);

  // Add our views as children to the base container.
  pangolin::DisplayBase().AddDisplay( *m_view3d );
  pangolin::DisplayBase().AddDisplay( *m_LSimCamImage );
  pangolin::DisplayBase().AddDisplay( *m_RSimCamImage );

}

void RenderEngine::UpdateScene(){
  std::map<ModelNode*, SceneGraph::GLObject*>::iterator it;
  for(it=m_mSceneEntities.begin(); it != m_mSceneEntities.end(); it++) {
    ModelNode* mn = it->first;
    SceneGraph::GLObject* p = it->second;
    p->SetPose( mn->GetPose() );

    //Update all of our tires.
    if((dynamic_cast<RaycastVehicle*>(mn) != NULL)){
      std::map<string, SceneGraph::GLObject*>::iterator jj;
      RaycastVehicle* pVehicle = (RaycastVehicle*) mn;
      for(jj=m_mRaycastWheels.begin(); jj != m_mRaycastWheels.end(); jj++) {
        string name = jj->first;
        SceneGraph::GLObject* wheel = jj->second;
        if(name == pVehicle->GetName()+"@FLWheel"){
          wheel->SetPose(pVehicle->GetWheelPose(0));
        }
        else if(name == pVehicle->GetName()+"@FRWheel"){
          wheel->SetPose(pVehicle->GetWheelPose(1));
        }
        else if(name == pVehicle->GetName()+"@BLWheel"){
          wheel->SetPose(pVehicle->GetWheelPose(2));
        }
        else if(name == pVehicle->GetName()+"@BRWheel"){
          wheel->SetPose(pVehicle->GetWheelPose(3));
        }
      }
    }
  }
}


