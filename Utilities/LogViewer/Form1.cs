using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Xml.XPath;
using System.Xml.Linq;

namespace LogViewer
{
    public partial class mainwindow : Form
    {
        public mainwindow()
        {
            InitializeComponent();
        }

        private void openLog(object sender, EventArgs e)
        {
            openFileDialog1.Filter = "Xml Logs (*.xmllog)|*.xmllog|All Files|*";

            if ( openFileDialog1.ShowDialog() == DialogResult.OK )
            {
                String logfilename = openFileDialog1.FileName;

                XDocument logdoc = XDocument.Load(logfilename);
                XElement logroot = logdoc.Root;

                IEnumerable<XElement> moduleCollection = logroot.XPathSelectElements("./Module");
                IEnumerable<XElement> msgCollection = logroot.XPathSelectElements("./Message");

                XAttribute timeAttr = logroot.Attribute("time");
                XAttribute dateAttr = logroot.Attribute("date");
                String logTime = (timeAttr == null) ? "unknown" : timeAttr.Value;
                String logDate = (dateAttr == null) ? "unknown" : dateAttr.Value;
                textSummary.Text = String.Format
                (
                    "Voodoo log file created on {0} at {1}. {2} modules were registered and {3} events logged.",
                    logDate, logTime, moduleCollection.Count(), msgCollection.Count()
                );

                IEnumerator<XElement> moduleIter = moduleCollection.GetEnumerator();
                while ( moduleIter.MoveNext() )
                {
                    XAttribute nameAttr = moduleIter.Current.Attribute("name");
                    String name = ( nameAttr == null ) ? String.Empty : nameAttr.Value;
                    XAttribute majorAttr = moduleIter.Current.Attribute("major");
                    String major = (majorAttr == null) ? String.Empty : majorAttr.Value;
                    XAttribute minorAttr = moduleIter.Current.Attribute("minor");
                    String minor = (minorAttr == null) ? String.Empty : minorAttr.Value;
                    XAttribute patchAttr = moduleIter.Current.Attribute("patch");
                    String patch = (patchAttr == null) ? String.Empty : patchAttr.Value;
                    XAttribute revAttr = moduleIter.Current.Attribute("rev");
                    String rev = (revAttr == null) ? String.Empty : revAttr.Value;
                    XAttribute debugAttr = moduleIter.Current.Attribute("debug");
                    String debug = (debugAttr == null) ? String.Empty : debugAttr.Value;
                    gridModules.Rows.Add(name, major, minor, patch, rev, debug);
                }

                long eventNum = 0;
                IEnumerator<XElement> msgIter = msgCollection.GetEnumerator();
                while (msgIter.MoveNext())
                {
                    XAttribute sevAttr = msgIter.Current.Attribute("severity");
                    XAttribute srcAttr = msgIter.Current.Attribute("source");
                    XAttribute tickAttr = msgIter.Current.Attribute("ticks");
                    String sev = (sevAttr == null) ? String.Empty : sevAttr.Value;
                    String src = (srcAttr == null) ? String.Empty : srcAttr.Value;
                    String tick = (tickAttr == null) ? String.Empty : tickAttr.Value;
                    String msg = msgIter.Current.Value;
                    int rowID = gridMessages.Rows.Add(++eventNum, sev, tick, src, msg);
                    int color = 0;
                    if (!String.IsNullOrEmpty(sev))
                    {
                        try
                        {
                            color = System.Convert.ToInt32(sev); 
                        }
                        catch (System.Exception ex)
                        {
                        	
                        }
                    }
                    gridMessages.Rows[rowID].DefaultCellStyle.BackColor = Color.FromArgb(color, 128, 128);
                }
            }
        }
    }
}
