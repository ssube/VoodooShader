using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;

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

            foreach (String remote in m_RemotePaths)
            {
                // Create the filename
                String filename = "remote_" + VoodooHash.Hash(remote) + ".xml";

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
