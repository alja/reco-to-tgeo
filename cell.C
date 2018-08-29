void cell()
{
   TEvePointSet* ps = new TEvePointSet("cell");
   gEve->AddElement(ps);


   ps->SetNextPoint(180.5827, 15.7989, 0.0000);
   ps->SetNextPoint(181.2725, -0.0000, 0.0000);
   ps->SetNextPoint(181.2725, -0.0000, -15.7906);
   ps->SetNextPoint(180.5827, 15.7989, -15.7906);
   ps->SetNextPoint(287.9751, 25.1945, 0.0000);
   ps->SetNextPoint(289.0751, -0.0000, 0.0000);
   ps->SetNextPoint(289.0751, -0.0000, -25.1813);
   ps->SetNextPoint(287.9751, 25.1945, -25.1813);
}
