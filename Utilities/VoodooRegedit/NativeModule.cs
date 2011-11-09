using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Reflection;

namespace VoodooRegedit
{
    public class Pair<T, U>
    {
        public Pair()
        {
        }

        public Pair(T first, U second)
        {
            this.First = first;
            this.Second = second;
        }

        public T First { get; set; }
        public U Second { get; set; }
    };

    [StructLayout(LayoutKind.Sequential)]
    public struct ModuleVersion
    {
        [MarshalAs(UnmanagedType.LPWStr)]
        public string Name;
        public Guid LibID;
        public Int32 Major;
        public Int32 Minor;
        public Int32 Patch;
        public Int32 Rev;
        public bool  Debug;
    };

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate IntPtr NativeModule_VersionFunc();

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate UInt32 NativeModule_CountFunc();

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate IntPtr NativeModule_InfoFunc(UInt32 index, out Guid clsid);

    class NativeModule
    {
        IntPtr m_Module;

        NativeModule_VersionFunc m_VersionFunc;
        NativeModule_CountFunc m_CountFunc;
        NativeModule_InfoFunc m_InfoFunc;

        ModuleVersion m_Version;
        UInt32 m_Count;
        Dictionary<UInt32, Pair<Guid, String>> m_Classes;

        [DllImport("kernel32.dll")]
        public static extern IntPtr LoadLibraryEx(String dllname, IntPtr hfile, UInt32 flags);

        [DllImport("kernel32.dll")]
        public static extern IntPtr GetProcAddress(IntPtr hModule, String procname);

        [DllImport("kernel32.dll")]
        public static extern bool FreeLibrary(IntPtr hModule);

        public NativeModule(String filename)
        {
            m_Module = LoadLibraryEx(filename, IntPtr.Zero, 0x00000008);
            if (m_Module == IntPtr.Zero)
            {
                throw new Exception("Unable to load " + filename);
            }

            IntPtr fptr = GetProcAddress(m_Module, "ModuleVersion");
            m_VersionFunc = Marshal.GetDelegateForFunctionPointer(fptr, typeof(NativeModule_VersionFunc)) as NativeModule_VersionFunc;

            fptr = GetProcAddress(m_Module, "ClassCount");
            m_CountFunc = Marshal.GetDelegateForFunctionPointer(fptr, typeof(NativeModule_CountFunc)) as NativeModule_CountFunc;

            fptr = GetProcAddress(m_Module, "ClassInfo");
            m_InfoFunc = Marshal.GetDelegateForFunctionPointer(fptr, typeof(NativeModule_InfoFunc)) as NativeModule_InfoFunc;

            IntPtr versionptr = m_VersionFunc();
            m_Version = (ModuleVersion)Marshal.PtrToStructure(versionptr, typeof(ModuleVersion));

            m_Count = m_CountFunc();

            m_Classes = new Dictionary<UInt32, Pair<Guid, String>>();

            for (UInt32 i = 0; i < m_Count; ++i)
            {
                //IntPtr tpclsid = IntPtr.Zero;
                Guid tclsid = Guid.Empty;// = (Guid)Marshal.PtrToStructure(tpclsid, typeof(Guid));
                IntPtr nameptr = m_InfoFunc(i, out tclsid);
                if (nameptr != null)
                {
                    String tname = Marshal.PtrToStringUni(nameptr);
                    m_Classes.Add(i, new Pair<Guid, String>(tclsid, tname));
                }
            }
        }

        ~NativeModule()
        {
            FreeLibrary(m_Module);
        }

        public ModuleVersion Version
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

        public Pair<Guid, String> this[UInt32 i]
        {
            get
            {
                return m_Classes[i];
            }
        }
    }
}
