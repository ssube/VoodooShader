using System;
using System.IO;
using System.Windows.Forms;
using System.Xml.Xsl;

namespace LogViewer
{
    public partial class mainwindow : Form
    {
        XslCompiledTransform mTransform;
        String mBasePath;
        String mTempFile;
        Uri mDefaultPage, mTempPage;

        public mainwindow()
        {
            InitializeComponent();

            mBasePath = Path.GetDirectoryName(Application.ExecutablePath) + @"\Resources\";

            try
            {
                mTransform = new XslCompiledTransform(true);
                mTransform.Load(mBasePath + "VoodooLog.xsl");
            }
            catch (System.Exception ex)
            {
            	System.Windows.Forms.MessageBox.Show
                (
                    "Unable to compile XSL transform:\n" + ex.Message,
                    "XSL Error"
                );
            }
            
            mTempFile = mBasePath + "temp.html";
            mDefaultPage = new Uri(mBasePath + "default.html", UriKind.Absolute);
            mTempPage = new Uri(mTempFile, UriKind.Absolute);
            mainBrowser.Url = mDefaultPage;
        }

        private void openLog(object sender, EventArgs e)
        {
            mainBrowser.Url = mDefaultPage;

            openFileDialog1.Filter = "Xml Logs (*.xmllog)|*.xmllog|All Files|*";

            if ( openFileDialog1.ShowDialog() == DialogResult.OK )
            {
                String logfilename = openFileDialog1.FileName;

                try
                {
                    mTransform.Transform(logfilename, mTempFile);
                    mainBrowser.Url = mTempPage;
                }
                catch (System.Exception ex)
                {
                    System.Windows.Forms.MessageBox.Show
                    (
                        "Unable to load log:\n" + ex.Message,
                        "Load Error"
                    );
                }
            }
        }

        private void closeForm(object sender, FormClosingEventArgs e)
        {
            if ( File.Exists(mTempFile) )
            {
                try
                {
                    File.Delete(mTempFile);
                }
                catch (System.Exception ex)
                {
                	
                }
            }
        }

        private void saveHTML(object sender, EventArgs e)
        {
            if ( saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                String destfname = saveFileDialog1.FileName;
                if (destfname == mTempFile)
                {
                    System.Windows.Forms.MessageBox.Show("Unable to save over temp HTML log.");
                    return;
                }
                else
                {
                    try
                    {
                        String destpath = Path.GetDirectoryName(destfname);
                        File.Copy(mTempFile, destfname, true);
                        File.Copy(mBasePath + "VoodooLog.css", destpath + "VoodooLog.css", true);
                        File.Copy(mBasePath + "sorttable.js", destpath + "sorttable.js", true);
                    }
                    catch (Exception exc)
                    {
                        System.Windows.Forms.MessageBox.Show("Error saving log:\n"+exc.Message);
                    }
                }
            }
        }
    }
}
