
#ifndef __CINT__
#include  "Fireworks/Core/interface/FWGeometry.h"
#include "TGeoManager.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "TGLScenePad.h"
#include "TSystem.h"
#endif

#include "Fireworks/Core/interface/FWGeometry.h"
#include <vector>
#include "TEveManager.h"
#include "TEveElement.h"

#include "TEveViewer.h"
#include "TEveScene.h"
#include "TEveGeoNode.h"
#include "TEveBrowser.h"

#include "TGListTree.h"



TEveElement* getPB(FWGeometry* geom)
{
   TEveElementList* holder = new TEveElementList("PixelBerrel");
   std::vector<unsigned int> ids = geom->getMatchedIds( FWGeometry::Tracker, FWGeometry::PixelBarrel );


   for( std::vector<unsigned int>::iterator id = ids.begin(); id != ids.end(); ++id )
   {
      TEveGeoShape* shape = geom->getEveShape( *id );
      PXBDetId idid = PXBDetId( *id );
      unsigned int layer = idid.layer();
      const char* layname = Form("Layer_%d", layer);
      TEveElement* layerHolder = holder->FindChild(layname); 
      if (!layerHolder) {
         layerHolder= new TEveElementList();
         layerHolder->SetElementNameTitle(layname,layname);
         holder->AddElement(layerHolder);
      }

      unsigned int ladder = idid.ladder();
      unsigned int module = idid.module();
      const char* lname = Form("Ladder_%d", ladder);
      TEveElement* ladderHolder = layerHolder->FindChild(lname);
      if (!ladderHolder) {
         ladderHolder= new TEveElementList();
         ladderHolder->SetElementNameTitle(lname,lname);
         layerHolder->AddElement(ladderHolder);
      }
      ladderHolder->AddElement(shape);

       
	 
      shape->SetTitle( TString::Format( "PixelBarrel %d: Layer=%u, Ladder=%u, Module=%u",
                                        *id, layer, ladder, module ));
      shape->SetMainColor(kRed);
      shape->SetPickable(true);
   }
   return holder;
}

void geo(const char* recoGeoPath)
{
   if (gSystem->AccessPathName(recoGeoPath)) {
      printf("Invalid reco file path !\n.");
      return;
   }
   FWGeometry* geometry = new FWGeometry();
   geometry->loadMap(recoGeoPath);

   
   TEveScene* gs = (TEveScene*) gEve->GetScenes()->FindChild("GeoScene 3D Tower");
   if (!gs) return;

   TEveElement* geoList = gs->FindChild("3D Geometry");
   gs->GetGLScene()->SetSelectable(true);
   TEveElement* el = getPB(geometry);
   geoList->AddElement(el);

   gEve->AddElement(el);

   gEve->AddToListTree(geoList, true);
   gEve->GetBrowser()->MapWindow();
   gEve->FullRedraw3D();
}
