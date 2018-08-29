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

bool etaPositive = 0;

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

      lc.push_back(TEveVector(221.6932, 9.6793, 418.7680));
      lc.push_back(TEveVector(221.6932, -9.6793, 418.7680));
      lc.push_back(TEveVector(244.9099, -10.6930, 418.7680));
      lc.push_back(TEveVector(244.9099, 10.6930, 418.7680));
      lc.push_back(TEveVector(249.3275, 10.8859, 470.9680));
      lc.push_back(TEveVector(249.3275, -10.8859, 470.9680));
      lc.push_back(TEveVector(275.4383, -12.0259, 470.9680));
      lc.push_back(TEveVector(275.4383, 12.0259, 470.9680));

   }
   else {

      lc.push_back(TEveVector(221.6932, 9.6793, 418.7680));
      lc.push_back(TEveVector(221.6932, -9.6793, 418.7680));
      lc.push_back(TEveVector(244.9099, -10.6930, 418.7680));
      lc.push_back(TEveVector(244.9099, 10.6930, 418.7680));
      lc.push_back(TEveVector(249.3275, 10.8859, 470.9680));
      lc.push_back(TEveVector(249.3275, -10.8859, 470.9680));
      lc.push_back(TEveVector(275.4383, -12.0259, 470.9680));
      lc.push_back(TEveVector(275.4383, 12.0259, 470.9680));

   }

}


void setGlobalCorners()
{
   if (etaPositive) {
      gc.push_back(TEveVector(208.5219, 75.8958, 418.7680));
      gc.push_back(TEveVector(214.3431, 57.4331, 418.7680));
      gc.push_back(TEveVector(236.7902, 63.4477, 418.7680));
      gc.push_back(TEveVector(230.3593, 83.8439, 418.7680));
      gc.push_back(TEveVector(234.5144, 85.3563, 470.9680));
      gc.push_back(TEveVector(241.0613, 64.5922, 470.9680));
      gc.push_back(TEveVector(266.3064, 71.3566, 470.9680));
      gc.push_back(TEveVector(259.0739, 94.2952, 470.9680));

   }
   else{
      gc.push_back(TEveVector(230.3593, 83.8439, -418.7680));
      gc.push_back(TEveVector(236.7902, 63.4477, -418.7680));
      gc.push_back(TEveVector(214.3431, 57.4331, -418.7680));
      gc.push_back(TEveVector(208.5219, 75.8958, -418.7680));
      gc.push_back(TEveVector(259.0739, 94.2952, -470.9680));
      gc.push_back(TEveVector(266.3064, 71.3566, -470.9680));
      gc.push_back(TEveVector(241.0613, 64.5922, -470.9680));
      gc.push_back(TEveVector(234.5144, 85.3563, -470.9680));

   }
}


void endcap()
{
   TEveManager::Create();
 
   // (HE 16,4,3)
   // FIREWORKS NEW SHAPE BEGIN eta = 1.348500 etaPos = 1.348500, phiPos 0.305433 >>>>>>

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



   float dz = TMath::Abs((lc[4].fZ -lc[0].fZ)) * 0.5;
   printf("dz = %f ====================== \n", dz);

   { {
         TEvePointSet* tps = new TEvePointSet("globalPS");
         for (int i = 0; i < 8; ++i)
            tps->SetNextPoint(gc[i].fX, gc[i].fY, gc[i].fZ);
         // tps->SetNextPoint(gc[i].fX -center.fX, gc[i].fY -center.fY, gc[i].fZ -center.fZ);
         tps->SetMainColor(kCyan);
         tps->SetMarkerSize(3);
         tps->SetMarkerStyle(2);
         gEve->AddElement(tps);
      }

      {
         TEvePointSet* tps = new TEvePointSet("globalPS");
         for (int i = 0; i < 8; ++i)
          tps->SetNextPoint(gc[i].fX -gCenter.fX, gc[i].fY -gCenter.fY, gc[i].fZ -gCenter.fZ);
         tps->SetMainColor(kCyan);
         tps->SetMarkerSize(3);
         tps->SetMarkerStyle(2);
         gEve->AddElement(tps);
      }
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
         for (int i = 0; i < 8; ++i) {
            tps->SetNextPoint(lc[i].fX -lCenter.fX, lc[i].fY -lCenter.fY, lc[i].fZ -lCenter.fZ);
            printf("%.4f, %.4f, %.4f \n", lc[i].fX -lCenter.fX, lc[i].fY -lCenter.fY, lc[i].fZ -lCenter.fZ);
         }
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
   //  static const int arr[] = { 3, 2, 1, 0, 7, 6, 5, 4 };
   static const int arr[] = {  7, 6, 5, 4 ,3, 2, 1, 0};
   double points[16];
   for (int c = 0; c < 8; ++c)
   {
      points[ c*2 + 0 ] =  (lc[arr[c]].fX - lCenter.fX); 
      points[ c*2 + 1 ] =  (lc[arr[c]].fY - lCenter.fY);
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
      rot.SetAngles(phi,0, 0);
      // if (gCenter.fZ < 0)
      //  rot.ReflectZ(false, false);

      TGeoMatrix* comb = new TGeoCombiTrans( tr, rot );
      eveShape->SetTransMatrix(*comb);
      gEve->AddElement(eveShape);

   }
 

 

   TEveViewer *ev = gEve->GetDefaultViewer();
   TGLViewer  *gv = ev->GetGLViewer();
   gv->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kFALSE, 0);



   gEve->Redraw3D();
}

