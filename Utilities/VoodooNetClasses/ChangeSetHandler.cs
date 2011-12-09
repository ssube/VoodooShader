/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2011 by Sean Sube
 *
 * The Voodoo Shader Framework is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US
 *
 * Support and more information may be found at
 *   http://www.voodooshader.com
 * or by contacting the lead developer at
 *   peachykeen@voodooshader.com
 */
using System;
using System.Collections.Generic;
using System.Text;

namespace VoodooNetClasses
{
    public class ChangeSetHandler
    {
        PackageManifest m_Package;
        List<Version> m_Versions;
        List<String> m_File_Create, m_File_Remove;

        public ChangeSetHandler(PackageManifest p)
        {
            m_Package = p;
            if (m_Package != null)
            {
                m_Versions = new List<Version>(m_Package.Versions);
            }
        }

        public Version CollateChanges(Version v_from, Version v_to)
        {
            // If from is the direct parent of to, we can simply return to.
            if (v_from != null && v_to != null && v_to.Parent == v_from.Id)
            {
                return v_to;
            }

            Stack<Version> changechain = new Stack<Version>();

            if (v_to == null)
            {
                // If to is null, we're uninstalling everything (file create in remove, ignore remove)
                foreach (Version v in m_Versions)
                {
                    Version v2 = new Version();
                    v2.Description = v.Description;
                    v2.Id = String.Format("Remove {0}", v.Id);
                    v2.Messages = v.Messages;
                    v2.Parent = v.Parent;
                    v2.Create = v.Remove;

                    changechain.Push(v2);
                }
            }
            else
            {
                // Otherwise, we're installing something, so walk up.
                //! @todo This assumes a linear chain and is not optimal for branches.
                Version check = v_to;
                while (check != null)
                {
                    changechain.Push(check);
                    if ((v_from != null && check.Parent == v_from.Id) || check.Parent == null)
                    {
                        break;
                    }
                    else
                    {
                        check = m_Versions.Find(v => v.Id == check.Parent);
                    }
                }
            }
            
            // Compile the stack into a final changeset
            m_File_Create = new List<String>();
            m_File_Remove = new List<String>();

            while (changechain.Count > 0)
            {
                Version c_change = changechain.Pop();

                if (c_change.Remove != null && c_change.Remove.File != null)
                {
                    foreach (String removal in c_change.Remove.File)
                    {
                        if (m_File_Create.RemoveAll(s => s == removal) == 0)
                        {
                            m_File_Remove.Add(removal);
                        }
                    }
                }

                if (c_change.Create != null && c_change.Create.File != null)
                {
                    m_File_Create.AddRange(c_change.Create.File);
                }
            }

            String sourceID = v_from == null ? null : v_from.Id;
            String destID = v_to == null ? null : v_to.Id;

            Version finalversion = new Version();
            finalversion.Id = Guid.NewGuid().ToString();
            finalversion.Description = String.Format("__{0} to {1}__", sourceID, destID);
            finalversion.Create = new ChangeSet();
            finalversion.Create.File = m_File_Create.ToArray();
            finalversion.Remove = new ChangeSet();
            finalversion.Remove.File = m_File_Remove.ToArray();
            return finalversion;
        }
    }
}
