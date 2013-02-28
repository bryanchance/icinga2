/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012 Icinga Development Team (http://www.icinga.org/)        *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#include "i2-icinga.h"

using namespace icinga;

REGISTER_TYPE(User, NULL);

User::User(const Dictionary::Ptr& properties)
	: DynamicObject(properties)
{
	RegisterAttribute("display_name", Attribute_Config, &m_DisplayName);
	RegisterAttribute("macros", Attribute_Config, &m_Macros);
	RegisterAttribute("groups", Attribute_Config, &m_Groups);
}

User::~User(void)
{
	UserGroup::InvalidateMembersCache();
}

void User::OnAttributeChanged(const String& name, const Value& oldValue)
{
	if (name == "groups")
		UserGroup::InvalidateMembersCache();
}

User::Ptr User::GetByName(const String& name)
{
	DynamicObject::Ptr configObject = DynamicObject::GetObject("User", name);

	return dynamic_pointer_cast<User>(configObject);
}

String User::GetDisplayName(void) const
{
	if (!m_DisplayName.IsEmpty())
		return m_DisplayName;
	else
		return GetName();
}

Dictionary::Ptr User::GetGroups(void) const
{
	return m_Groups;
}

Dictionary::Ptr User::GetMacros(void) const
{
	return m_Macros;
}

Dictionary::Ptr User::CalculateDynamicMacros(const User::Ptr& self)
{
	Dictionary::Ptr macros = boost::make_shared<Dictionary>();

	{
		ObjectLock olock(self);
		macros->Set("CONTACTNAME", self->GetName());
		macros->Set("CONTACTALIAS", self->GetName());
	}

	macros->Seal();

	return macros;
}
