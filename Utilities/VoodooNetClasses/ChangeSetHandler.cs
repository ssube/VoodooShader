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
    class ChangeSetHandler
    {
        PackageManifest m_Package;
        List<String> m_File_Create, m_File_Remove;

        public ChangeSetHandler(PackageManifest p)
        {
            m_Package = p;
        }

        public void CollateChanges(Version v_from, Version v_to)
        {
            List<Version> versions = new List<Version>(m_Package.Versions);

            // Find the first common parent
            Dictionary<Version, Boolean> searched = new Dictionary<Version, Boolean>();
            Version c_from = v_from, c_to = v_to;

            while (true)
            {
                if (c_to.Parent == null || searched.ContainsKey(c_from))
                {
                    break;
                }
                c_to = versions.Find(v => v.Id == c_to.Parent);

                if (c_from.Parent == null || searched.ContainsKey(c_from))
                {
                    break;
                }
                c_from = versions.Find(v => v.Id == c_from.Parent);
            }

            String parent_id = null;
            if (c_check != null)
            {
                parent_id = c_check.Id;
            }

            // Get the change stack
            Stack<Version> changechain = new Stack<Version>();
            for (c_check = c_to; c_check.Parent != parent_id; versions.Find(v => v.Id == c_check.Parent))
            {
                changechain.Push(c_check);
            }

            // Compile the stack into a final changeset
            m_File_Create = new List<String>();
            m_File_Remove = new List<String>();

            while (changechain.Count > 0)
            {
                Version c_change = changechain.Pop();

                foreach (String removal in c_change.Remove.File)
                {
                    if (m_File_Create.RemoveAll(s => s == removal) == 0)
                    {
                        m_File_Remove.Add(removal);
                    }
                }

                m_File_Create.AddRange(c_change.Create.File);
            }
        }
    }
}
