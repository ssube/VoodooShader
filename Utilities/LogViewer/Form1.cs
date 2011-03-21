using System;
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

            mBasePath = System.IO.Directory.GetCurrentDirectory() + "\\Resources\\";

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
            if ( System.IO.File.Exists(mTempFile) )
            {
                try
                {
                    System.IO.File.Delete(mTempFile);
                }
                catch (System.Exception ex)
                {
                	
                }
            }
        }
    }
}
