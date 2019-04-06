////////////////////////////////////////////////////////////////////////////
//	Module 		: alife_object.cpp
//	Created 	: 27.10.2005
//  Modified 	: 27.10.2005
//	Author		: Dmitriy Iassenev
//	Description : ALife object class
////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "xrServer_Objects_ALife.h"
#include "alife_simulator.h"
#include "xrServer_Objects_ALife_Items.h"

#pragma warning(push)
#pragma warning(disable:4238)
void CSE_ALifeObject::spawn_supplies		()
{
	spawn_supplies(*m_ini_string);
}

void CSE_ALifeObject::spawn_supplies(LPCSTR ini_string)
{
	if (!ini_string)
		return;

	if (!xr_strlen(ini_string))
		return;

	CInifile ini(&IReader((void*)(ini_string), xr_strlen(ini_string)), FS.get_path("$game_config$")->m_Path);

	if (ini.section_exist("spawn"))
	{
		LPCSTR N, V;
		float p;
		for (u32 k = 0, j; ini.r_line("spawn", k, &N, &V); k++)
		{
			VERIFY(xr_strlen(N));

			float f_cond = 1.0f;
			bool bScope = false;
			bool bSilencer = false;
			bool bLauncher = false;
			bool alt_scope = false;
			xr_string st;
			int sId = 0;

			j = 1;
			p = 1.f;

			if (V && xr_strlen(V)) 
			{
				string64 buf;
				j = atoi_17(_GetItem(V, 0, buf));
				if (!j)	j = 1;

				bScope = (nullptr != strstr(V, "scope"));
				bSilencer = (nullptr != strstr(V, "silencer"));
				bLauncher = (nullptr != strstr(V, "launcher"));

				//probability
				if (strstr(V, "prob="))
					p = (float)atof(strstr(V, "prob=") + 5);

				if (fis_zero(p)) 
					p = 1.0f;

				if (strstr(V, "cond="))
					f_cond = (float)atof(strstr(V, "cond=") + 5);

				if (strstr(V, "scope="))
				{
					alt_scope = true;
					sId = atoi_17(strstr(V, "scope=") + 6);
				}
			}

			for (u32 i = 0; i < j; ++i)
			{
				if (randF(1.f) < p) 
				{
					CSE_Abstract* E = alife().spawn_item(N, o_Position, m_tNodeID, m_tGraphID, ID);
					//подсоединить аддоны к оружию, если включены соответствующие флажки
					CSE_ALifeItemWeapon* pWeapon = smart_cast<CSE_ALifeItemWeapon*>(E);
					if (pWeapon)
					{
						if (alt_scope)
						{
							pWeapon->m_scope_idx = sId;
						}

						if (pWeapon->m_scope_status == ALife::eAddonAttachable)
							pWeapon->m_addon_flags.set(CSE_ALifeItemWeapon::eWeaponAddonScope, bScope);
						if (pWeapon->m_silencer_status == ALife::eAddonAttachable)
							pWeapon->m_addon_flags.set(CSE_ALifeItemWeapon::eWeaponAddonSilencer, bSilencer);
						if (pWeapon->m_grenade_launcher_status == ALife::eAddonAttachable)
							pWeapon->m_addon_flags.set(CSE_ALifeItemWeapon::eWeaponAddonGrenadeLauncher, bLauncher);
					}
					CSE_ALifeInventoryItem* IItem = smart_cast<CSE_ALifeInventoryItem*>(E);
					if (IItem)
						IItem->m_fCondition = f_cond;
				}
			}
		}
	}
}

bool CSE_ALifeObject::keep_saved_data_anyway() const
{
	return (false);
}
#pragma warning(pop)
