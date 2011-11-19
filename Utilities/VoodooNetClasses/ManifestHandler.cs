using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using System.Security.Cryptography;

namespace VoodooNetClasses
{
    class ManifestHandler
    {
        String m_LocalPath;
        List<String> m_RemotePaths;

        public ManifestHandler(String localPath, List<String> remotePaths = null)
        {
            m_LocalPath = localPath;
            m_RemotePaths = remotePaths;

            SyncRemote();
            CollateLocal();
        }

        public bool SyncRemote()
        {
            if (m_RemotePaths == null)
            {
                return false;
            }

            MD5 hasher = System.Security.Cryptography.MD5.Create();

            foreach (String remote in m_RemotePaths)
            {
                // Create the filename
                hasher.Clear();
                byte[] remoteBytes = System.Text.Encoding.Unicode.GetBytes(remote);
                byte[] hashBytes = hasher.ComputeHash(remoteBytes);
                StringBuilder sb = new StringBuilder(hashBytes.Length);
                sb.Append(m_LocalPath).Append("\\");
                foreach (byte b in hashBytes)
                {
                    sb.Append(b.ToString("X2"));
                }
                sb.Append(".xml");
                String filename = sb.ToString();

                if (File.Exists(filename))
                {
                    File.Delete(filename);
                }

                WebClient wc = new WebClient();
                wc.DownloadFile(remote, filename);
            }

            return true;
        }

        public void CollateLocal()
        {

        }
    }
}
