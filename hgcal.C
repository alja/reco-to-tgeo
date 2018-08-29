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
      /*
      //1795694592
      lc.push_back(TEveVector(4.9778, -100.4823, -0.9000));
      lc.push_back(TEveVector(-30.4577, 100.4823, -0.9000));
      lc.push_back(TEveVector(12.7399, 100.4823, -0.9000));
      lc.push_back(TEveVector(12.7399, -100.4823, -0.9000));
      lc.push_back(TEveVector(4.9778, -100.4823, 0.9000));
      lc.push_back(TEveVector(-30.4577, 100.4823, 0.9000));
      lc.push_back(TEveVector(12.7399, 100.4823, 0.9000));
      lc.push_back(TEveVector(12.7399, -100.4823, 0.9000));
      */

      // isEE=1 zpos=1 layer=1 phi subSector=-1 sector=2 cell=3
      lc.push_back(TEveVector(-5.5924, -59.1699, -0.0100));
      lc.push_back(TEveVector(-26.4589, 59.1699, -0.0100));
      lc.push_back(TEveVector(26.4589, 59.1699, -0.0100));
      lc.push_back(TEveVector(5.5924, -59.1699, -0.0100));
      lc.push_back(TEveVector(-5.5924, -59.1699, 0.0100));
      lc.push_back(TEveVector(-26.4589, 59.1699, 0.0100));
      lc.push_back(TEveVector(26.4589, 59.1699, 0.0100));
      lc.push_back(TEveVector(5.5924, -59.1699, 0.0100));

   }
   else {

   }

}


void setGlobalCorners()
{
   if (etaPositive) {
      /*
        gc.push_back(TEveVector(38.7117, 22.3502, 439.6500));
        gc.push_back(TEveVector(215.4369, 124.3826, 439.6500));
        gc.push_back(TEveVector(230.2114, 83.7901, 439.6500));
        gc.push_back(TEveVector(41.3665, 15.0562, 439.6500));
        gc.push_back(TEveVector(38.7117, 22.3502, 441.4500));
        gc.push_back(TEveVector(215.4369, 124.3826, 441.4500));
        gc.push_back(TEveVector(230.2114, 83.7901, 441.4500));
        gc.push_back(TEveVector(41.3665, 15.0562, 441.4500));
      */
      gc.push_back(TEveVector(27.8909, 16.1028, 317.3000));
      gc.push_back(TEveVector(131.9572, 76.1855, 317.3000));
      gc.push_back(TEveVector(150.0561, 26.4589, 317.3000));
      gc.push_back(TEveVector(31.7163, 5.5924, 317.3000));
      gc.push_back(TEveVector(27.8909, 16.1028, 317.3200));
      gc.push_back(TEveVector(131.9572, 76.1855, 317.3200));
      gc.push_back(TEveVector(150.0561, 26.4589, 317.3200));
      gc.push_back(TEveVector(31.7163, 5.5924, 317.3200));
   }
   else{


   }
}


void hgcal()
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
   static const int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
   //  static const int arr[] = {  7, 6, 5, 4 ,3, 2, 1, 0};
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
   double alpha = -0.087936*TMath::RadToDeg();
   printf("phi %f theta %f \n", phi, theta);

   {
      TEveGeoShape* eveShape = new TEveGeoShape("phi0");
      eveShape->SetPickable(false);
      eveShape->SetShape(solid);

      TGeoRotation  rot; 
      rot.SetAngles( phi-90 + alpha,0, 0);

      TGeoMatrix* comb = new TGeoCombiTrans( gtr, rot );
      eveShape->SetTransMatrix(*comb);
      gEve->AddElement(eveShape);

   }
 

   // phiPos 0.437002 == phiAxis 0.000000, thetaPos = 0.318098 , getThetaAxis = 0.000000

 

   TEveViewer *ev = gEve->GetDefaultViewer();
   TGLViewer  *gv = ev->GetGLViewer();
   gv->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kFALSE, 0);



   gEve->Redraw3D();
}

