#include "TEvePointSet.h"
#include "TEveVector.h"
#include "TEveManager.h"
#include "TEveLine.h"
#include "TEveViewer.h"
#include "TEveGeoShape.h"
#include "TGeoShape.h"
#include "TGeoVolume.h"
#include "TGeoArb8.h"
#include "TGeoBBox.h"
#include "TGeoMatrix.h"
#include "TGLViewer.h"

void box()
{
   TEveManager::Create();
 

   TGeoBBox* solid = new TGeoBBox(10, 30, 70);
   TEveGeoShape* eveShape = new TEveGeoShape();
   eveShape->SetPickable(false);
   eveShape->SetShape(solid);



  
   gEve->AddElement(eveShape);




   TEveViewer *ev = gEve->GetDefaultViewer();
   TGLViewer  *gv = ev->GetGLViewer();
   gv->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kFALSE, 0);



   gEve->Redraw3D();
}
