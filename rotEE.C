#define protected public
#include "TEvePointSet.h"
#include "TEveVector.h"
#include "TEveManager.h"
#include "TEveLine.h"
#include "TEveTrans.h"
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


void setLocalCorners()
{
   lc.push_back(TEveVector(-1.4629, -1.4629, -11.0000));
   lc.push_back(TEveVector(-1.4629, 1.3939, -11.0000));
   lc.push_back(TEveVector(1.3939, 1.3939, -11.0000));
   lc.push_back(TEveVector(1.3939, -1.4629, -11.0000));

   lc.push_back(TEveVector(-1.4630, -1.4630, 11.0000));
   lc.push_back(TEveVector(-1.4630, 1.5321, 11.0000));
   lc.push_back(TEveVector(1.5321, 1.5321, 11.0000));
   lc.push_back(TEveVector(1.5321, -1.4630, 11.0000));
}


void setGlobalCorners()
{
   gc.push_back(TEveVector(23.6678, -41.2661, 317.9250));
   gc.push_back(TEveVector(26.5207, -41.2628, 317.7768));
   gc.push_back(TEveVector(26.5111, -44.1085, 317.5261));
   gc.push_back(TEveVector(23.6581, -44.1119, 317.6742));
     
   gc.push_back(TEveVector(24.8067, -43.1986, 339.8103));
   gc.push_back(TEveVector(27.7977, -43.1950, 339.6550));
   gc.push_back(TEveVector(27.7875, -46.1784, 339.3921));
   gc.push_back(TEveVector(24.7966, -46.1820, 339.5475));
}


void rotEE()
{
   TEveManager::Create();
 
   //(EE iz +  ix 59 , iy 36)
   setLocalCorners();
   setGlobalCorners();

  
   TEveVector gCenter;
   for (int i = 0; i < 8; ++i)
      gCenter += gc[i];
   gCenter *= 0.125;


   TEveVector tgCenter; // top center 4 corners
   for (int i = 4; i < 8; ++i)
      tgCenter += gc[i];
   tgCenter *= 0.25;


   TEveVector axis = tgCenter - gCenter;
   double phi = axis.Phi()*TMath::RadToDeg();
   double theta = axis.Theta()*TMath::RadToDeg();
   printf("phi %f theta %f \n", phi, theta );

   float dz =(lc[4].fZ -lc[0].fZ) * 0.5;


   

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
      TEvePointSet* tps = new TEvePointSet("globalPS-translated");
      for (int i = 0; i < 8; ++i)
         tps->SetNextPoint(gc[i].fX , gc[i].fY , gc[i].fZ );
      tps->SetMainColor(kCyan);
      tps->SetMarkerSize(0.2);
      tps->SetMarkerStyle(2);
      gEve->AddElement(tps);
   }
   {
      TEveLine* line = new TEveLine("gAxis");
      line->SetNextPoint(0, 0, 0);
      float s  = 100;
      line->SetNextPoint(axis.fX*s, axis.fY*s, axis.fZ*s);
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
      points[ c*2 + 0 ] =  lc[arr[c]].fX;
      points[ c*2 + 1 ] =  lc[arr[c]].fY;
      printf("AMT xy[%d] <=>[%d] = (%.4f, %.4f) \n", arr[c], c, points[c*2],  points[c*2+1]);
   }

   TGeoShape* solid = new TGeoArb8(dz, &points[0]);
   TEveGeoShape* eveShape = new TEveGeoShape("phi0");
   eveShape->SetPickable(false);
   eveShape->SetShape(solid);


  
   TEveTrans& tr = eveShape->RefMainTrans();
   axis.Normalize();
   TVector3 v1t;
   tr.GetBaseVec(1, v1t);
   TEveVector v1(v1t.x(), v1t.y(), v1t.z());
   double dot13 = axis.Dot(v1);
   TEveVector gd = axis;
   gd*= dot13;
   v1 -= gd;
   v1.Normalize();
   TEveVector v2;
   TMath::Cross(v1.Arr(), axis.Arr(), v2.Arr());
   TMath::Cross(axis.Arr(), v1.Arr(), v2.Arr());
   v2.Normalize();

   tr.SetBaseVec(1, v1.fX, v1.fY, v1.fZ);
   tr.SetBaseVec(2, v2.fX, v2.fY, v2.fZ);
   tr.SetBaseVec(3, axis.fX, axis.fY, axis.fZ);
   tr.Move3PF(gCenter.fX, gCenter.fY, gCenter.fZ);
   //   float ang[3];
   // tr.GetRotAngles(ang);
   // printf("angle %f %f %f \n",ang[0], ang[1], ang[2] );
   //tr.UnitTrans();
   // TGeoRotation rot; 
   // rot.SetAngles(ang[0], ang[1], ang[2]);

   //   TGeoMatrix* comb = new TGeoCombiTrans( tr, rot );
   //   eveShape->SetTransMatrix(rot);

   eveShape->RefMainTrans().Print();

   
   gEve->AddElement(eveShape);
   TEveViewer *ev = gEve->GetDefaultViewer();
   TGLViewer  *gv = ev->GetGLViewer();
   gv->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kFALSE, 0);



   gEve->Redraw3D();
}

