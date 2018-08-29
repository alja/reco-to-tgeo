#define protected public

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


void setLocalCorners()
{
   if (etaPositive) {


      lc.push_back(TEveVector(-1.4629, -1.4629, -11.0000));
      lc.push_back(TEveVector(-1.4629, 1.3939, -11.0000));
      lc.push_back(TEveVector(1.3939, 1.3939, -11.0000));
      lc.push_back(TEveVector(1.3939, -1.4629, -11.0000));

      lc.push_back(TEveVector(-1.4630, -1.4630, 11.0000));
      lc.push_back(TEveVector(-1.4630, 1.5321, 11.0000));
      lc.push_back(TEveVector(1.5321, 1.5321, 11.0000));
      lc.push_back(TEveVector(1.5321, -1.4630, 11.0000));

   }
   else {


   }

}


void setGlobalCorners()
{
   if (etaPositive) {
      //(EE iz +  ix 59 , iy 36)
      gc.push_back(TEveVector(23.6678, -41.2661, 317.9250));
      gc.push_back(TEveVector(26.5207, -41.2628, 317.7768));
      gc.push_back(TEveVector(26.5111, -44.1085, 317.5261));
      gc.push_back(TEveVector(23.6581, -44.1119, 317.6742));
     
      gc.push_back(TEveVector(24.8067, -43.1986, 339.8103));
      gc.push_back(TEveVector(27.7977, -43.1950, 339.6550));
      gc.push_back(TEveVector(27.7875, -46.1784, 339.3921));
      gc.push_back(TEveVector(24.7966, -46.1820, 339.5475));


   }
   else{

   }
}


void ecalEndcap()
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


   TEveVector tgCenter;
   for (int i = 4; i < 8; ++i)
      tgCenter += gc[i];
   tgCenter *= 0.25;


   TEveVector axis = tgCenter - gCenter;
   axis *= 10;
   printf("AXIS phi = %f, theta = %f \n", axis.Phi()*TMath::RadToDeg(), axis.Theta()*TMath::RadToDeg() );

   float dz = TMath::Abs((lc[4].fZ -lc[0].fZ)) * 0.5;
   printf("dz = %f ====================== \n", dz);

   {
      TEvePointSet* tps = new TEvePointSet("globalPS");
      for (int i = 0; i < 8; ++i)
         tps->SetNextPoint(gc[i].fX, gc[i].fY, gc[i].fZ);
      // tps->SetNextPoint(gc[i].fX -center.fX, gc[i].fY -center.fY, gc[i].fZ -center.fZ);
      tps->SetMainColor(kCyan);
      tps->SetMarkerSize(0.2);
      tps->SetMarkerStyle(2);
      gEve->AddElement(tps);
   }

   {
      TEvePointSet* tps = new TEvePointSet("globalPS-translated");
      for (int i = 0; i < 8; ++i)
         tps->SetNextPoint(gc[i].fX -gCenter.fX, gc[i].fY -gCenter.fY, gc[i].fZ -gCenter.fZ);
      tps->SetMainColor(kCyan);
      tps->SetMarkerSize(0.2);
      tps->SetMarkerStyle(2);
      gEve->AddElement(tps);
   }
   {
      TEveLine* line = new TEveLine("gAxis");
      line->SetNextPoint(0, 0, 0);
      line->SetNextPoint(axis.fX, axis.fY, axis.fZ);
      line->SetMainColor(kCyan);
      line->SetPickable(false);
      gEve->AddElement(line);
   }

   
   {
      TEvePointSet* tps = new TEvePointSet("localPS");
      for (int i = 0; i < 8; ++i)
         tps->SetNextPoint(lc[i].fX , lc[i].fY, lc[i].fZ );

      // tps->SetNextPoint(lc[i].fX -lCenter.fX, lc[i].fY -lCenter.fY, lc[i].fZ -lCenter.fZ);      tps->SetMainColor(kCyan);
      tps->SetMarkerSize(0.2);
      tps->SetMarkerStyle(2);
      tps->SetMainColor(kOrange);
      gEve->AddElement(tps);
   }



   // ARb8
   static const int arr[] = {0, 1, 2, 3, 4, 5, 6, 7 };
   double points[16];
   for (int c = 0; c < 8; ++c)
   {
      points[ c*2 + 0 ] =  (lc[arr[c]].fX - lCenter.fX); 
      points[ c*2 + 1 ] =  (lc[arr[c]].fY - lCenter.fY);
      printf("AMT xy[%d] <=>[%d] = (%.4f, %.4f) \n", arr[c], c, points[c*2],  points[c*2+1]);
   }

   TGeoShape* solid = new TGeoArb8(dz, &points[0]);


   

   TEveGeoShape* eveShape = new TEveGeoShape("phi0");
   eveShape->SetPickable(false);
   eveShape->SetShape(solid);
   double phi = axis.Phi()*TMath::RadToDeg();
   double theta = axis.Theta()*TMath::RadToDeg();

   printf("phi %f theta %f, psi %f \n", phi, theta, axis.Psi()*TMath::RadToDeg());


   TGeoTranslation tr;
   TGeoTranslation gtr(gCenter.fX, gCenter.fY, gCenter.fZ);
   TGeoRotation rot; 
   rot.SetAngles(phi-90, -theta, 0);


   TGeoMatrix* comb = new TGeoCombiTrans( tr, rot );
   eveShape->SetTransMatrix(*comb);

   /*
     TGeoTranslation tr (25.693271, -43.687918, 328.663376);

     TGeoRotation rot;
     const Double_t matrix[9] = { -0.00337824,  -0.996134,  -0.0877803,
     0.998653, 0.00118373, -0.0518664,
     0.0517698,  -0.0878373,  0.994789 };

     rot.SetMatrix( matrix );
     TGeoMatrix* comb = new TGeoCombiTrans( tr, rot );
     eveShape->SetTransMatrix(*comb);
   */
                                
   
   gEve->AddElement(eveShape);
   TEveViewer *ev = gEve->GetDefaultViewer();
   TGLViewer  *gv = ev->GetGLViewer();
   gv->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kFALSE, 0);



   gEve->Redraw3D();
}

