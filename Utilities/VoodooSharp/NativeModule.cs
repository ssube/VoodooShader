/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
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
using System.Runtime.InteropServices;
using System.Reflection;

namespace VoodooSharp
{
    public struct ClassInfo
    {
        public Guid ClassID;
        public String Name;

        public ClassInfo(Guid iClassID, String iName)
        {
            ClassID = iClassID;
            Name = iName;
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct VersionInfo
    {
        public Guid LibID;
        public Int32 Major;
        public Int32 Minor;
        public Int32 Patch;
        public Int32 Build;
        public bool Debug;
        [MarshalAs(UnmanagedType.LPWStr)]
        public string Name;
        [MarshalAs(UnmanagedType.LPWStr)]
        public string GitID;
    };

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    delegate IntPtr NativeModule_PluginInitFunc();

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    delegate UInt32 NativeModule_ModuleCountFunc();

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    delegate IntPtr NativeModule_ModuleInfoFunc(UInt32 index, out Guid ClassID);

    public class NativeModule
    {
        private IntPtr m_Module;

        private NativeModule_PluginInitFunc m_PluginInitFunc;
        private NativeModule_ModuleCountFunc m_ModuleCountFunc;
        private NativeModule_ModuleInfoFunc m_ModuleInfoFunc;

        private VersionInfo m_Version;
        private UInt32 m_Count;
        private  Dictionary<UInt32, ClassInfo> m_Classes;

        [DllImport("kernel32.dll")]
        private static extern IntPtr LoadLibraryEx(String dllname, IntPtr hfile, UInt32 flags);

        [DllImport("kernel32.dll")]
        private static extern IntPtr GetProcAddress(IntPtr hModule, String procname);

        [DllImport("kernel32.dll")]
        private static extern bool FreeLibrary(IntPtr hModule);

        public NativeModule(String filename)
        {
            UInt32 flags = 0;
            if (System.IO.Path.IsPathRooted(filename))
            {
                flags = 0x00000008;
            }

            m_Module = LoadLibraryEx(filename, IntPtr.Zero, flags);
            if (m_Module == IntPtr.Zero)
            {
                throw new Exception("Unable to load " + filename);
            }

            IntPtr fptr = GetProcAddress(m_Module, "PluginInit");
            m_PluginInitFunc = Marshal.GetDelegateForFunctionPointer(fptr, typeof(NativeModule_PluginInitFunc)) as NativeModule_PluginInitFunc;

            fptr = GetProcAddress(m_Module, "ClassCount");
            m_ModuleCountFunc = Marshal.GetDelegateForFunctionPointer(fptr, typeof(NativeModule_ModuleCountFunc)) as NativeModule_ModuleCountFunc;

            fptr = GetProcAddress(m_Module, "ClassInfo");
            m_ModuleInfoFunc = Marshal.GetDelegateForFunctionPointer(fptr, typeof(NativeModule_ModuleInfoFunc)) as NativeModule_ModuleInfoFunc;

            if (m_ModuleInfoFunc == null || m_ModuleCountFunc == null || m_ModuleInfoFunc == null)
            {
                throw new Exception("Not a native Voodoo module.");
            }

            IntPtr versionptr = m_PluginInitFunc();
            m_Version = (VersionInfo)Marshal.PtrToStructure(versionptr, typeof(VersionInfo));

            m_Count = m_ModuleCountFunc();

            m_Classes = new Dictionary<UInt32, ClassInfo>();

            for (UInt32 i = 0; i < m_Count; ++i)
            {
                Guid tClassID = Guid.Empty;
                IntPtr nameptr = m_ModuleInfoFunc(i, out tClassID);
                if (nameptr != null)
                {
                    String tname = Marshal.PtrToStringUni(nameptr);
                    m_Classes.Add(i, new ClassInfo(tClassID, tname));
                }
            }
        }

        ~NativeModule()
        {
            if (m_Module != IntPtr.Zero)
            {
                FreeLibrary(m_Module);
            }
        }

        public VersionInfo Version
        {
            get
            {
                return m_Version;
            }
        }

        public UInt32 Count
        {
            get
            {
                return m_Count;
            }
        }

        public ClassInfo this[UInt32 i]
        {
            get
            {
                return m_Classes[i];
            }
        }
    }
}
