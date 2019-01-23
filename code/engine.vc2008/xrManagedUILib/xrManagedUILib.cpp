#include "stdafx.h"
#include "xrManagedUILib.h"

using namespace XRay;

Test::Test()
{
	//CUILayoutInflater* layoutInflater = new CUILayoutInflater;
	//CUIViewGroup* view = (CUIViewGroup*) layoutInflater->Inflate("ui_mm_loading_screen.xml", nullptr);

	CUIXml xml;
	xml.Load(CONFIG_PATH, UI_PATH, "ui_mm_loading_screen.xml");

	XML_NODE* pNode = xml.GetRoot()->FirstChild();
	while (pNode)
	{
		XML_NODE* pElement = pNode->ToElement();
		if (pElement)
		{
			Log::Info(gcnew String(pElement->Value()));
			if (strstr(pElement->Value(), "ViewGroup"))
			{
				CUIViewGroup* view = new CUIViewGroup(xml, pElement);
				Log::Info("view group is created");
			}
			else if (strstr(pElement->Value(), "View"))
			{
				CUIView* view = new CUIView(xml, pElement);
				Log::Info("view is created");
			}
		}
		pNode = pNode->NextSibling();
	}
}

Test::~Test()
{

}