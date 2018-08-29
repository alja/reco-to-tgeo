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

std::vector<TEveVector> lc;
std::vector<TEveVector> gc;

bool etaPositive = 1;

double etatotheta(double eta) {
   using namespace TMath;

   if (eta < 0)
      return Pi() - 2*ATan(Exp(- Abs(eta)));
   else
      return 2*ATan(Exp(- Abs(eta)));
}

void setLocalCorners()
{
   if (etaPositive) {
      lc.push_back(TEveVector(190.4000, 8.3130, 359.6566));
      lc.push_back(TEveVector(190.4000, -8.3130, 359.6566));
      lc.push_back(TEveVector(190.4000, -8.3130, 325.5623));
      lc.push_back(TEveVector(190.4000, 8.3130, 325.5623));
      lc.push_back(TEveVector(232.6000, 10.1555, 439.3705));
      lc.push_back(TEveVector(232.6000, -10.1555, 439.3705));
      lc.push_back(TEveVector(232.6000, -10.1555, 397.7195));
      lc.push_back(TEveVector(232.6000, 10.1555, 397.7195));
   }
   else {
      // dEta = 0.0435, dPhi = 0.0436332, dz = 77.925
      // eta = 0.913500 etaPos = -0.913500, phiPos 0.305433
      lc.push_back(TEveVector(181.1000, 7.9070, 201.1971));
      lc.push_back(TEveVector(181.1000, -7.9070, 201.1971));
      lc.push_back(TEveVector(181.1000, -7.9070, 178.3685));
      lc.push_back(TEveVector(181.1000, 7.9070, 178.3685));
      lc.push_back(TEveVector(288.8000, 12.6093, 320.8488));
      lc.push_back(TEveVector(288.8000, -12.6093, 320.8488));
      lc.push_back(TEveVector(288.8000, -12.6093, 284.4440));
      lc.push_back(TEveVector(288.8000, 12.6093, 284.4440));
   }
}


void setGlobalCorners()
{
   if (etaPositive) {
      gc.push_back(TEveVector(190.5814, -0.0000, 359.6566));
      gc.push_back(TEveVector(189.8562, -16.6103, 359.6566));
      gc.push_back(TEveVector(189.8562, -16.6103, 325.5623));
      gc.push_back(TEveVector(190.5814, -0.0000, 325.5623));
      gc.push_back(TEveVector(232.8216, -0.0000, 439.3705));
      gc.push_back(TEveVector(231.9357, -20.2918, 439.3705));
      gc.push_back(TEveVector(231.9357, -20.2918, 397.7195));
      gc.push_back(TEveVector(232.8216, -0.0000, 397.7195));
   }
   else{
      // dEta = 0.0435, dPhi = 0.0436332, dz = 77.925
      // eta = 0.913500 etaPos = -0.913500, phiPos 0.305433
      gc.push_back(TEveVector(170.3405, 61.9989, -178.3685));
      gc.push_back(TEveVector(175.0958, 46.9168, -178.3685));
      gc.push_back(TEveVector(175.0958, 46.9168, -201.1971));
      gc.push_back(TEveVector(170.3405, 61.9989, -201.1971));
      gc.push_back(TEveVector(271.6418, 98.8695, -284.4440));
      gc.push_back(TEveVector(279.2251, 74.8181, -284.4440));
      gc.push_back(TEveVector(279.2251, 74.8181, -320.8488));
      gc.push_back(TEveVector(271.6418, 98.8695, -320.8488));
   }
}


