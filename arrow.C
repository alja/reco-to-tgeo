// @(#)root/eve:$Id$
// Author: Alja Mrak-Tadel

// Demonstrates usage of TEveArrow class.


#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TEveManager.h"
#include "TEveVector.h"

#include "TGLClip.h"
#include "TGLUtil.h"

const float fgColor[4] = { 1.0, 0.6, 0.2, 0.5 };
TEveVector in(0.1, 0.4, 0.2);

//==============================================================================


class TGLClipsiLogical : public TGLLogicalShape
{
protected:
   virtual void DirectDraw(TGLRnrCtx & rnrCtx) const override{}

public:
   TGLClipsiLogical() : TGLLogicalShape() {}
   virtual ~TGLClipsiLogical() {}
   void Resize(Double_t ext){}
};


class Clipsi : public TGLClip
{
private:
   TGLRnrCtx* m_rnrCtx;
   Clipsi(const Clipsi&);            // Not implemented
   Clipsi& operator=(const Clipsi&); // Not implemented

   TGLVertex3 vtx[4];

public:
   Clipsi(TGLRnrCtx* ctx, TEveVector* vec):
      TGLClip(* new TGLClipsiLogical, TGLMatrix(), fgColor), m_rnrCtx(ctx)
   { 

  for (int i = 0; i < 4; ++i)
         vtx[i].Set(vec[i].fX, vec[i].fY, vec[i].fZ);


   }

   virtual ~Clipsi() {}
   using TGLClip::Setup;
   virtual void Setup(const TGLBoundingBox & bbox) override {}

   using TGLClip::PlaneSet;
   virtual void PlaneSet(TGLPlaneSet_t & planeSet) const override
   {
      TGLVertex3 o;


    
      planeSet.push_back(TGLPlane(o, vtx[0], vtx[1]));
      planeSet.push_back(TGLPlane(o, vtx[1], vtx[2]));
      planeSet.push_back(TGLPlane(o, vtx[2], vtx[3]));
      planeSet.push_back(TGLPlane(o, vtx[3], vtx[0]));
   }
};
//==============================================================================

void cmsGeo()
{

   TFile::SetCacheFileDir(".");
   gGeoManager = gEve->GetGeometry("http://root.cern.ch/files/cms.root");

   gGeoManager->DefaultColors();

   TGeoVolume* top = gGeoManager->GetTopVolume()->FindNode("CMSE_1")->GetVolume();

   TEveGeoTopNode* trk = new TEveGeoTopNode(gGeoManager, top->FindNode("TRAK_1"));
   trk->SetVisLevel(6);
   gEve->AddGlobalElement(trk);

   TEveGeoTopNode* calo = new TEveGeoTopNode(gGeoManager, top->FindNode("CALO_1"));
   calo->SetVisLevel(3);
   gEve->AddGlobalElement(calo);

   TEveGeoTopNode* muon = new TEveGeoTopNode(gGeoManager, top->FindNode("MUON_1"));
   muon->SetVisLevel(4);
   gEve->AddGlobalElement(muon);



}

//==============================================================================


void arrow()
{
   gSystem->IgnoreSignal(kSigSegmentationViolation, true);

   TEveManager::Create();
   cmsGeo();

   in *= 5;

   TEveStraightLineSet* ls = new TEveStraightLineSet("Frust");
   gEve->AddElement(ls);
   ls->SetMainColor(kWhite);


   TEveVector normXY(0., 1., 0);
   TEveVector b0 = in.Cross(normXY);
   b0.Normalize();
   TEveVector b1 = in.Cross(b0);
   b1.Normalize();



   ls->AddLine(0, 0, 0, in.fX, in.fY, in.fZ);

   TEveVector c[4];
   c[0] += b0; c[0] += b1; 
   c[1] -= b0; c[1] += b1;  
   c[2] -= b0; c[2] -= b1;  
   c[3] += b0; c[3] -= b1; 
   for (int i = 0; i < 4; ++i)
   {
      c[i] += in;
   }


   TEvePointSet* marker = new TEvePointSet(8);
   marker->Reset(4);
   marker->SetName("marker");
   marker->SetMarkerColor(kOrange);
   marker->SetMarkerStyle(3);
   marker->SetMarkerSize(0.2);

   for (int i = 0; i < 4; ++i)
   {
      marker->SetPoint(i, c[i].fX, c[i].fY, c[i].fZ);
   }

   gEve->AddElement(marker);



   ls->AddLine(c[0], c[1]);
   ls->AddLine(c[1], c[2]);
   ls->AddLine(c[2], c[3]);
   ls->AddLine(c[3], c[0]);

   TEveViewer *ev = gEve->GetDefaultViewer();
   TGLViewer  *gv = ev->GetGLViewer();
   gv->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kFALSE, 0);

   TGLClip* clip = new Clipsi(gEve->GetDefaultGLViewer()->GetRnrCtx(), &c[0]);
   clip->SetMode(TGLClip::kOutside);
   // gEve->GetGlobalScene()->SetRnrChildren(false);
   gEve->GetGlobalScene()->GetGLScene()->SetClip(clip);

   gEve->FullRedraw3D(kTRUE);
}
