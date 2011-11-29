using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace VoodooGUI
{
    public partial class ScanForm : Form
    {
        static BackgroundWorker worker;

        List<String> errors;
        List<KeyValuePair<String, String>> results;
        Dictionary<String, String> search;
        int currentDirs;

        public ScanForm()
        {
            InitializeComponent();

            search = new Dictionary<string, string>();
            search.Add("morrowind.exe", "Morrowind");
            search.Add("oblivion.exe", "Oblivion");
            search.Add("hl2.exe", "Source Engine");
            search.Add("nwmain.exe", "Neverwinter Nights");
            search.Add("nwn2main.exe", "Neverwinter Nights 2");
            errors = new List<String>();
            results = new List<KeyValuePair<String, String>>();

            // Estimate scan size
            progressBar1.Maximum = EstimateScan();

            // Basic counter
            worker = new BackgroundWorker();
            worker.ProgressChanged += new ProgressChangedEventHandler(worker_ProgressChanged);
            worker.DoWork += new DoWorkEventHandler(worker_DoWork);
            worker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(worker_RunWorkerCompleted);
            worker.WorkerReportsProgress = true;
            worker.RunWorkerAsync(search);
        }

        int EstimateScan()
        {
            int count = 0;
            foreach (DriveInfo drive in DriveInfo.GetDrives())
            {
                try
                {
                    foreach (DirectoryInfo innerdir in drive.RootDirectory.GetDirectories())
                    {
                        try
                        {
                            count += innerdir.GetDirectories().Length;
                        }
                        catch (Exception)
                        {
                        }
                    }
                }
                catch (Exception)
                {
                }
            }
            return count;
        }

        void worker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            KeyValuePair<Int64, TimeSpan> info = (KeyValuePair<Int64, TimeSpan>)e.Result;

            foreach (KeyValuePair<String, String> result in results)
            {
                dataGridView1.Rows.Add(true, result.Key, result.Value);
            }
            textBox2.Lines = errors.ToArray();

            MessageBox.Show(String.Format("Scanned {0} files in {1} seconds, found {2} results.", info.Key, info.Value.TotalSeconds, results.Count));
        }

        void worker_DoWork(object sender, DoWorkEventArgs e)
        {
            DateTime start = DateTime.Now;
            Int64 count = 0;
            foreach (DriveInfo drive in DriveInfo.GetDrives())
            {
                try
                {
                    CountDir(ref count, drive.RootDirectory, 0, e);
                }
                catch (Exception exc)
                {
                    worker.ReportProgress(1, exc.Message);
                }
            }
            e.Result = new KeyValuePair<Int64, TimeSpan>(count, DateTime.Now - start);
        }

        void worker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            if (e.ProgressPercentage == 0)
            {
                progressBar1.Value = ++currentDirs;
            }
            else if (e.ProgressPercentage == 1)
            {
                textBox1.Text = String.Format("Scanned {0} files, found {1} results.", e.UserState, results.Count);
            }
            else if (e.ProgressPercentage == 2)
            {
                errors.Add((String)e.UserState);
            }
            else if (e.ProgressPercentage == 3)
            {
                results.Add((KeyValuePair<String, String>)e.UserState);
            }
        }

        private Int64 CountDir(ref Int64 count, DirectoryInfo dir, int level, DoWorkEventArgs e)
        {
            ++level;
            FileInfo[] files = dir.GetFiles();

            foreach (FileInfo file in files)
            {
                String name;
                if (((Dictionary<String, String>)e.Argument).TryGetValue(file.Name.ToLower(), out name))
                {
                    worker.ReportProgress(3, new KeyValuePair<String,String>(name, file.FullName));
                }
            }

            count += files.LongLength;
            if (level < 6) { worker.ReportProgress(1, count); }

            foreach (DirectoryInfo innerdir in dir.GetDirectories())
            {
                try
                {
                    CountDir(ref count, innerdir, level, e);
                }
                catch (Exception exc)
                {
                    worker.ReportProgress(2, exc.Message);
                }

                if (level == 2) { worker.ReportProgress(0); }
            }

            return count;
        }
    }
}