void validate()
{
   TEveManager::Create();
 
   setLocalCorners();
   setGlobalCorners();

  
   TEveVector gCenter;
   for (int i = 0; i < 8; ++i)
      gCenter += gc[i];
   gCenter *= 0.125;

   TEveVector lCenter;
   for (int i = 0; i < 8; ++i)
      lCenter += lc[i];
   lCenter *= 0.125;



   float dz = TMath::Abs((lc[4].fX -lc[0].fX)) * 0.5;
   printf("dz = %f ====================== \n", dz);

   {
      TEvePointSet* tps = new TEvePointSet("globalPS");
      for (int i = 0; i < 8; ++i)
         tps->SetNextPoint(gc[i].fX, gc[i].fY, gc[i].fZ);
      // tps->SetNextPoint(gc[i].fX -center.fX, gc[i].fY -center.fY, gc[i].fZ -center.fZ);
      tps->SetMainColor(kCyan);
      tps->SetMarkerSize(3);
      tps->SetMarkerStyle(2);
      gEve->AddElement(tps);

      TEveLine* line = new TEveLine("gcenter");
      line->SetNextPoint(0, 0, 0);
      line->SetNextPoint(gCenter.fX, gCenter.fY, gCenter.fZ);
      line->SetMainColor(kCyan);
      line->SetPickable(false);
      gEve->AddElement(line);
   }

   {
      {
         TEvePointSet* tps = new TEvePointSet("localPS");
         for (int i = 0; i < 8; ++i)
            tps->SetNextPoint(lc[i].fX , lc[i].fY, lc[i].fZ );

         // tps->SetNextPoint(lc[i].fX -lCenter.fX, lc[i].fY -lCenter.fY, lc[i].fZ -lCenter.fZ);      tps->SetMainColor(kCyan);
         tps->SetMarkerSize(3);
         tps->SetMarkerStyle(2);
         tps->SetMainColor(kOrange);
         gEve->AddElement(tps);
      }


      {
         TEvePointSet* tps = new TEvePointSet("localPSTrans");
         for (int i = 0; i < 8; ++i)
            tps->SetNextPoint(lc[i].fX -lCenter.fX, lc[i].fY -lCenter.fY, lc[i].fZ -lCenter.fZ);

         tps->SetMarkerSize(3);
         tps->SetMarkerStyle(2);
         tps->SetMainColor(kOrange);
         gEve->AddElement(tps);
      }

      TEveLine* line = new TEveLine("lcenter");
      line->SetNextPoint(0, 0, 0);
      line->SetNextPoint(lCenter.fX, lCenter.fY, lCenter.fZ);
      line->SetMainColor(kOrange);
      line->SetPickable(false);
      gEve->AddElement(line);
   }

   // ARb8
   static const int arr[] = { 1, 0, 3, 2,  5, 4, 7, 6 };
   double points[16];
   for (int c = 0; c < 8; ++c)
   {
      points[ c*2 + 0 ] =  -(lc[arr[c]].fZ - lCenter.fZ); 
      points[ c*2 + 1 ] =  +(lc[arr[c]].fY - lCenter.fY);
      printf("AMT xy[%d] <=>[%d] = (%.4f, %.4f) \n", arr[c], c, points[c*2],  points[c*2+1]);
   }

   TGeoShape* solid = new TGeoArb8(dz, &points[0]);


   TGeoTranslation tr;
   TGeoTranslation gtr(gCenter.fX, gCenter.fY, gCenter.fZ);
   TGeoTranslation ltr(lCenter.fX, lCenter.fY, lCenter.fZ);
   double phi = gCenter.Phi()*TMath::RadToDeg();
   double theta = gCenter.Theta()*TMath::RadToDeg();
   printf("phi %f theta %f \n", phi, theta);

   {
      TEveGeoShape* eveShape = new TEveGeoShape("phi0");
      eveShape->SetPickable(false);
      eveShape->SetShape(solid);

      TGeoRotation rot; 
       rot.RotateY(90);

     
      TGeoRotation rotPhi;
      rotPhi.SetAngles(0, -phi,  0);

      rot.MultiplyBy(&rotPhi);
      // if (gCenter.fZ < 0)
      //  rot.ReflectZ(true, false);
     
      TGeoMatrix* comb = new TGeoCombiTrans( gtr, rot );
     
      eveShape->SetTransMatrix(*comb);
      gEve->AddElement(eveShape);

   }
 
   // validate
    const TBuffer3D* buff = box->MakeBuffer3D();

         

 

   TEveViewer *ev = gEve->GetDefaultViewer();
   TGLViewer  *gv = ev->GetGLViewer();
   gv->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kFALSE, 0);



   gEve->Redraw3D();
}

